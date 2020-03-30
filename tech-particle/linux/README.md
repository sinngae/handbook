# linux
研究Linux内核

## shell
shell script

## 内存管理

## 进程管理

## 信号

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