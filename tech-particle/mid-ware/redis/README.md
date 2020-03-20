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

### Redis Partition

### Presharding

## 数据结构
string/list/set/sortedset/map

## 持久化

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

4.  **Redis的批量操作**  
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