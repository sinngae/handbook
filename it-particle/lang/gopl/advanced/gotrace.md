Go trace
---
`go tool tace`是一个强大的性能分析工具，能够对Go程序的行为进行可视化追踪，帮助分析和诊断GC、goroutine调度、系统调用阻塞等。

多种视图：
+ View trace, 基础时间线试图，展示程序执行的各个阶段
    + 时间线（执行的时间单元）、堆的分配和释放、协程数量及状态、OS线程、虚拟处理器P、协程和事件
+ goroutine analysis，插卡程序中协程分布和状态
+ Network blocking profile，网络同步阻塞情况
+ Synchronizatoin blocking profile，同步阻塞情况
+ syscall blocking profile，系统调用
+ scheduler latency profile，调度延迟概况
+ User defined tasks & regions, 用户自定义任务和区域
+ minimum mutator utilization，最小修改器利用率


```go
package main
import (
    "os"
    "runtime/trace"
)

func main() {
    f, _ := os.Create("trace.out")
    defer f.Close()

    trace.Start(f)
    defer trace.Stop()

    for i := 0; i < 10000; i ++ {
        _ = make([]byte, 1 <<20)
    }
}
```

go trace 有一定性能损耗，只能用于非生产环境。