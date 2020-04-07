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

## 会话
每个进程组必在一个会话里（仅用户态，内核进程有会话？），进程创建时默认成为父进程会话里的进程。一般地，会话ID即是其第一个进程的ID，第一个进程是该会话的leader。
每个会话有一个控制终端（controlling tty，/dev/tty，打开失败意味着没有）。
一般，一个会话创建于登录程序，登录的终端即控制终端（可能是tty或pty，真实终端或虚拟终端或串口等）。登录程序的所有后代进程都是该会话的成员进程。
```c
sid = getsid(); // 获取session id
sid = tcgetsid(fd); // fd - 控制终端

/* 创建一个新的会话
    当前进程不能是一个进程组leader，不然与新建进程组冲突。
    当前进程成为新建会话的leader，也成为新的进程组的leader，但并没有真正的控制终端。
    如何创建控制终端？
    ioctl(fd, TIOCSCTTY, ...);
*/
pid = setsid();

/* 创建daemon进程
    脱离控制终端
*/
if (fork() != 0)    // 退出父进程，子进程成为孤儿
    exit();
setsid();           // 创建新会话，子进程成为leader
if (fork() != 0)    // 子进程退出，孙进程成为非会话leader的孤儿  
    exit();
```
当控制终端断开连接，SIGHUP被发送给会话leader，任何从控制终端读取输入的read函数都会返回EOF错误（一般返回-1，errno置EIO）。
如果终端是虚拟终端的slave端，master端被关闭，slave端的前端进程组也会收到SIGHUP。
如果会话leader已退出，则发送给所有前端进程组进程。
一般会话leader是个任务控制shell，收到SIGHUP后会向它的后代进程都发送SIGHUP信号。如果会话leader不捕获处理SIGHUP，则会话leader会退出，所有的前端进程收到SIGHUP信号。

## 进程
### fork调用
```c
pid_t p
p = fork(); // 一般通过系统调用fork()新建进程
if (p == (pid_t)-1) {
    /* error */
} else if (p == 0) {    // 子进程作为父进程的一个拷贝，与父进程的空间大部分保持一致
    /* child */
    // 可以通过getppid()获取父进程PID
    setpgid(0, pgid);   // setpgid1
} else {    // 父进程获取到子进程的PID
    /* parent */
    setpgid(p, pgid);   // 不管setgpid1还是此处先调用，都确保子进程加入pgid
}


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
#### exec调用
fork调用创建当前进程的副本，所有资源“复制”（映射+拷贝？）到子进程。
exec调用加载一个可执行的二进制文件，替代当前运行的进程，并不创建新进程。
#### clone调用
clone调用大致同fork调用，但新创建的进程不独立与父进程，而是和父进程共享某些资源。可以指定需要共享或复制的资源，如内存数据、打开的文件、信号处理程序。
#### 命名空间
轻量级虚拟化技术，系统资源的隔离后的视图。
父命名空间全局ID不变，每个子命名空间中都有自己的ID序列。父命名空间里有子命名空间的对应ID。
**chroot调用**将进程限制到文件系统的某一部分。

### 信号
SIGKILL (die! now!), SIGTERM (please, go away), SIGHUP (modem hangup), SIGINT (^C), SIGQUIT (^\), 详见signal.md。
很多信号都有终止进程的默认行为（有些带有core文件生成，rlimit限制其大小）。
信号SIGCHLD 和 SIGWINCH 默认被忽略。
除了SIGKILL 和 SIGSTOP，其他的信号都可以被捕获/忽略/阻塞。详见signal手册。

### 进程组
每个进程都有所属的进程组，进程组用PGID标识（进程创建的时候，默认被加入父进程所在的进程组）。一般地，进程组的ID与其第一个进程ID相同（该进程被称为进程组leader）。
```c
pgid_t pgid = getpgrp();    // 获取当前进程的进程组ID
pgid_t pgid = getpgid(0);   // 获取当前进程的进程组ID
pgid_t pgid = getpgid(p);   // 获取进程p的进程组ID

/* 把进程pid加入已有的进程组pgid
    如果pgid == pid || pgid == 0，即新建一个进程组，leader为pid
    pid == 0，指代当前进程ID
    setpgrp()与setpgid(0, 0)等效
*/
setpgid(pid, pgid);

/* 向同一进程组的所有进程发送信号 */
killpg(pgrp, sig);
/* 父进程等待其所有子进程 */
waitpid(0, &status, ...);
/* leader 等待进程组里其他进程 */
waitpid(-pgrp, &status, ...);
```

```sh
ps -j # 查看进程，带有PPID/PID/PGID/SID

# 通过管道连接的多个命令，是多个进程，同一进程组。
```
#### 前端进程组
会话里的多个进程组，最多只有一个成为其前端进程组。tty的输入及信号(Ctrl-Z/Ctrl-C)都是发给前端进程组的。
```c
/* 进程查看其会话的前端进程组
    fd - 控制终端；
        /dev/tty指向控制终端，完全独立于标准输入输出的重定向
        ctermid()获取控制终端名，返回fd
    @return: >1 - 非进程组ID
*/
tcgetpgrp(fd);
/* 进程设置其会话的前端进程组，当前进程必须仍与控制终端对话中
    fd
    pgrp - 会话中的一个进程组，任意呢？
*/
tcsetpgrp(fd, pgrp)
```
#### 后端进程组
会话的多个进程组中非前端的就是后端的进程组。前端进程组与用户交互。
后端进程接收到SIGTTIN信号，意味着可以从终端读取输入（一般是用户使用fg命令切换任务）。
后端进程写入控制终端，可能会获得SIGTTOUT信号，一般是控制终端不可用？

#### 孤儿进程组
进程组中的leader进程在其他进程退出前终止，该进程组成为孤儿。特殊地，会话leader的进程组总是孤儿状态。
同一会话的控制终端进程（进程组的leader的父进程？）终止运行，会导致其他进程先收到SIGHUP信号，它们可能会结束运行，或者捕获信号处理，或者忽略，然后收到SIGCONT信号，继续运行。
会话leader已经是孤儿状态，所以会话leader终止时，没有信号被发送。

## 线程
一个进程可以有多个线程，新建线程与其父线程的PID相同。
linux线程的实现就是轻量级进程（LWP），linux的LWP的效率不比windows的线程效率低。
系统调用clone()，带有参数CLONE_THREAD，创建线程。
系统调用gettid()返回TID。
系统调用tkill()向线程发送信号。
```c
#include <unistd.h>
#include <sys/types.h>
#define CLONE_SIGHAND   0x00000800
#define CLONE_THREAD    0x00010000
#include <linux/unistd.h>
#include <errno.h>
_syscall0(pid_t,gettid)

int thread(void *p) {
        printf("thread: %d %d\n", gettid(), getpid());
}

main() {
        unsigned char stack[4096];
        int i;

        i = clone(thread, stack+2048, CLONE_THREAD | CLONE_SIGHAND, NULL);
        if (i == -1)
                perror("clone");
        else
                printf("clone returns %d\n", i);
        printf("parent: %d %d\n", gettid(), getpid());
}
```

## 深入linux进程
进程相关的元数据都保存在内核态Ring 0中，内核对进程的所有的操作都是针对task_struct数据结构（定义在include/sched.h），包含很多成员，可以与内核的多个子系统联系起来，其结构详见linux内核进程。

### 0号进程
idle进程，系统启动后第一个进程，运行在内核态（Ring 0），唯一一个没有通过fork/kernel_thread产生的进程。
完成加载系统后，演变为特殊的idle进程。

CPU每个核有一个运行队列，每个队列上有一个idle进程，系统的空闲时间即idle进程的“运行时间”。idle进程是最低优先级，即其他进程都没有的情况下，才会调度到idle进程，idle进程并不执行什么计算，而要做到节能和低退出延时。实际上idle进程使用CPU的hlt指令，使CPU处于暂停状态，等待硬件中断发生时恢复。
### 1号进程
init进程（centos上称为systemd），是idle通过kernel_thread创建。
内核空间初始化后，加载init程序，是系统其他用户进程的祖先。
init进程启动其他系统进程，然后变为守护进程监视系统其他进程。

不同的linux版本init进程的运行并不相同，用户的设置不同，init进程要做的事情也不同。

### 2号进程
kthreadd进程，是init通过kernel_thread创建，始终运行在内核空间，负责内核空间的调度和管理。
kthreadd负责管理和调度其他内核线程（kernel_thread），所有的内核线程都会直接或间接地以kthreadd为父进程（最新的内核，早期内核不是这样）

最新的内核里，2号进程做了更多的事情，1号进程的工作减少了。

### 用户进程
控制台是最早最原始的输入设备，TTY是远程的输入设备，终端是对输入设备的抽象。现在的控制台/TTY/终端都是虚拟的了，以键盘为输入、屏幕为输出，是控制台还是tty并不用区分，是否是远程取决于虚拟连接。
shell（sh/bash/zsh等）是用户与系统交互的程序，抽象了输入和输出设备。GUI是图形化的shell。虚拟终端与伪终端的区别？

tty，电传打字机，现在代指各种终端设备。
pty，虚拟tty，代指远程telnet或xterm或ssh的终端交互。
pts/ptmx，pty的实现方法，pts是pty slave，ptmx是pty master。

比如用户设置了开机运行sshd服务，init进程/kthreadd进程开机时就会启动shhd进程。当用户远程ssh登录，就会以sshd为父进程创建子进程（一般是一对进程pts/ptmx）提供服务，一般是运行一个bash进程。bash进程是新建了一个会话，bash是会话leader。
类似的还有telnet或其他自定义的服务。

再如用户登录系统（无GUI系统通过login进程登录），也是运行了bash。
再如用户登录GUI系统，运行gnome-terminal，是先运行了gnome-terminal，再运行了bash。