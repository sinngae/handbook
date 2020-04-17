# redis源码阅读

## 数据结构
应用层5种数据结构
+ string
+ hash
+ list
+ set
+ zset，sorted set

7种编码方式
+ raw
+ int
+ zipmap
+ linkedlist
+ ziplist
+ intset

redisObject用于表示key和value，实际包括数据类型type/编码方式encoding/数据指针ptr/虚拟内存vm/等等

redis的key是string数据类型。key->string/key->hmap/key->list/key->set/key->zset

string类型可以存储任何二进制数据，包括图片、序列化的对象等，最大512M。
hash类型，key->value映射表，hget/hset/hgetall。
list类型，按插入顺序排序，lpush/rpush/lpop/rpop/lrange。
list实现是双向链表。
set类型是value的无序集合，实现是hashtable，sdd/spop/smembers/sunion等，去重，可用于判断元素是否可存在。
zset类型也是value的集合，去重，有一个优先级参数为zset排序，zadd/zrange/zrem/zcard，实现是hashmap和跳跃表。

## 源码划分
Redis源码的模块划分，可分为数据结构、内存编码、数据类型
1. 阅读Redis的数据结构部分，位于如下文件：
+ 内存分配 zmalloc.c和zmalloc.h
+ 动态字符串 sds.h和sds.c
+ 双端链表 adlist.c和adlist.h
+ 字典 dict.h和dict.c
+ 跳跃表 server.h文件里面关于zskiplist结构和zskiplistNode结构，以及t_zset.c中所有zsl开头的函数，比如 zslCreate、zslInsert、zslDeleteNode等等。
+ 日志类型 hyperloglog.c 中的 hllhdr 结构， 以及所有以 hll 开头的函数

2. 熟悉Redis的内存编码结构
+ 整数集合数据结构 intset.h和intset.c
+ 压缩列表数据结构 ziplist.h和ziplist.c

3. 熟悉Redis数据类型的实现
+ 对象系统 object.c
+ 字符串键 t_string.c
+ 列表建 t_list.c
+ 散列键 t_hash.c
+ 集合键 t_set.c
+ 有序集合键 t_zset.c中除 zsl 开头的函数之外的所有函数
+ HyperLogLog键 hyperloglog.c中所有以pf开头的函数

4. 熟悉Redis数据库的实现
+ 数据库实现 redis.h文件中的redisDb结构，以及db.c文件
+ 通知功能 notify.c
+ RDB持久化 rdb.c
+ AOF持久化 aof.c

以及一些独立功能模块的实现
+ 发布和订阅 redis.h文件的pubsubPattern结构，以及pubsub.c文件
+ 事务 redis.h文件的multiState结构以及multiCmd结构，multi.c文件

5. 悉客户端和服务器端的代码实现
+ 事件处理模块 ae.c/ae_epoll.c/ae_evport.c/ae_kqueue.c/ae_select.c
+ 网路链接库 anet.c和networking.c
+ 服务器端 redis.c
+ 客户端 redis-cli.c

这个时候可以阅读下面的独立功能模块的代码实现
+ lua脚本 scripting.c
+ 慢查询 slowlog.c
+ 监视 monitor.c

6. 熟悉Redis多机部分的代码实现
+ 复制功能 replication.c
+ Redis Sentinel sentinel.c
+ 集群 cluster.c
+ 关于测试方面的文件有：
    + memtest.c         内存检测
    + redis_benchmark.c 用于redis性能测试的实现。
    + redis_check_aof.c 用于更新日志检查的实现。
    + redis_check_dump.c 用于本地数据库检查的实现。
    + testhelp.c        一个C风格的小型测试框架。

+ 一些工具类的文件如下：
    + bitops.c      GETBIT、SETBIT 等二进制位操作命令的实现
    + debug.c       用于调试时使用
    + endianconv.c  高低位转换，不同系统，高低位顺序不同
    + help.h        辅助于命令的提示信息
    + lzf_c.c       压缩算法系列
    + lzf_d.c       压缩算法系列
    + rand.c        用于产生随机数
    + release.c     用于发布时使用
    + sha1.c        sha加密算法的实现
    + util.c        通用工具方法
    + crc64.c       循环冗余校验
    + sort.c        SORT命令的实现

+ 一些封装类的代码实现：
    + bio.c         background I/O的意思，开启后台线程用的
    + latency.c     延迟类
    + migrate.c     命令迁移类，包括命令的还原迁移等
    + pqsort.c      排序算法类
    + rio.c         redis定义的一个I/O类
    + syncio.c      用于同步Socket和文件I/O操作

整个Redis的源码分类大体上如上所述。


## 数据结构

### skiplist跳跃表
相比于B数、BR树，提供较简单的实现和相同的搜索速度，且更少的锁，更少的rebalance开支。

### dict

## faq
**hyperlog log基数评估算法**  
根据局部第一次出现的概率和出现的次数，估算整体的数量；根据多桶数据估算结果的平均，消除标准差，得到近似结果。
