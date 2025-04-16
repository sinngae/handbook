SQL
---


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