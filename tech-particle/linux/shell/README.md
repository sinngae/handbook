# Linux shell
脚本语言与非脚本语言没有官方的定义，区别只在于设计的初衷，脚本语言常常作为一种临时方案或者补丁，设计者并不把它做成通用程序设计语言，没有考虑过用它构建大型软件。相反，通用程序设计语言，往往是多位专家小组严格设计，从一开始就考虑了通用性，以及再大型工程中的可靠性和可扩展性。

## shell script
```shell
## 仅对当前脚本进程起作用，对子进程不起作用
## 1.当命令的返回值为非零状态时，立即退出脚本的执行，而不是继续执行（很多shell默认执行）
## 如果想根据民工执行的返回值，输出log，则不适用此命令，而是通过exit命令来做退出
set -e
```

## bash命令
``` shell
## 查看cpu占用高的线程
ps H -eo user,pid,ppid,tid,time,%cpu,cmd --sort-%cpu

## 查看os版本
cat /etc/os-release

## 如何测算一个机器的io上限
vnstat -i eth0 -l # 实时流量情况
vnstat -i eth0 -h # 按小时查询流量情况
vnstat -i eth0 -d # 按天数查询流量情况
vnstat -i eth0 -m # 按月数查询流量情况
vnstat -i eth0 -w # 按周数查询流量情况
vnstat -i eth0 -t # 查询TOP10流量情况
## 更多命令帮助信息可以 vnstat --help 进行查看。

top
: '
us: is meaning of "user CPU time"
sy: is meaning of "system CPU time"
ni: is meaning of" nice CPU time"
id: is meaning of "idle"
wa: is meaning of "iowait" 
hi：is meaning of "hardware irq"
si : is meaning of "software irq"
st : is meaning of "steal time"
us 用户空间占用CPU百分比
sy 内核空间占用CPU百分比
ni 用户进程空间内改变过优先级的进程占用CPU百分比
id 空闲CPU百分比
wa 等待输入输出的CPU时间百分比
hi 硬件中断
si 软件中断 
st: 实时（来源http://bbs.chinaunix.net/thread-1958596-1-1.html）
'
```

win7 文件乱码
``` shell
iconv -f gb2312 a.txt -o a2.txt
```