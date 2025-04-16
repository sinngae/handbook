# Redis
an open source(BSD licensed), in-memory data structure store, used as a database, cache and message broker.

BSD协议开源的，in-memory数据结构存储的，作为数据库、缓存和消息的中介代理。


redis可达10w/s读写，满足高性能。redis的value可以是数值/字符串/双向循环链表（消息队列）/Set（可实现tag系统，一种灵活的日志分类方式）/json结构，还支持过期时间。
redis的value的最大长度远超memcache？

## 缓存设计思想
key->value
map？hashmap？hashtable？
O(1)的查找复杂度

哈希表是用数组实现的，数组的元素是链表。
简单地处理碰撞，最差的时候，链表没用上，数组无限大；或者数组没用上，链表无限长。这是退化。
怎么办，rehash
java8里的hashmap在元素增大到一定程度，就会从链表转成红黑树。

### 缓存雪崩
redis做缓存，却因为大量的key同时失效（可能是启动时批量加载的），导致大量连接穿透redis取访问数据库，导致数据库崩溃。
批量加载的数据最好有个随机值？雪崩了如何处理才能再次拉起呢。

缓存穿透，缓存和数据库中都没有的数据，被黑客用来一直发起请求。比如对于id自增的数据访问id=-1的请求。需要在接入层做安全，比如用户验证、参数合法检查等。redis 4的布隆过滤器。

缓存击穿，有些热点数据的访问量很大，其失效的瞬间，请求量落到了数据库。

## 持久化
AOF（Append-Only File） 
RDB（Redis Database）
混合持久化

### RDB
dump.rdb，快照形式，默认配置。
命令触发或根据配置触发。

需要持久化时，fork子进程将数据写到磁盘上的临时rdb文件，写完后替换原来的rdb，实现copy-on-write。
与内存数据存在一些差距，按配置的时间或更新量。
可以用于做定期备份，适用于灾备恢复，可能丢失部分数据。

### AOF
appendonly.aof，类似mysql binlog，默认重启后优先读取数据
+ always，每次写后立即同步
+ everysec，每秒同步一次
+ no，有操作系统决定同步时机？

默认每秒同步一次，异常时该一秒的数据会丢失。

需要持久化时，调用write函数追加到aof文件。

数据安全性更高，可只丢失最后一次的写操作；
文件比RDB文件大，恢复慢；写入性能稍差；

### 混合持久化
AOF文件中保存一个RDB、追加后续的写操作。
平衡了文件大小和恢复操作。


### 淘汰策略
可配置6种淘汰策略

## 单线程服务
基于Event-Loop模式处理client请求，和Nodejs很像。
不必考虑线程安全，简化开发，提高性能。
减少线程切换的损耗。

单点内存和吞吐量有限
可用性差，挂掉后，不可服务
高负载下，数据查询慢

## 集群服务
### redis slot
负载均衡（一致性hash）

### 主从复制redis replication
Master/slave chains架构

全量复制/部分复制/偏移量/

### Redis Partition

### Presharding

## 数据结构
string/list/set/sortedset/map


## 渐进式rehash

## 单点性能/集群性能
变量：机器性能/不同操作系统/value长度/redis不同配置

## FAQ
1. **Redis事务？**  
Redis Cluster集群中，key分配在不同的Redis节点上的，Redis的事务机制会带来集群的不可用（性能很差）。
Redis事务不支持回滚操作。
一般不用Redis的集群。

2. **Redis的多数据库机制**  
Redis支持多个数据库，并且每个数据库的数据是隔离的。
单点redis可以支持16个数据库（db0 ~ db15）。
Redis Cluster集群中一般只使用一个数据库空间，即默认db0。

3. **Redis集群不足**  
批量操作直接提交给一个单点？
一个key（即使有多个value或subkey-value）只能存到一个节点上，不能存到多个节点上。

4. **Redis的批量操作**  
mset、mget操作
Redis Cluster集群，key划分到不同的slot中，直接使用mset或者mget等操作不行

+ 串行get操作
+ Hashtag使key映射到同一redis节点上  
对于key为{foo}.value1、{foo}.value2，{foo}value3。只有key中{}中的部分（即hash tags）才被用来做hash，计算出来的redis节点是同一个。
+ Proxy分片集群架构，如Codis，会将mget/mset的多个key拆分成多个命令发往不同的redis实例。
+ 放到同一个hashmap里，就会在同一个redis节点？

5. **Redis读写分离**
分片集群本身吞吐量就高，没有必要。
redis操作都是在内存上，较少涉及IO吞吐，读写分离不会提升太多性能。
做了读写分离后，还要考虑主从一致性，主从延迟等问题，带来业务复杂度。

Redis的主要问题是考虑容量，单机最多10-20G，key太多降低redis性能。因此采用分片集群来保证性能。

6. **redis-cluster 为什么是16384个slot**
函数crc16()有2^16 - 1=65535个不同的值。2^14=16384
+ redis间心跳包通讯需要带着节点的所有配置（通知到其他节点），其中包含该节点的slots配置。这就意味着：如果是16384个slot，大约需要2kb（压缩前）；如果是65535个slot，大约需要8kb。
+ redis-cluster规模很少达到1000个以上的master节点（因为其他的设计上的均衡）

所以16384个slot足够保证1000个master节点的集群中每个maser节点的slot数量，又可以把slot的信息（位图形式）便捷地广播出去。
注意：小型集群中，因为集群中master节点数N比较小，位图中slots/N个位被设置，因而很难压缩。

7. **memcache vs redis**  
memcache没有持久化，数据不能超出内存大小，数据结构简单。

