# strace
总的来说，用于跟踪进程的系统调用和信号：
+ 过滤指定的系统调用
+ 统计指定的系统调用的调用次数、耗时、成功率
+ 跟踪发送给进程的信号
+ 可以attach到运行着的进程pid

```sh
# 跟踪进程，可能会有大量输出
strace [exe] 2>&1
# 过滤指定文件
strace [exe] 2>&1|grep file.txt
# 过滤指定的系统调用，多个用逗号间隔，可以使用！反向
#   -e expr -- a qualifying expression: option=[!]all or option=[!]val1[,val2]...
#   options: trace, abbrev, verbose, raw, signal, read, write
#   -e trace=[file|process|network|signal|ipc] 仅跟踪某类调用
#   -e raw= 指定以十六进制显示
#   -e signal= 指定信号
#   -e read= 指定读取数据
strace [exe] -e [expr]
# 查看进程的状态，不用猜，就能看到进程在干啥
#   非常有意思，有兴趣的可以试下，运行top，然后看看top都执行了什么
strace -p [pid]
# 统计（从运行到ctrl-c退出的）进程的cpu时间、调用、错误等
#   -c -- count time, calls, and errors for each syscall and report summary
#   -C -- like -c but also print regular output
#   可看到一段时间内该进程的cpu时间都消耗在哪些函数上了，再用-e查看这些函数在做什么
strace -c -p [pid]

# 网络分析
#   替代tcpdump查找某些问题
strace -e poll,select,connect,recvfrom,sendto ping www.baidu.com
#   /var/run/nscd/socket?ping首先尝试连接NSCD，Name Service Cache Daemon（通常用来基于NIS,YP,LDAP或者类似的目录协议提供域名查询）。
#   sin_port=htons(53)，访问DNS服务（端口53），询问百度域名ip地址，然后访问百度ip


```

**strace看到进程输出很少，函数调用消耗cpu也少，是不是进程就不忙呢？**
可以了解一下ltrace（跟踪库函数调用）、perf-trace、trace-cmd、time、ptrace函数