Data Sync
---

## mysql主从同步机制：

+ master服务器将数据的改变记录二进制binlog日志，当master上的数据发生改变时，则将其改变写入二进制日志中；
+ slave服务器会在一定时间间隔内对master二进制日志进行探测其是否发生改变，如果发生改变，则开始一个I/O Thread请求master二进制事件，并写入本地relay-log(中继日志)文件中；
+ 此时主节点为每个I/O线程启动一个dump线程，用于向其发送二进制事件；
+ 从节点启动SQL线程从中继日志中读取二进制日志，在本地重放，使得其数据和主节点的保持一致，最后I/O Thread和SQL Thread将进入睡眠状态，等待下一次被唤醒。


# 主从同步的三种模式：
+ 基于SQL语句的复制(statement-based replication, SBR)，对应的binlog格式为STATEMENT
    + 每一条修改数据的sql语句会记录到binlog中。
    + 优点：是并不需要记录每一条 sql语句和每一行的数据变化，减少了binlog日志量，节约IO，提高性能。这也意味着可以更快地完成从备份中获取和恢复的工作。
    + 缺点：是在某些情况下会导致 master-slave中的数据不一致(如sleep()函数， last_insert_id()，以及user-defined functions(udf)等会出现问题)
+ 基于行的复制(row-based replication, RBR)，对应的binlog格式为ROW
    + 仅记录修改的内容，不记录具体的SQL。
    + 优点：不记录每条sql语句的上下文信息，仅需记录哪条数据被修改了，修改成什么样了。而且不会出现某些特定情况下的存储过程、或function、或trigger的调用和触发无法被正确复制的问题。
    + 缺点：是会产生大量的日志，尤其是altertable的时候会让日志暴涨。
+ 混合模式复制(mixed-based replication, MBR)，对应的binlog格式为MIXED
    + 以上两种模式的混合使用，一般的复制使用STATEMENT模式保存binlog，对于STATEMENT模式无法复制的操作使用ROW模式保存binlog，MySQL会根据执行的SQL语句选择日志保存方式。