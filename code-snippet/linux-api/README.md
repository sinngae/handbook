# Linux C/C++ API
使用C/C++在linux上开发，需要调用linux 操作系统API，或者操作系统提供的POSIX API，或者操作系统内核提供的API，或者linux上glibc等模块的API

## 举例
int pid = fork(); // pid>0 为父进程空间，pid为子进程id；pid=0为子进程空间

void(*signal(int sig, void (*handler)(int)))(int); // signal是一个函数，为sig安装处理函数。其返回值为void (*func)(int)，安装成功则返回系统默认的信号处理函数，出错则为SIGERR。其第一个参数为int sig，为软中断值。其第二个参数为void (*handler)(int)，为处理信号sig的函数。
