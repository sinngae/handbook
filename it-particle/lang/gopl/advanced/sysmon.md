# sysmon（runtime系统监控线程）
> 基础前提：**sysmon是独立M（OS线程），不带P，死循环周期性执行**；轮询间隔自适应，最小20μs，最大10ms；一旦发生抢占，会缩短轮询周期，提高监控灵敏度。
> 源码入口：`runtime/proc.go sysmon()`，所有功能都在循环内执行，刚好承接前面GMP、抢占、GC、netpoll的知识。

## 核心6大职责
### 1. retake()：调度抢占
两件事都在retake里：
1. **回收阻塞syscall的P**：发现M卡在阻塞系统调用，P绑定在这个M上；sysmon把P剥离，交给其他空闲M继续跑G，避免P被卡死。
2. **CPU密集G异步抢占（Go1.14+）**：遍历所有P，如果当前运行G超过**10ms**还没主动让出P，向M发送`SIGURG`信号，触发异步抢占，暂停这个G，防止死循环G永久霸占P。

### 2. netpoll 网络轮询，唤醒网络阻塞G
调用`netpoll(0)`，查询epoll/kqueue等IO多路复用，拿到就绪的网络G（如net/http、socket、SSE长连接阻塞的G），放到全局队列，等待调度运行。
> 补充：普通G调度时也会执行netpoll；但如果所有P都很忙，没人触发netpoll，**sysmon兜底执行netpoll**，防止网络事件没人处理。

### 3. GC相关监控 & 兜底强制GC
1. **2分钟兜底GC**：如果距离上一轮GC超过2分钟，堆内存一直达不到GOGC阈值，sysmon强制触发一轮GC，清理终结器、长期未回收资源。
2. 监控GC进度、GC pacing，配合GC调度；也会感知`GOMEMLIMIT`内存上限，内存逼近上限时协助触发GC。
3. 唤醒scavenger：把闲置很久（>5分钟）的物理内存span归还给操作系统，降低进程RSS内存占用。

### 4. 死锁检测 checkdead
检查：**所有G都阻塞，没有可运行G** → 判断死锁，直接panic抛出死锁错误。
> 只检测用户goroutine全部休眠的场景。

如果，仅有部分协程死锁，

### 5. M线程池水位管理
监控M数量，当M不足、任务堆积时，按需新建M；在空闲时管理、回收闲置M，控制OS线程数量，避免大量M占用内核资源。

### 6. 其他辅助监控
- 更新调度统计信息，给`schedtrace`、`trace`工具采集运行时指标
- 处理profile采样（CPU采样）
- 处理timer（部分超时定时器唤醒兜底）

## 一句话区分重点（面试常考）
sysmon**不是GC，不是调度器主体**，是**GMP调度器的后台看门狗**：
调度器负责正常G切换；sysmon负责兜底：syscall阻塞抢P、长时间运行G抢占、网络事件兜底、定时GC、内存归还、死锁检测。

## 常见误区
❌ sysmon在内核态运行：**用户态runtime代码，只是跑在独立OS线程上**
❌ sysmon是goroutine：不是，是独立M，**无P，不走GMP调度**
❌ sysmon负责执行GC标记：不负责，只触发GC；GC标记由mark worker G执行

## 串联整套知识链路
1. 用户G死循环跑10ms → sysmon扫描发现 → retake发送SIGURG抢占，夺回P
2. G阻塞在socket syscall → sysmon检测 → retake剥离P，P交给别的M
3. 程序低负载长期不分配内存 → sysmon2分钟到，强制GC
4. 网络IO事件没人poll → sysmon执行netpoll唤醒阻塞G
