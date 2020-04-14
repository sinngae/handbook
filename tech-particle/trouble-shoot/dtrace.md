# 动态追踪技术
DTrace，Dynamically trace，动态追踪。

## perf
perf，linux的性能分析工具，基于linux内核子系统linux性能计数器，提供性能分析框架，比如硬件（CPU/PMU(Performance Monitoring Unit))/功能/软件（软件计数器/tracepoint）统计功能。

三个事件：
+ 硬件事件，PMU产生，特定条件下探测性能事件是否发生以及发生的次数，比如cache命中
+ 软件事件，内核产生的事件，内核各模块事件，如进程切换、tick数
+ tracepoint事件，内核中静态tracepoint所触发的事件，用于判断程序运行期间内核的行为细节，比如slab分配器的分配次数等。

## Dtrace
strace的大哥？