# 进程
thread/processs/process-group/session

线程/进程/进程组/会话

一个会话包含多个进程组，一个进程组包含多个进程，一个进程包含多个线程。一般有编号，那么就有了SessionID/Process-GroupID/ProcessID/ThreadID。

每个会话有一个控制终端（tty是Teletypes电传打字机的缩写，最早的“实时”控制台I/O设备，现在的设备包括虚拟控制台，串口及伪终端），会话中的进程组一般指前端运行的进程组。终端上输入的每个字符产生的中断信号，都会发送给该会话所有的前端进程。

**Linux top/ps中的列**  
+ PID，process ID
+ LWP，light weight process，线程？内核线程？
+ TGID，thread group ID for the thread group leader
+ PGID，process group ID
+ SID,  Session ID for the session leader
+ TPID，tty process group ID for the process group leader
+ TID，thread ID

**pthread id**
pthread库提供的ID生效范围不在系统级别。用处是？用户态调度线程？


## 进程生命周期
### fork调用
```c
pid_t p
p = fork(); // 一般通过系统调用fork()新建进程
if (p == (pid_t)-1)
    /* error */
else if (p == 0)    // 子进程作为父进程的一个拷贝，与父进程的空间大部分保持一致
    /* child */
    // 可以通过getppid()获取父进程PID
else    // 父进程获取到子进程的PID
    /* parent */

/* 退出进程（并返回一个字节n给父进程）：
    exit(n);
    return n; // main函数
    异常退出一般是信号触发
*/

//pid_t p;
int status;
p = wait(&status); // 
/* 结束了的进程，但没有被wait，被称为zombie，只保存两个字节：exit code和termination reason。
    如果父进程先退出，1号进程（init）成为子进程的父进程。
*/
```
### 信号
SIGKILL (die! now!), SIGTERM (please, go away), SIGHUP (modem hangup), SIGINT (^C), SIGQUIT (^\), 详见signal.md。
很多信号都有终止进程的默认行为（有些带有core文件生成，rlimit限制其大小）。
信号SIGCHLD 和 SIGWINCH 默认被忽略。
除了SIGKILL 和 SIGSTOP，其他的信号都可以被捕获/忽略/阻塞。详见signal手册。

## linux进程

### 0号进程
### 1号进程
### 2号进程
