# 动态追踪技术
DTrace，Dynamically trace，动态追踪。Sun开发的DTrace是动态跟踪技术的鼻祖。

## perf
perf，linux的性能分析工具，基于linux内核子系统linux性能计数器(2.6.31首次加入，称为Performace counter，仅仅作为PMU的抽象；2.6.32中正式改名为Performance Event，可以处理所有性能相关的事件)。

perf提供性能分析框架，比如硬件（CPU/PMU(Performance Monitoring Unit))/功能/软件（软件计数器/tracepoint）统计功能。

三个事件：
+ 硬件事件，PMU产生，特定条件下探测性能事件是否发生以及发生的次数，比如cache命中
+ 软件事件，内核产生的事件，内核各模块事件，如进程切换、tick数
+ tracepoint事件，内核中静态tracepoint所触发的事件，用于判断程序运行期间内核的行为细节，比如slab分配器的分配次数等。

perf可以做什么：
+ 计算每个时钟周期内的指令数（IPC值），IPC偏低表明CPU利用率低。
+ 做程序的函数级别采样，图形化分析程序的性能瓶颈。
+ 替代strace，添加动态内核probe点。
+ 做benchmark衡量调度器的好坏

### 硬件特性，PMU
CPU cache/SMP cache/CPU流水线及超标量体系结构及乱序执行/CPU分支预测/内核tracepoint

PMU，Perfomance monitor unit，为了揭示程序对CPU的使用情况，而加入的硬件设计。软件使用PMU针对某种硬件事件设置计数器，处理器便会统计该事件的发生次数（硬件执行，程序执行无感知），当产生的次数超过设置的值，便产生中断。内核捕获这些中断，以考察程序对硬件特性的利用效率。

tracepoint时内核源代码里散落的hook，使能后，可以在程序运行到特定代码时，触发hook，被用于各种trace/debug工具，如perf。如内核的内存管理模块，slab分配器中的tracepoint/hook，内核运行到它时，便会通知perf。perf把tracepoint事件记录，生成报告。大家可以通过分析报告，了解程序运行细节。
### 常用命令
```sh
# 安装，内核高于2.6.31即可
yum install perf

# 查看所有能够触发perf采样点的事件，2.3k多种
#   三类：硬件事件/软件事件/tracepoint事件
perf list   # man perf-list

# 输出被调用程序运行的整体情况和汇总数据
#   task-clock              CPU时间片使用量及占用率，该值越高，说明越倾向于CPU计算型
#   context-switches        进程切换次数及频率
#   cpu-migrations          cpu迁移次数及频率，（CPU迁移指被调度器从一个CPU转移到另一个CPU）
#   page-faults             页错误次数及频率
#   cycles                  处理器时钟量
#   instructions            机器指令量
#   branches                
#   branch-misses
#   seconds time elapsed    
#   seconds user
#   seconds sys
perf stat [exe file]
perf stat -e [event] # 指定事件

# 查看系统里的资源消耗“大户”
perf top
perf top -e [event] # 指定事件

# 程序运行及汇总数据写入文件
#   -e 指定事件
#   -g 使能调用图(call-gragh)，堆栈记录
perf record -e [event] [exe file] # 记录程序运行的某事件统计信息到perf.data
perf report # 查看perf.data
```
## Dtrace
