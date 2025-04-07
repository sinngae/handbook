# mysql

## 引擎
目前广泛使用的是MyISAM和InnoDB两种引擎：

### MyISAM
MyISAM引擎是MySQL 5.1及之前版本的默认引擎，它的特点是：
+ 不支持行锁，读取时对需要读到的所有表加锁，写入时则对表加排它锁
+ 不支持事务
+ 不支持外键
+ 不支持崩溃后的安全恢复
+ 在表有读取查询的同时，支持往表中插入新纪录
+ 支持BLOB和TEXT的前500个字符索引，支持全文索引
+ 支持延迟更新索引，极大提升写入性能
+ 对于不会进行修改的表，支持压缩表，极大减少磁盘空间占用

### InnoDB
InnoDB在MySQL 5.5后成为默认索引，它的特点是：
+ 支持行锁，采用MVCC来支持高并发
+ 支持事务
+ 支持外键
+ 支持崩溃后的安全恢复
+ 不支持全文索引

总体来讲，MyISAM适合SELECT密集型的表，而InnoDB适合INSERT和UPDATE密集型的表


### 存储引擎
mysql存储引擎有多种，常见的有MyISAM和InnoDB
+ 1.InnoDB支持事务、外键，MyISAM不支持
+ 2.InnoDB是mysql5.5后默认的
+ 3.MyISAM适合查询以及插入为主的应用；InnoDB适合频繁修改以及涉及到安全性较高的应用
+ 4.InnoDB不支持FULLTEXT类型索引，不保存表的行数
+ 5.自增长字段，InnoDB必须包含只有该字段的索引，但是MyISAM表中可以和其他字段一起建立联合索引
+ 6.清空整张表时，InnoDB是一行行删除，很慢；MyISAM则是重建表
+ 7.InnoDB支持行锁，对于like之类的sql还是锁整张表

MyISAM适用于历史库/数据仓库；InnoDB适用于当前库/业务数据库；


## explain SQL分析和优化

分析出
+ 查询的目标表是如何访问的，全表扫描、索引扫描
+ 表之间的联系顺序
+ 使用了哪些索引
+ 查询的估算成本

结果（输出了一个表格）观察
+ id序号，如果有子查询、联合查询等会有多个序列号
+ select_type，查询类型
    + simple，简单查询（非复合、非联合的查询）
    + primary，最外层查询
    + subquery，子查询的第一个select
    + derived，派生表（from子句的子查询）
+ table，目标表
+ partitions，如果使用了分区表，会显式分配的分区
+ type，访问类型，也即访问表的方式，从最好到最差依次是：
    + system，表中只有一行数据（等于系统表）
    + const，通过主键 或 唯一索引直接访问表中一条数据
    + eq_ref，对于前一个表的一行数据，从后一个表读取一行数据（常见于使用主键或唯一索引的等值连接中）
    + ref，对于前一个表的一行数据，从后一个表读取匹配的所有行（非唯一索引的等值连接）
    + range，只检索给定范围内的行（where子句范围条件，大于、小于、between）
    + index，全索引扫描，扫描整个索引树
    + ALL，全表扫描（扫描整个表）
+ possible_keys，列出可能使用的索引
+ key，实际使用的索引（NULL，没有使用索引）
+ key_len，实际使用的索引长度（越短越好，表示了索引的精确度？）
+ ref，被使用的索引的列（const、func表示使用了常量或函数）
+ rows，MySQL计算得必须要检查得行数（越小表示查询效率越高）
+ filtered，表示通过条件过滤的行的（占所有扫描行）比例，越高越好
+ extra，额外信息
    + using index，只是用了索引，没有访问表数据
    + using where，使用了where子句过滤
    + using temporary，使用了临时表来存储中间结果
    + using filesort，需要文件排序来满足orderby 或 group by
        + 将结果写入磁盘文件，再执行quicksort排序
        + 分块排序，再merge sort合并
        + 可以处理大量数据，性能开销也大

优化方向：
+ type列，尽量避免All 和 index，优先使用eq_ref、ref、range
+ key列，确保使用了合适的索引，考虑添加合适的索引
+ rows 尽量少，extra 避免临时表和磁盘文件排序

