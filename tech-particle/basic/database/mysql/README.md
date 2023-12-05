# mysql

## 存储引擎
mysql存储引擎有多种，常见的有MyISAM和InnoDB
+ 1.InnoDB支持事务、外键，MyISAM不支持
+ 2.InnoDB是mysql5.5后默认的
+ 3.MyISAM适合查询以及插入为主的应用；InnoDB适合频繁修改以及涉及到安全性较高的应用
+ 4.InnoDB不支持FULLTEXT类型索引，不保存表的行数
+ 5.自增长字段，InnoDB必须包含只有该字段的索引，但是MyISAM表中可以和其他字段一起建立联合索引
+ 6.清空整张表时，InnoDB是一行行删除，很慢；MyISAM则是重建表
+ 7.InnoDB支持行锁，对于like之类的sql还是锁整张表

MyISAM适用于历史库/数据仓库；InnoDB适用于当前库/业务数据库；

## mysql 命令行
```sql
-- 三种注释
'''dfadf'''
-- ...
/*...*/
-- 登录shell
-- mysql -h 127.0.0.1 -u root -p testdb;

-- 事务
start transaction;
set session transaction isolation level read committed;
set @@session.autocommit=0;
select * from stu where stu_no=1000002 for update;
commit;
/*
1、不管autocommit 是1还是0，START TRANSACTION后只有当commit数据才会生效，ROLLBACK后就会回滚。
2、当autocommit 为 0 时，不管有没有START TRANSACTION，只有当commit数据才会生效，ROLLBACK后就会回滚。
3.如果autocommit 为1 ，并且没有START TRANSACTION，调用ROLLBACK是没有用的。即便设置了SAVEPOINT。
*/

-- 查看进程访问信息
show processlist;

-- 执行某处sql
source /opt/openfire/resources/database/openfire_mysql.sql;
```

## mysql配置文件my.ini
```sh
## 编辑C:\ProgramData\MySQL\MySQL Server 5.7\my.ini
#   @[name]			用户变量
#   @@[name]			系统变量

## mysql 5.6 添加追踪日志
vi /etc/my.cnf
## 添加以下两行
#   general_log=ON
#   general_log_file=runtime.log

## 重启mysqld服务
service mysqld restart

## 在以下路径，可看到runtime.log日志文件
#   /var/lib/mysqlmysql二进制文件路径
sc delete MySQL57
```

## 配置外网访问
```sql
CREATE USER 'monty'@'localhost' IDENTIFIED BY 'some_pass';
GRANT ALL PRIVILEGES ON *.* TO 'monty'@'localhost' WITH GRANT OPTION;
CREATE USER 'monty'@'%' IDENTIFIED BY 'some_pass';
GRANT ALL PRIVILEGES ON *.* TO 'monty'@'%' WITH GRANT OPTION;
flush privileges;
```

## 特殊sql
```sql
-- 非插即更
insert into tbl0 values(x, y)
on duplicate key 
update set x=x;

-- 交换列值
update tbl0, tbl1
set tbl0.x = tbl1.x, tbl1.x = tbl0.x
where tbl0.y=y0 and tbl1.y=y1;

-- 重置自增ID
alter table tablename auto_increment=[new start];
```

## 库表统计数据
```sql
SELECT
    TABLE_SCHEMA AS "数据库",
    sum( table_rows ) AS "记录数",
    concat( TRUNCATE ( sum( data_length ) / 1024 / 1024, 2 ), ' MB' ) / 1024 AS "数据容量（GB）",
    concat( TRUNCATE ( sum( index_length ) / 1024 / 1024, 2 ), 'MB' ) / 1024 AS "索引容量（GB）" 
FROM
    information_schema.TABLES 
WHERE
    table_schema = 'console';
    
   
SELECT
    table_schema AS "数据库",
    table_name AS "表名",
    table_rows AS "记录数",
    TRUNCATE ( data_length / 1024 / 1024, 2 ) / 1024 AS "数据容量（GB）",
    TRUNCATE ( index_length / 1024 / 1024, 2 ) / 1024 AS "索引容量（GB）" 
FROM
    information_schema.TABLES 
WHERE
    table_schema = 'console' 
order by 
	TABLE_ROWS desc;

ORDER BY
    data_length DESC,
    index_length DESC;
```