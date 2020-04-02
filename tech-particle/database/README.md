# 数据库
ACID/隔离级别/索引数据结构/查询优化/主从备份/分布式集群

## ACID 事务的特性
+ 原子性 atomicity  
要么完成，要么全部回滚
+ 一致性 consistency  
实际并发的结果与理论串行结果一致  
事务执行前后，处于一致状态？
+ 隔离性 isolation  
同一时间仅有一个事务处理同一数据，处理这一数据的其他事务串行化处理，失败的事务不影响数据  
多个事务并发执行，相互独立。
+ 持久性 durability  
已提交的事务永久地更改数据保存在数据库中，不会回滚

ACID是传统数据库设计概念，追求强一致性，带有数学严谨性。
工程思维上更多使用支持BASE的大型分布式系统，牺牲强一致性，获取高可用性。

**怎么理解隔离性**

实现的方式有两种，write ahead logging 和 shadow paging  
后者简单开销大，一个事务修改的块都提交到磁盘，很难并发  
前者对数据的修改必须记录到日志中，日志被冲刷到永久存储器中。不需要冲刷数据页。出现崩溃的时候，从日志恢复数据库。  


## SQL标准中的四个隔离级别
隔离级别即数据库处理并发的锁定策略。

**数据库可以设置默认的隔离级别，一个客户端的连接中也可以指定隔离级别，或者具体的sql语句指定锁行为。**

**常见问题**
+ 1.脏读  
在一个事务中读取了另一个事务未提交的数据
+ 2.不可重复读  
一个事务多次查询数据库，却因另一事务的提交查询结果不同
+ 3.幻读（查询了未提交插入的表数据）  
一个事务多次统计或更新多个数据，因另一事务插入提交数据，导致返回结果不同


隔离级别由低到高：
+ Read-uncommited  
读未提交，无保证
+ Read-commited  
读已提交，可避免1
+ Repeatable-read  
可重复读，可避免1、2
+ serializable  
串行化，可避免1、2、3发生

隔离级别越高，并发越低，

mysql默认repeatable-read，支持四种；
oracle只支持serializable和read-commited，默认为后者；
其他大多read-commited  

```sql
-- mysql 查看事务隔离级别
select @@tx_isolation;
-- 设置
set [global|session] transaction isolation level '[isolate level name]';
set tx_isolation='[...]'
```
