# tcpdump
对网络上的数据包进行截获的包分析工具。 tcpdump可以将网络中传送的数据包的“头”完全截获下来提供分析。它支持针对网络层、协议、主机、网络或端口的过滤，并提供and、or、not等逻辑语句来帮助你去掉无用的信息。

## 常用命令
```sh
# 默认模式 - 监视第一个网络接口（网卡？）上所有的数据包
tcpdump
# 选项
#   -B [size]       - 
#   -c [count]      -
#   -C [file size]
#   -E [algo:secret]
#   -F [file]
#   -G [seconds]
#   -i [interface/eth name]   - 指定网络接口
#   -j [tstamtype]
#   -M [secret]
#   --number
#   -Q [in|out|inout]
#   -r [file]
#   -s [snaplen]    - 表示包大小
#   --time-stamp-precision [precision]
#   --immediate-mode
#   -T [type]
#   --version
#   -V [file]
#   -w [file name]  - 指定写入文件
#   -W [file count]
#   -y [datalinktype]
#   -z [postrotate-command]
#   -Z [User]
#   [expression]

# 常用
# 抓取eth0的包，写入a.acap
tcpdump -i eth0 -s 0 -w a.cap
# 查看抓包内容，默认仅摘要
tcpdump -r a.cap
# 使用Ascll显示
tcpdump -A -r a.cap
# 使用十六进制显示
tcpdump -X -r a.cap
# 抓某一端口的包可以使用

tcpdump -i eth0 port [端口号]

# ip排序
tcpdump -n -r a.cap |awk '{print$3}'|sort -u
# 只显示指定源IP
tcpdump -n src host [ip addr] -r a.cap
# 只显示指定目的IP
tcpdump -n dst host [ip addr] -r a.cap
# 只查看指定端口
tcpdump -n port [port num] -r a.cap
tcpdump -n udp port [port num] -r a.cap # udp port
# 常用 十六进制显示
tcpdump -n -X  port 80 -r a.cap

```