# getrlimit & setrlimit

头文件  
    #include <sys/resource.h>

函数原型  
```
int getrlimit(int resource, struct rlimit *rlim);
int setrlimit(int resource, const struct rlimit *rlim);
```
功能  
每个进程都有一组资源限制，其中一部分可以通过getrlimit和setrlimit函数查询和更改。进程资源限制通常是在初始化时由进程0建立的，然后由每个后继进程继承。每种实现都可以对各种限制做出调整。

参数
```
struct rlimit {
    rlim_t rlim_cur;  /* Soft limit(软限制) */
    rlim_t rlim_max;  /* Hard limit (硬限制，是软限制的上限)(ceiling for rlim_cur) */
};

/*resource参数的取值可为下列其一*/
RLIMIT_AS           // 进程虚拟内存（地址空间，Address Space）的最大字节长度。该限制会影响brk、mmap和mremap等。
RLIMIT_CORE         // core文件的最大字节长度。超出这个大小的core文件会被截短。指定0则表示不产生core文件。
RLIMIT_CPU          // CPU时间的使用限制（秒）。进程达到软限制时，会收到一个SIGXCPU信号（默认会终止进程。但进程可以捕获该信号）。如果进程继续消耗CPU时间，它会每秒收到一个SIGXCPU信号，直到达到硬限制，并接收到SIGKILL信号（不同的实现在此处可能会有差别）。
RLIMIT_DATA         // 进程数据段（初始化数据节、未初始化数据节和堆）的最大字节长度。该限制会影响brk和sbrk等。
RLIMIT_FSIZE        // 进程所能创建的文件的最大字节长度。
RLIMIT_LOCKS        // 进程可创建的flock锁和fcntl租借锁的总数（租借锁是Linux特有的：fcntl可通过F_SETLEASE命令对文件加读或写的租借锁。当另一个进程尝试打开或截短该文件而产生冲突时，内核会通过信号通知持有租借锁的进程。后者应当对此作出响应，如flush缓冲区或移除租借锁等）。
RLIMIT_MEMLOCK      // 进程使用mlock能够锁定在RAM中的最大字节长度（防止被换出到交换分区。内存的锁定和解锁以页为单位）。该限制会影响mlock、mlockall和mmap等。
RLIMIT_MSGQUEUE     // 调用进程的实际用户所能分配的Posix消息队列的最大字节长度。
RLIMIT_NICE         // 进程可通过setpriority() 或 nice()调用设置的最大完美值。linux 2.6.12+
RLIMIT_NOFILE       // 进程所能打开（如使用open/pipe/socket）的文件描述符的最大值加1。注意，进程间的文件描述符是独立的。超出该限制会抛出EMFILE错误。
RLIMIT_NPROC        // 调用进程的实际用户所能创建进程（在Linux上，更准确的说法是线程）的最大数目。超出该限制时，fork会失败并抛出EAGAIN错误。
RLIMIT_RSS          // 最大驻内存集字节长度(RSS)
RLIMIT_RTPRIO       // 进程可通过sched_setscheduler 和 sched_setparam设置的最大实时优先级。
RLIMIT_SIGPENDING   // 用户可拥有的最大挂起信号数。
RLIMIT_STACK        // 最大的进程堆栈，以字节为单位。
```

规则(unix中的规则要求)
+ 1.任何进程都可以将一个软限制更改为小于或等于其硬限制值。
+ 2.对于普通用户来说，任何进程都可降低其硬限制值，但必须大于等于软限制值。
+ 3.只有超级用户进程可以提高硬限制值。
