# 核心一句话
> Go 原生 runtime **不能自动检测局部死锁**（一部分G卡住，还有其他G正常跑）。sysmon 的 checkdead 只会检测「**全部G休眠**」的全局死锁。局部死锁必须靠**观测、工具、埋点**来主动发现。

下面分：**现象、工具、代码埋点、排查思路**，结合GMP一起理解。

## 一、局部死锁会出现什么现象？
特征：
1. 部分goroutine卡在channel/mutex/cond/wg.Wait，永远不动；
2. **还有其他goroutine正常跑，CPU/进程不退出，不会panic**；
3. 服务表现：部分接口超时、请求堆积、队列不消费，但是进程还活着；
4. 资源不释放：goroutine数量持续上涨，不会回落（最典型信号！）

> 重点指标：goroutine数量持续上涨，且大部分G状态是 `chan send` / `chan receive` / `semacquire`（mutex等待）

`go doc runtime.Stack` 里G的几种关键状态：
- `semacquire`：阻塞在sync.Mutex/RWMutex（等待锁）
- `chan send` / `chan receive`：阻塞在channel
- `condwait`：sync.Cond.Wait
- `IO wait`：网络阻塞（正常，不是死锁）
- `sleep`：time.Sleep（timer托管，正常休眠）

## 二、线上感知手段（优先级从高到低）
### 1. 监控：goroutine数量告警（最实用）
采集 `runtime.NumGoroutine()`，做监控大盘：
- 正常：请求上来G上涨，请求结束G回落；
- 局部死锁：G只增不减，持续线性上涨，不下降。

⚠️ 区分：
- 大量网络IO阻塞：G涨，但流量下降后G会慢慢降；
- 局部死锁：G一旦创建就永久卡住，不会释放，只涨不跌。

额外辅助指标：
- 阻塞的goroutine数量（通过pprof采集）
- 接口延迟、队列长度、pending请求数

### 2. pprof（最常用，线上可以抓）
```bash
# 获取goroutine堆栈
curl [http://127.0.0.1:6060/debug/pprof/goroutine?debug=2](http://127.0.0.1:6060/debug/pprof/goroutine?debug=2) > goroutine.stack
```
`debug=2` 会输出完整goroutine堆栈 + goroutine状态。

看文件，批量查找：
- `semacquire`：卡在mutex
- `chan send` / `chan receive`：channel阻塞
统计：**大量goroutine停在同一个代码行、相同调用栈**，基本就是局部死锁。

> 原理：死锁的G会永久停在同一行等待，堆栈不会变。正常请求G执行完就销毁。

### 3. go trace（适合复现环境，不适合长时间线上）
```go
// 埋点开启trace
f, _ := os.Create("trace.out")
defer f.Close()
trace.Start(f)
defer trace.Stop()
```
```bash
go tool trace trace.out
```
trace里可以看到goroutine生命周期：
看到大量G状态长期停在blocked，永远不被唤醒，就是死锁。

### 4. 第三方静态/动态检测工具
1. `go vet`：只能发现简单channel问题，**不能检测运行时死锁**
2. `goleak`（单元测试神器）
> 单元测试里检测goroutine泄漏，非常适合发现局部死锁。原理：测试结束后，检查是否还有额外goroutine没有退出。
```go
import "github.com/uber-go/goleak"
func TestXXX(t *testing.T) {
    defer goleak.VerifyTestMain(t)
    // 业务代码
}
```
单元测试跑完，如果有goroutine卡住不退出，直接报错。适合在CI阶段提前拦截。

3. `deadlock` 库（运行时锁检测）
`github.com/sasha-s/go-deadlock`
替换标准`sync.Mutex`，运行时记录锁获取顺序，**检测循环锁等待**。
> 注意：只针对Mutex循环等待死锁，channel死锁检测不到；性能有开销，一般只在测试环境打开，不上生产。

## 三、业务埋点方案（自己在代码里感知）
适合业务队列、worker场景：
1. **给每个goroutine加超时+心跳**
比如worker协程，每隔一段时间上报心跳时间戳；
监控检测：如果某个worker心跳长时间不更新，判定卡住。
> 适合消费协程、后台任务G。

2. 带超时的channel / select，不要无限阻塞
❌ 坏代码（永久阻塞，一旦没人发数据就局部死锁）
```go
val := <-ch
```
✅ 增加超时兜底，避免永久阻塞
```go
select {
case val := <-ch:
//处理
case <-time.After(3 * time.Second):
    //超时告警、打日志，退出协程，避免永久卡住
}
```
> 这是工程上最重要的防御手段：**不要写无超时的无限阻塞等待**

3. 给WaitGroup、Cond增加业务侧超时控制
sync原生没有超时，自己用select+context做超时兜底。

## 四、排查步骤（实操流程）
1. 告警：goroutine数量持续上涨
2. 拉pprof goroutine堆栈 debug=2
3. 筛选blocked的G，看阻塞位置：
    - 大量G卡在同一行 `mu.Lock()`：锁竞争/循环锁
    - 大量G卡在`ch <- x` / `<-ch`：channel消费方退出，不再读数据
4. 看调用栈，分析依赖关系，判断资源能不能被释放
5. 在测试环境用 go-deadlock / goleak复现

## 五、容易踩坑区分
1. 大量G阻塞在IO wait：socket读写，netpoll托管，**不是死锁**，连接关闭后会唤醒。
2. 大量G阻塞在semacquire/channel，且没有唤醒源 → **局部死锁**。

## 面试精简总结
1. Go runtime原生**无法自动检测局部死锁**，sysmon只检测全局全部休眠场景；
2. 最核心特征：goroutine只增不减；
3. 检测手段：监控NumGoroutine告警、pprof goroutine堆栈；测试阶段可以用goleak、go-deadlock；
4. 工程防御：所有阻塞操作尽量加`context/select超时`，避免永久阻塞G。

如果你想，我可以写一段**可复现的局部死锁demo**，你跑起来，用pprof抓堆栈看现象。