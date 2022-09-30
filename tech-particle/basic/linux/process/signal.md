# 信号
+ SIGHUP - 用户终端结束时，终端的控制进程结束，通知同一session内的各个作业。

登录linux时，linux会分配给登录用户一个终端（session），在这个终端运行的前台、后台进程组都属于这个终端。用户退出登录时，前台进程组和后台对终端输出的进程组都会收到SIGHUP信号。这个信号的默认操作为终止进程。这个信号可以被捕获，如wget就会捕获并忽略它，就算终端退出，wget也会继续下载。另，与终端脱离关系的守护进程，这个信号用于通知它重新读取配置文件。
+ SIGINT - 程序终止的信号（interrupt），在用户键入INTR字符（通常是Ctrl-C）时发出，用于通知前台进程终止进程。
+ SIGQUIT - 和SIGINT类似，但由QUIT字符（通常是Ctrl-\）来控制，进程在因收到SIGQUIT退出时会产生core文件，在这个意义上类似与一个程序错误信号。
+ SIGILL - 执行了非法指令，通常是因为可执行文件本身出现错误，或者视图执行数据段、堆栈溢出时也有可能产生这个信号。
+ SIGTRAP - 断点指令或其他trap指令产生，由debugger使用
+ SIGABRT
+ SIGBUS
+ SIGFPE
+ SIGKILL - 用来立即结束程序的运行，本信号不能被阻塞、处理和忽略。如果管理员发现了某个进程终止不了，可以尝试发送这个信号。
+ SIGUSR1 - 留给用户使用
+ SIGSEGV
+ SIGUSR2 - 留给用户使用
+ SIGPIPE
+ SIGALRM - 时钟定时信号，计算的是实际的时间或始终时间。alarm函数使用该信号
+ SIGTERM - 程序结束信号，可以被阻塞、处理。通常用来要求程序自己正常退出，shell命令kill缺省产生这个信号。如果进程终止不了，我们才尝试SIGKILL
+ SIGCHLD - 子进程结束
+ SIGCONT - 
+ SIGSTOP - 停止（stopped）进程的执行。本信号不能被阻塞，可以用一个handler来让程序由stopped状态变为继续执行时完成特定的工作。例如，重新显示提示符。
+ SIGTTIN - 后端进程请求TTY的输入
+ SIGTTOUT - 后端进程发送到TTY的输出，且被stty tostop否定
+ SIGURG
+ SIGXCPU
+ SIGXFSZ
+ SIGVTALRM
+ SIGPROF
+ SIGWINCH
+ SIGIO
+ SIGPR
+ SIGSYS - 非法的系统调用
+ SIGTSTP - TTY信号，一般是Ctrl-Z被按下


## 信号分类
+ 程序不可捕获、阻塞、或忽略的信号有:SIGKILL、SIGSTOP
+ 不能恢复至默认动作的信号有：SIGKILL、SIGTRAP
+ 默认会导致进程流产的信号有：SIGABRT，SIGBUS、SIGFPE、SIGILL、SIGIOT、SIGQUIT、SIGSEGV、SIGTRAP、SIGXCPU、SIGXFSZ
+ 默认会导致进程退出的信号有：SIGALRM、SIGHUP、SIGINT、SIGKILL、SIGPIPE、SIGPOLL、SIGPROF、SIGSYS、SIGSYS、SIGTERM、SIGUSR1、SIGUSR2、SIGVTALRM
+ 默认会导致进程停止的信号有：SIGSTOP、SIGTSTP、SITTIN、SIGTTOUT
+ 默认进程忽略的信号有：SIGCHLD、SIGPWR、SIGURG、SIGWINCH

此外，SIGIO在svr4是退出，在4.3bsd中是忽略；SIGCONT在进程挂起时是继续，否则是忽略，不能阻塞。
