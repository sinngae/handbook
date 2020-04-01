# Linux C API
应该叫做Linux OSPI in C，OS program interface。
## unistd.h
```c
/* 闹钟函数，设置一个指定秒数定时器（覆盖旧值，每个进程仅一个闹钟时间），时间到时，向进程发送SIGALRM信号（进程可忽略，默认终止该进程）。
返回
*/
unsigned int alarm（unsigned int seconds);
```