# Linux shell
bash命令
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