* filename :  
* author:  renzq
* timestamp  :  2016-06-01  13:56:58
* desctiption :   
This file is wrriten for ...登录命令行
mysql -h 127.0.0.1 -u root -p testdb
root1234

事务
start transaction;
set session transaction isolation level read committed;
set @@session.autocommit=0;
select * from stu where stu_no=1000002 for update;
commit;

查看进程访问信息
show processlist;

/*
   1、不管autocommit 是1还是0 
   START TRANSACTION 。后只有当commit数据才会生效，ROLLBACK后就会回滚。

   2、当autocommit 为 0 时
   不管有没有START TRANSACTION。
   只有当commit数据才会生效，ROLLBACK后就会回滚。

   3.如果autocommit 为1 ，并且没有START TRANSACTION 。
   调用ROLLBACK是没有用的。即便设置了SAVEPOINT。
   */
三种注释
#...
-- ...
/*...*/
source /opt/openfire/resources/database/openfire_mysql.sql;	// 执行某处sql


mysql配置文件my.ini
C:\ProgramData\MySQL\MySQL Server 5.7\my.ini
@[name]				用户变量
@@[name]			系统变量
mysql 5.6 添加追踪日志
vi /etc/my.cnf
添加以下两行
general_log=ON
general_log_file=runtime.log
service mysqld restart			重启mysqld服务
在以下路径，可看到runtime.log日志文件
/var/lib/mysqlmysql二进制文件路径

sc delete MySQL57

mysql> CREATE USER 'monty'@'localhost' IDENTIFIED BY 'some_pass';
mysql> GRANT ALL PRIVILEGES ON *.* TO 'monty'@'localhost'
-> WITH GRANT OPTION;
mysql> CREATE USER 'monty'@'%' IDENTIFIED BY 'some_pass';
mysql> GRANT ALL PRIVILEGES ON *.* TO 'monty'@'%'
-> WITH GRANT OPTION;
flush privileges;
