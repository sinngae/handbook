# 1.磁盘io 命令 
sar -b 1 10 
iostat

# 6.shell脚本实现统计文本中各字符串出现频率
shell sort命令，排序
wc 统计字符
awk 逐行处理
可以先导入数据库，然后count(*) group by

# 14.怎么衡量计算机的负载是高还是低
vmstat 输出一条记录，子系统上次重起后到现在的平均值

vmstat -s 显示各种统计计数和内存统计信息

mpstat 实时系统监控，与cpu有关的统计信息，sysstat安装包里的命令

mpstat -P ALL 每个cpu的使用情况都打印出来，按cpu编号

mpstat 2 100 每2秒刷新一次，刷新100次

uptime 打印系统总共已经运行了多长时间和系统的最近1分钟5分钟15分钟平均负载

iostat 查看系统io状态 -c 查看cpu信息-d磁盘信息

sar -u 1 3 统计cpu使用情况，间隔1秒，统计3次

# 6.查看进程打开了哪些文件
lsof -c mysql

lsof -u username

lsof -p pid

lsof -i // 列出所有网络连接

lsof -i tcp // 列出所有tcp连接

lsof -i :3306 // 列出所有使用端口的信息

lsof -i tcp:80

lsof -a -u test -i // 列出某个用户的所有活跃的网络端口
