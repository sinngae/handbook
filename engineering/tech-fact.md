# tech fact
google 工程师Jeff Dean 首先在他关于分布式系统的ppt文档列出来的，关于计算机与软件的现状：

[记录](http://highscalability.com/numbers-everyone-should-know)

[视频](http://sites.google.com/site/io/building-scalable-web-applications-with-google-app-engine)

1纳秒 = 10亿分之一秒 = 10^-9 秒 

Numbers Everyone Should Know
|英文|中文|值|
|-|-|-|
L1 cache reference|读取CPU的一级缓存|0.5 ns
Branch mispredict|(转移、分支预测)|5 ns
L2 cache reference|读取CPU的二级缓存|7 ns
Mutex lock/unlock|互斥锁\解锁|100 ns
Main memory reference|读取内存数据|100 ns
Compress 1K bytes with Zippy|1k字节压缩|10,000 ns
Send 2K bytes over 1 Gbps network|在1Gbps的网络上发送2k字节|20,000 ns
Read 1 MB sequentially from memory|从内存顺序读取1MB|250,000 ns
Round trip within same datacenter|从一个数据中心往返一次，ping一下|500,000 ns
Disk seek|磁盘搜索|10,000,000 ns 
Read 1 MB sequentially from network|从网络上顺序读取1兆的数据|10,000,000 ns
Read 1 MB sequentially from disk|从磁盘里面读出1MB|30,000,000 ns 
Send packet CA->Netherlands->CA|一个包的一次远程访问|150,000,000 ns

其中，内存和磁盘的访问速度是指随机访问，那么相差1000,000倍，但如果是顺序访问的话大约为7倍。