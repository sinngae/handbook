# linux
Linux内核五大模块，进程调度模块/内存管理模块/文件系统模块/进程间通信模块/网络接口模块。

## shell
shell script

## 内存管理

## 进程管理

## 信号

## 内核模块编写

## epoll/cgroup/coredump

## gcc/git

## 常用shell命令
```sh
## 查看cpu占用高的线程
ps H -eo user,pid,ppid,tid,time,%cpu,cmd --sort-%cpu

## 查看os版本
cat /etc/os-release
uname -a

# win文件乱码
iconv -f gb2312 a.txt -o a2.txt
```

## man命令
```sh
man [shell cmd]
man 2 [sys call]
# 1 用户命令，可由任何人启动的
# 2 系统调用，即由内核提供的函数
# 3 例程，即库函数，比如标准C库libc
# 4 设备，即/dev目录下的特殊文件
# 5 文件格式描述，例如/etc/passwd
# 6 游戏，不用解释啦
# 7 杂项，例如宏命令包、惯例等
# 8 系统管理员工具，只能由root启动
# 9 其他（?Linux?特定的），用来存放内核例行程序的文档
# n 新文档，可能要移到更适合的领域
# o 老文档，可能会在一段期限内保留
# l 本地文档，与本特定系统有关的
```