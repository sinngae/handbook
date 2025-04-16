GOTRACEBACK
---
none，不输出异常调用栈。
single(默认)，输出导致异常的调用栈。
all，输出用户创建的所有协程调用栈，第一个是异常流程的。
system，输出所有协程调用栈，包括Go框架底层创建的协程（比如垃圾标记和回收的协程），首个是异常。
crash，输出所有协程调用栈，并生成core。



可用GDB、 delve调试core文件

```sh
dlv core ./main core.4432 
bt # 展开的是协程异常栈
gr # 切换协程，或 查看当前协程信息
grs # 查看所有协程信息


```

手动kill Go程序 生成core文件，或者 gcore(gdb的命令，gdb下执行。不设置gotraceback可能缺少一些信息)