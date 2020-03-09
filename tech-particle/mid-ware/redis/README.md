# Redis
an open source(BSD licensed), in-memory data structure store, used as a database, cache and message broker.

BSD协议开源的，in-memory数据结构存储的，作为数据库、缓存和消息的中介代理。

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
负载均衡（一致性hash）

主从复制redis replication
Master/slave chains架构

Redis Partition

Presharding

## 数据结构
string/list/set/sortedset/map

## 持久化