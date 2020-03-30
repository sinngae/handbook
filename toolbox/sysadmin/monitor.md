# 系统资源监控
系统管理员必备资源监控工具

## top系列
top/iotop/iftop
```sh
top
#    us: is meaning of "user CPU time"
#    sy: is meaning of "system CPU time"
#    ni: is meaning of" nice CPU time"
#    id: is meaning of "idle"
#    wa: is meaning of "iowait" 
#    hi: is meaning of "hardware irq"
#    si: is meaning of "software irq"
#    st: is meaning of "steal time"
#    us: 用户空间占用CPU百分比
#    sy: 内核空间占用CPU百分比
#    ni: 用户进程空间内改变过优先级的进程占用CPU百分比
#    id: 空闲CPU百分比
#    wa: 等待输入输出的CPU时间百分比
#    hi: 硬件中断
#    si: 软件中断 
#    st: 实时（来源http://bbs.chinaunix.net/thread-1958596-1-1.html）
```

## stat系列
vmstat/iostat/mpstat/vnstat
```sh
# 怎么衡量计算机的负载是高还是低
vmstat          # 输出一条记录，子系统上次重起后到现在的平均值
vmstat -s       # 显示各种统计计数和内存统计信息
mpstat          # 实时系统监控，与cpu有关的统计信息，sysstat安装包里的命令
mpstat -P ALL   # 每个cpu的使用情况都打印出来，按cpu编号
mpstat 2 100    # 每2秒刷新一次，刷新100次
uptime          # 打印系统总共已经运行了多长时间和系统的最近1分钟5分钟15分钟平均负载

# 磁盘io 命令 
iostat          # 查看系统io状态 -c 查看cpu信息-d磁盘信息
sar -u 1 3      # 统计cpu使用情况，间隔1秒，统计3次
sar -b 1 10

## 如何测算一个机器的io上限
vnstat -i eth0 -l # 实时流量情况
vnstat -i eth0 -h # 按小时查询流量情况
vnstat -i eth0 -d # 按天数查询流量情况
vnstat -i eth0 -m # 按月数查询流量情况
vnstat -i eth0 -w # 按周数查询流量情况
vnstat -i eth0 -t # 查询TOP10流量情况
## 更多命令帮助信息可以 vnstat --help 进行查看。
```

## 其他
free