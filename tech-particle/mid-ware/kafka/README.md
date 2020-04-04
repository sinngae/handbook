# kafka
kafka是分布式的分区的/复制的/提交的日志服务。kafka的设计是作为一个统一的信息收集平台，能够实时地收集反馈信息，并需要能够支撑较大的数据集，并具备良好的容错能力。

1. 对消息保存时按topic分类，producer发送消息，consumer接收消息。
2. 每个kafka实例server成为一个broker，即缓存代理。
3. kafka集群、producer、consumer 依赖zk保证可用性。
4. 消息被消费后不会被立即删除，日志文件将按配置要求保存一定时间后删除，不论消息是否被消费。这样释放空间，减少消息消费对文件系统I/O开支，与JMS（如ActiveMQ）实现不同。
5. 对于consumer，需要保存消息消费的offset，offset可先行向前，也可被重置为任意值，以灵活消费消息。
6. offset信息曾保存在zk中，后来保存在kafka指定topic中。
7. producer与consumer是轻量级的，可随意离开，不会影响集群。
8. partition的设计目的：根本原因是kafka是基于文件存储的，用partition把日志分散在多个server上，避免单文件尺寸达到磁盘上限。每个partition被当前kafka实例保存；一个topic被分片到不同的partition上，提高消息保存和消费的效率，也容纳更多的consumer，提升并发消费的能力。
9. distribution：kafka按配置产生partition的备份个数（replicas）。每个partition将备份到多台机器以提高可用性。replicated方案调度多个备份：a.每个partition都有一个server为leader；leader负责所有的读写操作；作为leader的server承载了其所有的读写压力；b.如果leader失效，会有其他follower来接管并成为leader；c.follower只单调地和leader跟进，同步消息即可；d.有多少个partition就有多少个leader；kafka会将leader均衡地分散在每个实例上，保证负载均衡。
10. producer将消息发布到指定的topic中，也指定归属于哪个partition（比如基于round-robin方式（一种以轮询方式依次将一个域名解析到多个ip地址的调度不同服务器的算法round-robin DNS））
11. consumer kafka只支持topic：a.每个consumer只属于一个consumer group；每个group有多个consumer；发送到topic的消息只会被订阅此topic的每个group的一个consumer消费；b.如果所有的consumer都在同一group，则kafka起到queue的作用，消息会在consumer之间负载均衡；c.如果所有的consumer都在不同的group，那就是发布订阅模式，消息将会广播给所有consumer；d.可以认为一个group是一个订阅者，kafka只能保证一个partition中的消息被某个consumer消费时是顺序的，从topic角度，消息不是有序的，只能是整体有序；e.按kafka的设计，同一group不能有多于partition
12. kafka的保证：guarantees  a.发送到partition中的消息将会按照接收的顺序追加到日志中。b.对消费者，消费的顺序和日志中消息顺序一致。c.如果topic的replication factor为n，允许n-1个kafka实例失效。

## 使用场景
1. messaging 常规消息系统  
   a.partitions/replication/容错使kafka具有良好的扩展性和性能优势。
   b.kafka并未提供JMS的事务性、消息传输担保（消息确认机制）、消息分组等企业级特性。
   kafka只适用于常规的消息系统，尚未确保消息的发送和接收绝对可靠
2. website activity tracking  
   kafka作为“网站活性跟踪"的最佳工具。可将网页/用户操作等消息发到kafka上，并实时监控或离线统计分析。
3. log aggregation  
   kafka的特性适于做”日志收集中心“；
   a.application把操作日志**批量/异步**地发送到kafka集群，而不是保存到本地或者DB中。
   b.kafka可以批量提交消息或压缩消息。producer不会有太多性能开支。
   c.consumer端可以使用hadoop等其他系统化的存储和分析系统。

## 设计原理及特性
### 持久性
kafka使用文件存储消息，故性能上严重依赖文件系统的本身特性，且很难优化。文件缓存并直接做内存映射。kafka是对日志文件的append操作，很少磁盘检索。为减少写入次数/减少磁盘IO操作，broker将消息暂时缓存起来（buffer），当消息数量达到阀值，再flush到磁盘。
### 性能
磁盘IO、网络IO都会影响到kafka吞吐量。
producer端将消息缓存起来，达到数量阀值时，批量发送给broker；consumer端批量fetch到多条消息（最大消息数量可配置）。
broker端（似乎）sendfile调用可以提高网络IO的性能：将文件的数据映射到系统内存中，socket直接读取相应内存，无需进程再次copy和交换。
压缩机制需要消耗一定CPU资源，支持gzip/snappy等格式，不过kafka瓶颈更多在网络IO。
### 生产者
负载均衡：producer和topic下所有的partition leader保持socket连接；消息由producer通过socket发到broker中间没有任何“路由层”。producer决定消息路由到哪个partition，比如采用random/key-hash/轮询等（多个partition之间的消息负载均衡）。partition leader的位置（host::port）保存在zk中，producer通过注册watcher来监听partition leader的变更事件。
**异步发送**：多条消息缓存在buffer中，数据越小，延时越大（可以设置），提升了网络效率，但也导致producer异常时，尚未发送的消息丢失。
### 消费者
consumer向broker发送fetch请求，附带消息起始位置（offset），获取多条消息；consumer端offset可重置，来重新消费消息。
JMS中，消息位置由producer保存（？），以免重复发送消息；kafka中，consumer自己保存，并间歇同步到zk（新版存储到kafka中）。
### 消息传送机制
对于JMS，消息传输担保有且只有一次（exactly once）。
1. at most once，最多一次，“非持久化”，发送一次，不管成功失败。
consumer调用fetch取多条，处理异常，offset依然更新。
2. at least once，消息至少成功依次，失败重发。
consumer调用fetch，但offset没有保存到zk。
3. exactly once，消息只会发依次。
基于两阶段提交/事务。
kafka中，kafka优先采用2。
### 复制备份
1. follower只需同步消息（与consumer消费一样）
2. leader跟踪所有follower状态，如果follower落后太多或失效，就把它从relicas队列中删除。
3. 当所有follower都将一条消息保存成功，才称为是committed，消费者才消费得到。
4. 至少有一个replics中的实例存活即可，HBase需要多数派存活。
5. leader失效，会选取一个up-to-date的follower，且其server上不会有太多其他leader。
### 日志
1. 如果一个topic名为mytopic，有2个partition，那么日志保存在mytopic_0和mytopic_1两个目录中。
2. 日志文件中保存了一序列log-entries日志条目，每个log-entry格式为4字节的数字N表示消息的长度，N个字节的消息内容。
3. 每个日志都有一个offset来唯一标记一条消息，offset是一个8字节的数字，标识此消息在此partition中所处的起始位置。
4. 每个partition在物理存储层面，有多个logfile组成，称为segment。segment-file的命名为最小的offset，即起始消息的offset。所有partition中所持有的segment列表信息存储在zk中。
5. segment文件尺寸达到一定阀值时（默认1GB），将会创建一个新文件。
6. buffer中消息条数达到阀值，或距离最近一次flush时间差达到阀值，也会触发flush到日志文件
