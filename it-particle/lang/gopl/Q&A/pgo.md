# Go PGO（Profile-Guided Optimization，配置文件引导优化）
> **版本前提**
> - Go1.20：预览版，需要手动 `-pgo` 开启
> - Go1.21+：正式生产可用；`go build` 默认 `-pgo=auto`，主包目录存在 `default.pgo` 自动启用PGO
> - **输入只能是 pprof CPU profile**，不是 heap、block 等其他profile。
> - 原理：拿**真实业务负载的CPU采样**给编译器，编译器对高频函数做激进内联、接口调用虚拟化、代码布局优化；通常性能收益 **2%~14%**，CPU密集型服务收益更明显

## 完整三步流程
1. 采集业务真实负载的 cpu profile
2. 将profile放到main包目录，命名 `default.pgo`
3. 重新编译，编译器基于这个profile做PGO优化

## 一、采集CPU Profile（两种方式）
### 方式1：http服务（net/http/pprof，最常用，线上服务）
```go
import _ "net/http/pprof"

func main() {
    go func() {
        http.ListenAndServe("localhost:6060", nil)
    }()
    // 业务逻辑
}
```
压测/跑真实流量一段时间后，抓取cpu profile：
```bash
# 采集30秒CPU采样，输出cpu.pprof
go tool pprof -http= localhost:6060/debug/pprof/profile?seconds=30
# 或者直接curl保存
curl [http://127.0.0.1:6060/debug/pprof/profile](http://127.0.0.1:6060/debug/pprof/profile)\?seconds=30 > cpu.pprof
```

### 方式2：普通程序（runtime/pprof，非http）
```go
package main

import (
    "os"
    "runtime/pprof"
)

func main() {
    f,_ := os.Create("cpu.pprof")
    defer f.Close()
    pprof.StartCPUProfile(f)
    defer pprof.StopCPUProfile()

    // 执行业务逻辑
}
```

> ⚠️ 采集要点：
> 1. **必须是代表性流量**：和线上真实业务负载一致；如果压测场景和真实流量差异巨大，PGO反而可能负优化。
> 2. 采样时间：30s~几分钟足够，不要太短。
> 3. 不要用开发机器小流量profile去优化生产二进制。

## 二、编译启用PGO
把采集好的 `cpu.pprof` 复制到**main包目录**，重命名 `default.pgo`
```bash
mv cpu.pprof ./default.pgo
```

直接build，自动识别default.pgo开启PGO（Go1.21+）
```bash
go build -o app
```

手动指定profile路径（多场景profile切换，不使用default.pgo）
```bash
go build -pgo=./cpu.pprof -o app
```

关闭PGO
```bash
go build -pgo=off -o app
```

### ✅ 验证是否成功启用PGO
```bash
go version -m ./app
```
输出里看到 `pgo=path/to/default.pgo` 代表PGO生效。

## 三、编译器做哪些优化？
1. **高频函数更激进内联**：profile标记的热点函数放宽内联阈值
2. **接口调用虚拟化**：如果profile显示接口总是只有一个具体类型，编译器把间接调用转为直接调用，进一步内联
3. 代码布局优化：热点代码放在一起，提升i-cache命中率
4. 影响整个程序，**包括标准库**，不只是自己写的业务代码

## 四、工程最佳实践 & 坑点
1. **建议把 default.pgo 提交进git仓库**
   profile作为编译输入，保证CI/CD构建可复现；不同架构可以维护多个profile，构建时选择对应profile。
2. profile和二进制架构尽量匹配：GOOS/GOARCH不一致，效果会变差。
3. **业务代码大重构后，必须重新采集profile！**
   函数、调用关系大变，旧profile会失效，甚至性能倒退。
4. PGO是**编译期优化**，不影响运行时pprof采集，不侵入业务逻辑。
5. 不是万能：IO密集型服务收益很小；**CPU密集型（编解码、计算、协议解析）收益最明显**。
6. 不能混用：heap profile、goroutine profile**不能**给PGO使用，只认CPU profile。

## 五、CI流水线示例
```yaml
#1 用基准环境跑压测，采集cpu.pprof
#2 重命名 default.pgo 放到源码main目录
#3 go build 自动启用PGO编译
#4 压测对比 baseline（不带PGO版本），验证QPS/延迟收益
```

## 六、面试精简一句话总结
Go PGO是配置引导编译优化，使用业务真实负载采集的pprof CPU profile，编译阶段对热点函数做激进内联、接口虚拟化、指令布局优化；Go1.21默认auto，main目录放置default.pgo自动开启，适合CPU密集服务；**如果profile的负载场景和真实业务差异大，会出现负优化**。

如果你需要，我可以写一段**基准测试代码，对比开启PGO前后性能差异**。