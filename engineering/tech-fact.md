# tech fact
google 工程师Jeff Dean 首先在他关于分布式系统的ppt文档列出来的，关于计算机与软件的现状：

[记录](http://highscalability.com/numbers-everyone-should-know)

[视频](http://sites.google.com/site/io/building-scalable-web-applications-with-google-app-engine)

1纳秒 = 10亿分之一秒 = 10^-9 秒 

Numbers Everyone Should Know
|英文|中文|值|m值|L1为基数|L1为1s基数|
|-|-|-|-|-|-|
L1 cache reference|读取CPU的一级缓存|0.5 ns||1|1s
Branch mispredict|(转移、分支预测)|5 ns||10|10s
L2 cache reference|读取CPU的二级缓存|7 ns||14|14s
Mutex lock/unlock|互斥锁\解锁|100 ns||200|3m20s
Main memory reference|读取内存数据|100 ns||200|3m20s
Compress 1K bytes with Zippy|1k字节压缩|10,000 ns||20,000|5h33m
Send 2K bytes over 1 Gbps network|在1Gbps的网络上发送2k字节|20,000 ns||40,000|11h6m
Read 1 MB sequentially from memory|从内存顺序读取1MB|250,000 ns||500,000|5.78d
Round trip within same datacenter|从一个数据中心往返一次，ping一下|500,000 ns|0.5ms|1,000,000|11.57d
Disk seek|磁盘搜索|10,000,000 ns |10ms|20,000,000|231d
Read 1 MB sequentially from network|从网络上顺序读取1兆的数据|10,000,000 ns|10ms|20,000,000|231d
Read 1 MB sequentially from disk|从磁盘里面读出1MB|30,000,000 ns |30ms|60,000,000|694d
Send packet CA->Netherlands->CA|一个包的一次远程访问|150,000,000 ns|150ms|300,000,000|3472d

其中，内存和磁盘的访问速度是指随机访问，那么相差1000,000倍，但如果是顺序访问的话大约为7倍。

如果读取L1缓存是打个响指，互斥锁加锁需要三分钟，网络送出去2kB需要半天，内存读1MB数据需要近一周。
磁盘动一下需要7个月，网络上读1M需要7个月，从磁盘读取1MB需要近两年，网络上包往返需要10年左右。

1. 为什么需要多级缓存？

2. 为什么L1缓存造不大？