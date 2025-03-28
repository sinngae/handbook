数据库
---
组成：表结构/数据类型/约束/索引/外键/视图/存储过程/触发器/函数/用户权限及授权

理论：ACID/隔离级别/SQL(DML/DDL/分组查询/...)/锁机制(悲观锁/乐观锁)/NF范式/ER图

实战：查询优化/存储机制/主从备份/分布式集群



数据处理大致可分为两类：
OLAP，Online Analytical Processing，在线分析处理，比如数据仓库的场景。
OLTL，Online Transaction Processing，在线事务处理，比如银行交易数据增删查改的场景。

非结构化数据存储与分析，如nosql、Hadoop等

-|需求比较|OLTP|OLAP
-|-|-|-
功能性需求|索引类型|B+索引|Bitmap、BitmapJoin索引
..|事务|一般要求支持事务？<br>非事务无锁OLTP？|无事务处理？
..|SQL|
非功能性|高并发|大量并发，常规业务|并发需求不高，批量查询、生成报表
..|高性能|要求响应非常快，一般优先级高|要求速度快
..|吞吐量|？|大
..|SQL操作|大多为插入、修改操作(DML)|大多为大量查询操作或批量DML操作

# 事务的特性 ACID
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
+ 实现的方式有两种，write ahead logging 和 shadow paging。
+ 后者简单开销大，一个事务修改的块都提交到磁盘，很难并发。
+ 前者对数据的修改必须记录到日志中，日志被冲刷到永久存储器中。不需要冲刷数据页。出现崩溃的时候，从日志恢复数据库。  


## SQL标准中的四个隔离级别
隔离级别即数据库处理并发的锁定策略。

**数据库可以设置默认的隔离级别，一个客户端的连接中也可以指定隔离级别，或者具体的sql语句指定锁行为。**

**数据并发问题**
+ 1.脏读  
在一个事务中读取了另一个事务未提交的数据
+ 2.不可重复读  
一个事务多次查询数据库，却因另一事务的(更新?)提交，造成查询结果不同
+ 3.幻读（查询了未提交插入的表数据）  
一个事务多次统计或查询的结果集，因另一事务的(插入/删除?)提交，导致查询结果不同


隔离级别由低到高：
+ Read-uncommited  - 读未提交，无保证
+ Read-commited  - 读已提交，可避免1
+ Repeatable-read  - 可重复读，可避免1、2
+ serializable  - 串行化，可避免1、2、3发生

隔离级别越高，并发越低，

mysql默认repeatable-read，支持四种；
oracle只支持serializable和read-commited，默认为后者；
DB2特有的的四种隔离级别；
其他大多read-commited  

```sql
-- mysql 查看事务隔离级别
select @@tx_isolation;
-- 设置
set [global|session] transaction isolation level '[isolate level name]';
set tx_isolation='[...]'
```

# 索引
主键/唯一索引/非唯一索引/聚集索引/外键索引

按索引查询/全表扫描

物理存储 + 约束 + 索引算法：表的顺序存储/索引结构(B树/B+树/B*树/hashtable/bitmap) + 唯一约束/非空约束 + 二进制对比算法(基于各种字符集；位运算？)

索引是为了加快数据检索。

每个表只能有一个聚集索引(clustered index，聚簇索引)，用于指定表中数据存储在硬盘上的存储顺序。聚集索引的叶子节点就是数据节点，而非聚集索引的叶子节点仍然时索引节点，指向对应的数据块。可以非唯一，也可以为空。

## 聚集索引
主键是表的特殊的非空唯一索引，如果没有指定聚集索引，主键就是聚集索引。

聚集索引一般是表中的主键索引（默认是查询顺序），如果表中没有显示指定主键，则会选择表中的第一个不允许为NULL的唯一索引，如果还是没有的话，就采用Innodb存储引擎为每行数据内置的6字节ROWID作为聚集索引。

每张表只有一个聚集索引，因为聚集索引在精确查找和范围查找方面良好的性能表现（相比于普通索引和全表扫描），聚集索引就显得弥足珍贵，聚集索引选择还是要慎重的（一般不会让没有语义的自增id充当聚集索引）。但是一般，自增主键能极大提升写入效率。

## **思考：热点数据聚集**
聚集索引本质是数据库提供的数据存储的控制，那么是否可以按此做热点数据聚集呢。
redis的LRU策略只发生在内存不足时？
比如，把当天登录的热点用户聚集起来，把当前小时的热点用户聚集起来，把当前一刻钟的热点用户聚集起来。（可以把热点用户替换成热点文章数据，热点视频等）
聚集操作应该放在业务事务之外，避免拖慢业务事务的速度。
应当避免过于频繁地更新聚集字段。

+ 要求速度很快的查询需求应该使用聚集索引。
  + 聚集索引的列成员如果被update会引起磁盘上的数据位置变更。所以，聚集索引应该
  + 适用于大量非重复的列？如果是历史表，相比UUID应该使用日期做聚集索引
+ 其他查询需求，应该做索引覆盖。
+ 一个表的索引不宜过多，过多会使插入和删除变慢(也有可能导致update变慢)，并且占用内存等。
  + 远超存量的大量数据插入，可以先删除索引，插入数据后再重建索引。
+ 未覆盖到的列的查询，或者非二进制对比的查询将导致全表查询。
+ 索引查询的效率取决于索引的层数(对于B树系列索引)

## 自增ID、UUID、自定义有序ID、雪花作为主键的优劣
1. 自增ID作为主键
+ 行数据随主键的顺序填写磁盘，提升了磁盘利用率
+ 索引更紧凑，查询数据更快
+ 连续增长，避免B+树频繁合并和分裂
+ 简单易读

+ 可靠性问题，自增ID回溯问题（极大值溢出）
+ 安全性差，对外暴露信息（比如每日新增用户数、新增订单量）
+ 性能差，在数据库服务中 插入争抢自增的锁；新自增的ID，需要二次查询才能获取
+ 难以在分布式数据库存储上适用，仅局部唯一
+ 不利于数据迁移和扩展

分库分表、数据迁移、新老数据合并，这些业务都不适用自增ID

2. UUID作为主键
+ 保证全局唯一
+ 更加安全？

+ UUID包含MAC地址（真的能泄露么？）
+ 无序，随机生成 插入导致聚集索引频繁分裂、随机IO、内存碎片化，最终插入性能越来越差
+ 占用36字节，比较占用空间


## 查询情况
1. 主键查询

2. 回表查询
先（通过非聚集索引）查询行数据所在的主键值，再根据主键值（聚集索引）访问行数据

3. 覆盖索引
a. 索引覆盖了查询（where部分的字段包括了select的字段）
b. 联合索引（where部分的字段组成一个索引；explain sql extra字段中有 using index）
这样不经过两次B+树访问，即可得到想要的结果。

4. 级联查询

n. 复合查询

+ 最左前缀原则
  + where语句的多个搜索条件调换顺序不影响查询结果，因为mysql查询优化器会自动优化顺序
  + where语句，若遇到范围查找（<> between like）或 不在索引中的字段，则停止匹配（范围查询会使用到索引）

## 索引失效
1. 查询条件中有OR，即使符合索引命中也会失效
2. like查询以%开头
3. 索引列需要计算
4. 违背最左匹配原则？
5. MySQL优化器认为全表扫描更快
6. B+树 is null不会走索引，is not null会走索引
7. 


## 索引下推
index condition pushdown，一种针对联合索引查询的优化技术，通过在存储引擎层提前过滤数据，减少回表次数，从而提升查询性能。
适用场景，查询条件包含索引的前缀列和其他索引列，但非前缀列无法直接使用索引匹配。
1. 特定方法，range范围查找、ref 通过索引查找单行或多行、eq_ref等值引用、ref_or_null
2. 非聚簇索引
3. 复杂查询条件
4. 没有适用子查询、存储函数、OR等

# 其他


## 范式 NF，Normal Form
1. 第一范式（1NF）：数据库表的每一项都是不可分割的原子数据项，不能是集合。
2. 第二范式（2NF）：在第一范式的基础上，所有属性完全依赖于主键，完全依赖就是不能取决于主键的一部分。
3. 第三范式（3NF）：在第二范式的基础上，消除传递依赖。
4. 巴斯-科德范式(BCNF，Boyce-Codd NF)，修正的3NF。在3NF基础上，任何非主属性不能对主键子集依赖（即在3NF基础上，消除主属性对候选码的部分函数依赖和传递函数依赖）。
5. 4NF：在BCNF基础上，消除非平凡且非函数依赖的多值依赖（即把同一表内的多对多关系删除）。
6. 5NF：在4NF基础上，消除不是由候选码所蕴含的连接依赖。

范式越高，冗余越少，查询越复杂；反之，查询越简单。关系型数据库设计时，需要在冗余、查询性能、逻辑复杂度之间做平衡。

## E-R图
实体关系图：1对多，多对多/派生

面向异构的分库分表的E-R图设计
真实工程，没有理想的简单关系，像黑板上画的简单E-R图一样。

## SQL
数据定义语言DDL/数据操纵语言DML/完整性/视图定义/事务控制/嵌入式SQL/动态SQL/授权
### 数据定义 DDL
### 数据操纵 DML
#### 连接运算
自然连接/左连接/右连接/全连接/外连接/内连接
#### 聚集函数
avg/min/max/sum/count

#### 查询优化
查询优化器，sql explan

## 数据空洞
执行大量删除操作后，有些数据库（MYSQL-MyISAM等）不会立马释放磁盘文件，而是等待插入操作插入数据到被删除的位置，这个场景下产生的大量无数据占用磁盘空间的情况叫做数据空洞。好处是可以实现磁盘空间的重复利用，坏处是带来空间的浪费。MYSQL提供OPTIMIZE TABLE xxx 来处理数据空洞

# 锁
悲观锁  
当要对数据库中的一条数据进行修改的时候，为了避免同时被其他人修改，最好的办法就是直接对该数据进行加锁以防止并发。这种借助数据库锁机制，在修改数据之前先锁定，再修改的方式被称之为悲观并发控制【Pessimistic Concurrency Control，缩写“PCC”，又名“悲观锁”】。

乐观锁  
乐观锁是相对悲观锁而言的，乐观锁假设数据一般情况不会造成冲突，所以在数据进行提交更新的时候，才会正式对数据的冲突与否进行检测，如果冲突，则返回给用户异常信息，让用户决定如何去做。乐观锁适用于读多写少的场景，这样可以提高程序的吞吐量

# 底层原理
## 为什么DB使用B+树
B树在提高了IO性能的同时并没有解决元素遍历的底下的问题，正是为了解决这个问题，B+树应用而生。B+树只需要去遍历叶子结点就可以实现遍历整棵树的。而在数据库中基于范围的查找是非常频繁的，而B树不支持这样的操作或者说效率比较低下。

1、 B+树的磁盘读写代价更低：B+树的内部节点并没有指向关键字具体信息的指针，因此其内部节点相对B树更小，如果把所有同一内部节点的关键字存放在同一盘块中，那么盘块所能容纳的关键字数量也越多，一次性读入内存的需要查找的关键字也就越多，相对IO读写次数就降低了。

2、B+树的查询效率更加稳定：由于非终结点并不是最终指向文件内容的结点，而只是叶子结点中关键字的索引。所以任何关键字的查找必须走一条从根结点到叶子结点的路。所有关键字查询的路径长度相同，导致每一个数据的查询效率相当。

3、由于B+树的数据都存储在叶子结点中，分支结点均为索引，方便扫库，只需要扫一遍叶子结点即可，但是B树因为其分支结点同样存储着数据，我们要找到具体的数据，需要进行一次中序遍历按序来扫，所以B+树更加适合在区间查询的情况，所以通常B+树用于数据库索引