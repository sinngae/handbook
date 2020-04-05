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