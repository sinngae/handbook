# 动态追踪技术
DTrace，Dynamical tracing，动态跟踪技术。Sun开发的DTrace是动态跟踪技术的鼻祖。
不影响程序运行地跟踪程序运行。

动态跟踪技术就像中医的把脉，现代的核磁。

计算机调试里，profiling指对应用程序的画像，描述的是程序使用CPU、内存等资源的情况。

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

perf原是用于linux内核调优的，比如内核中的热点代码/slab分配器。现在也可用于linux上应用程序的调优，如常规的热点调用、I/O统计等。是否进程切换频繁？或死循环？perf一下！

perf工具包有很多功能，知道一些功能很容易，有效解决问题依然有难度，就像练拳，姿势和套路都有，还需要实战融会贯通。

perf还可以使用perf/python脚本做扩展

### 硬件特性
CPU cache/SMP cache/CPU流水线及超标量体系结构及乱序执行/CPU分支预测/内核tracepoint

软件程序并不总是顺序执行的，遇到if-else/switch/while/for等等都会产生分支。CPU为了提高执行效率，为此设计了分支预测，概要来说就是根据历史执行记录来预判执行分支。分支预测对for/while等的结果比较理想，而对于switch-case的预测结果并不理想。（少用switch-case？）

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
#   cycles                  处理器时钟量 # 在vmware虚拟机上执行后续几个指标都是not supported，不知为何；后续在纯linux机器上测试
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
全局性统计：perf list/perf bench/perf test/perf stat/perf top

特定分析：perf kmem/perf lock/perf mem/perf sched/perf trace
### BTB分支预测失效
1. 外层循环10000次，内部循环20次 
2. 外层循环10000次，内部循环10次
3. 外层循环10次，内部循环10000次

三个版本，测试结果？

猜测：版本1内部的分支预测失败 X 外部循环10000次。如果反过来，分支预测失败仅有20次。分支预测关键常数16，高于16的优先预测？

perf stat可以看到分支预测失败的次数。
### tracepoint
tracepoint大约有2k个，每一个都很有意义，但都学习是一项缓慢痛苦却不得不做的事情。

### perf probe
如何在想观察的地方动态插入监测点呢？
systemTap早就实现了，但其安装并不容易，而perf也提供这样功能，perf probe。
```sh
# 在内核函数schedule的第12行加入动态probe点，相当于一个新的tracepoint
#   然后即可用perf record -e选择生成报表。
perf probe schedule:12 cpu
perf record -e probe:schedule -a sleep 1 # ?
```
### perf sched系列
调度器会影响系统运行效率，因此同一代码有不同的测评结果。perf sched是权威的统一测评工具。
对于调度器开发者很实用。
```sh
perf sched record            # low-overhead recording of arbitrary workloads
perf sched latency           # output per task latency metrics
perf sched map               # show summary/map of context-switching
perf sched trace             # output finegrained trace
perf sched replay            # replay a captured workload using simlated threads
```

**perf bench**
衡量性能的标准方法
### perf kmem
收集内核slab分配器的事件，如内存分配/释放等，可用于发现程序的内存分配、内存碎片、内存管理分析。
```sh
# 
perf kmem --alloc -l 10 --caller [exe file]
```
Ping-pong 是一种现象，在多 CPU 系统中，多个 CPU 共享的内存会出现”乒乓现象”。一个 CPU 分配内存，其他 CPU 可能访问该内存对象，也可能最终由另外一个 CPU 释放该内存对象。而在多 CPU 系统中，L1 cache 是 per CPU 的，CPU2 修改了内存，那么其他的 CPU 的 cache 都必须更新，这对于性能是一个损失。Perf kmem 在 kfree 事件中判断 CPU 号，如果和 kmalloc 时的不同，则视为一次 ping-pong，理想的情况下 ping-pone 越小越好。详见cache调优

### perf look
内核锁（还比较初级），perf kmem其实都是perf record的特例。
### perf timechart
直观的图形输出综合信息
```sh
# 记录数据，按Ctrl-C结束，时间太长，会导致最终图片很大
perf timechart record
#   另一个终端执行测试程序

# 生成svg图片
perf timechart
```

### 生成火焰图
与google chrome的调试火焰图类似。

perf火焰图(Flame Graph)，Linux优化大师Brendan Gregg使用perl语言发明。从全局到细节，逐层次展现时间分布、调用栈。
```sh
# 下载火焰图脚本
git clone https://github.com/brendangregg/FlameGraph
# 生成目标程序测试报告
#   -F 99表示每秒99次
#   -p [pid]进程号
#   -g 记录调用栈
#   sleep 30则是持续30秒
#   -a 表示记录所有cpu调用
perf record -F 99 -a -g [exe file] -- sleep [seconds]
perf record -F 99 -a -g ./foo -- sleep 10
# 生成火焰图
perf script -i perf.data &> perf.unfold
FlameGraph/stackcollapse-perf.pl perf.unfold &> perf.folded
FlameGraph/flamegraph.pl perf.folded > perf.svg
```

局限性，会出现两种情况：  
1. 调用栈不完整，有些只有前几层
2. 函数名缺失，只有内存地址（匿名函数）
3. 多进程火焰图？

如果生成的火焰图有十六进制乱码，可能是缺失了debuginfo，可用gdb -p pid 查看

**Broken stacks 问题**

常见的火焰图类型有：
+ On-CPU，压测CPU使用率饱和
+ Off-CPU，压测CPU上不来，阻塞在I/O或锁
如果On-CPU/off-CPU的火焰图差不多，可能是被其他进程抢占了
+ Memory
+ Hot/Code
+ Differential。

## systemtap
Linux上内核或应用程序的非功能性或功能性诊断工具，提供命令行和脚本语言，提供live analysis/programmable on-line response/whole-system symbolic access。

参考 https://sourceware.org/systemtap/getinvolved.html

### 内部tapset
### 探针 probe
### 安装
```sh
# install
yum install systemtap kernel-devel yum-utils debuginfo-install kernel

# 测试
stap -ve 'probe begin { log("hello world") exit () }'
stap -c df -e 'probe syscall.* { if (target()==pid()) log(name." ".argstr) }'
```
**一个stap例子**
```sh
#!/usr/bin/stap
global tm;

probe begin {
    printf("Begin...\n");
}

probe process("/usr/local/nginx/sbin/nginx").function("kcbgtcr").return {
    tm = gettimeofday_us();
}
probe process("/usr/local/nginx/sbin/nginx").function("kcbrls").return {
    printf("Time:%d\n", gettimeofday_us() - tm);
}
```

### mysql优化
MySQL处理binlog/UNDO/REDO/DO

## eBPF

## go pprof
go语言自带的profiling工具：
+ CPU profile，以一定频率采集应用程序在CPU上的数据
+ Memory profile，又称Heap profile
+ Block profile，gorotine不运行的情况，用于分析阻塞、查找死锁等性能瓶颈
+ Gorotine profile，goroutine的运行及其互相调用关系

wrk压测工具/go tool pprof生成分析报告/go-torch生成火焰图/
## Dtrace
（略）