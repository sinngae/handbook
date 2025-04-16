explain SQL分析和优化
---

# 分析出
+ 查询的目标表是如何访问的，全表扫描、索引扫描
+ 表之间的联系顺序
+ 使用了哪些索引
+ 查询的估算成本

# 结果（输出了一个表格）观察
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

# 优化方向：
+ type列，尽量避免All 和 index，优先使用eq_ref、ref、range
+ key列，确保使用了合适的索引，考虑添加合适的索引
+ rows 尽量少，extra 避免临时表和磁盘文件排序，但是实际轻卡
+ 