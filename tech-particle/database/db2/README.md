# DB2
IBM DB2

## 常用命令
```sh
db2 connect to [db name] user [user name] using [passwd]

# 搜索库/表
db2 "list active databases"
db2 "select name,type,creator from sysibm.systables where type='T' and name like 'TEST%'"

# 查看表定义
db2 "describe table [db name].[tbl name]"
db2 "describe indexes for table [db name].[tbl name]"
db2look -d [db name] -z db2inst1 -t "[tbl name]" -a -e -c

# 查询表数据
db2 "select * from [db name].[tbl name]"

# 查询分页
db2 "select * from [tbl name] fetch first [nums] rows only"
db2 "SELECT * FROM (SELECT rownumber() OVER (ORDER BY ID) AS ROW_NEXT,tbl0.* FROM tabel0 as tbl0) AS tbl2 WHERE ROW_NEXT BETWEEN 21 and 40"

# 查询优化
db2expln -d [db name] -statement "select * from [db name].[tbl name] where id=100" -terminal

# 事务
db2 +c "update [db name].[tbl name] set type='2' where id=100"
db2 +c "[sql2]"
db2 "commit"

# db2look
#   输出DB的DDL到out.sql
db2look -d [db name] -u [user] -e -o [out.sql]
#   模式名
db2look -d [db name] -z [scheme name] -e -o [out.sql]
#   输出用户创建表的统计信息
db2look -d [db name] -u [user] -m -o [out.sql]
#   输出用户创建表的DDL语句，适用于W1的联合对象的DDL
db2look -d [db name] -u [user] -e -wrapper W1 -o [out.sql]
#   输出用户创建表的DDL语句，适用于S1的联合对象DDL
db2look -d [db name] -u [user] -e -server S1 -o [out.sql]
#   输出表的DDL
db2look -d [db name] -z [scheme name] -t "T1" -a -e -c

# 中文乱码？测试没有这个命令，可能是环境的问题
db2set db2codepage=1252
```

```sh
# db2命令查看
db2 list command options
#   db2 [option ...] [db2-command | sql-statement | 
#       [? [phrase | message | sqlstate | class-code]]]
#       option：-a、-c、-e{c|s}、-finfile、-lhistfile、-n、-o、-p、-rreport、-s、-t、
#       -td;、-v、-w、-x 和 -zoutputfile。
#   -a 显示 SQLCA                            OFF
#   -c 自动落实                              ON
#   -e 显示 SQLCODE/SQLSTATE                 OFF
#   -f 读取输入文件                          OFF
#   -l 将命令记录到历史文件中                 OFF
#   -n 除去换行字符                          OFF
#   -o 显示输出                              ON
#   -p 显示 db2 交互式提示符                  ON
#   -r 将输出报告保存到文件                   OFF
#   -s 在命令出错时停止执行                   OFF
#   -t 设置语句终止字符                       OFF
#   -v 回送当前命令                           OFF
#   -w 显示 FETCH/SELECT 警告消息             ON
#   -x 不打印列标题                           OFF
#   -z 将所有输出保存到输出文件                OFF

# 执行sql脚本
db2 -tvf [sql filename]
db2 -td@ -f [sql filename]  # 以@做结束符
db2 -svtf [sql filename]    # 常用

# explain
db2expln -database ttdb -statement "SELECT col1,col2 FROM ttdb.demo WHERE id=1" -terminal
```

## 隔离级别
1. 未落实的读（UR）  
未落实的读亦称“脏读”，是最低的隔离级别，并且提供最高的并行性。
除非另一个连接企图删除（drop）或者更改（alter）整个表，否则读操作时没有行锁定。而修改（update）操作与游标稳定性级别相同。
此隔离级别仍存在的问题：未落实的读，不可重复读，幻象。
此隔离级别所防止的问题：丢失更新。
2. 游标稳定性（CS）  
游标稳定是默认的隔离级别，提供了低程度的锁定。
在这一隔离级别中，游标的“当前”行是锁定的。如果该行只是被读，锁定会一直持续到一个新行被访问或者该工作单元终止。如果该行被修改，锁定会一直持续到该工作单元终止。
此隔离级别仍存在的问题：不可重复读，幻象。
此隔离级别所解决的问题：丢失更新，未落实的读。
3. 读稳定性（RS）  
使用读稳定性，在（一个连接的）同一个工作单元中的所检索的全部行都会被锁定。
对于一个给定的游标，它要锁定所有与结果集匹配的行。例如，如果你有一个含1000行的表并且查询返回10行，那么只有那10行会被锁定。读稳定性使用中等级别的锁定。
此隔离级别仍存在的问题：幻象。
此隔离级别所解决的问题：丢失更新，未落实的读，不可重复的读。
4. 可重新性（RR）  
可重复读是最高的隔离级别，提供了最大程度的锁定和最少的并行。
产生结果集的所有行都会被锁定，即：即使不必出现在最终结果集中的行也会被锁定。在此该工作单元结束前，任何其它连接都不能修改、删除或插入一个会影响结果集的行。重复读确保一个连接在一个工作单元中多次进行的同一项查询都返回结果。

```sh
# 会话或连接的隔离级别

# 单个sql的隔离级别通过 WITH {隔离级别} 子句设定
db2 "SELECT * FROM table WHERE IDX=idx0 WITH {UR|CS|RS|RR}"

# 修改隔离级别
db2 connect reset
db2 change isolation to {RR|RS|CS|UR} # 指定查询所用的隔离级别，
```