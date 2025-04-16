# trouble shoot
troube shooting?/性能调优

## gdb/valgrind/strace
运行时堆栈/寄存器/汇编原理/反编译

pmap 打印进程的内存地址映射信息
+ -x   extended       Show the extended format. 显示扩展格式
+ -d   device         Show the device format.   显示设备格式
+ -q   quiet          Do not display some header/footer lines. 不显示头尾行
+ -V   show version   Displays version of program. 显示版本

## 打印日志
侵入性强

## code review
人工审查，比较低效。

## 拆解代码
对代码分段屏蔽，查看运行情况
代码重构/代码分析工具

自动化代码审查（静态分析/坏味道）工具 sonar cppcheck
```sh
cppcheck
```

## linux工具
进线程工具：gdb(基于ptrace) valgrind strace

## 网络工具：
+ tcpdump 
+ netwox 工具包
+ wiresharke
+ fiddler
+ postman 
 
## 动态追踪技术
"进程活体分析技术"
+ Solaris的DTrace是动态追踪技术鼻祖，后来发展成一整套工具族和方法论
+ Linux内核一直没有引入DTrace，但是有perf/ftrace。RHEL引入了systemTap。后来内核引入了BPF/eBPF，基于BPF的bcc
+ pprof(go语言)