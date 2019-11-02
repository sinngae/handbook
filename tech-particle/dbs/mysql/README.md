## 登录命令行
mysql -h 127.0.0.1 -u root -p testdb<br>
root1234<br>

## 事务
start transaction;<br>
set session transaction isolation level read committed;<br>
set @@session.autocommit=0;<br>
select * from stu where stu_no=1000002 for update;<br>
commit;<br>

## 查看进程访问信息
show processlist;<br>
<br>
/*<br>
   1、不管autocommit 是1还是0 <br>
   START TRANSACTION 。后只有当commit数据才会生效，ROLLBACK后就会回滚。<br>
<br>
   2、当autocommit 为 0 时<br>
   不管有没有START TRANSACTION。<br>
   只有当commit数据才会生效，ROLLBACK后就会回滚。<br>
<br>
   3.如果autocommit 为1 ，并且没有START TRANSACTION 。<br>
   调用ROLLBACK是没有用的。即便设置了SAVEPOINT。<br>
   */<br>

## 三种注释
...

-- ...

/*...*/

source /opt/openfire/resources/database/openfire_mysql.sql;	// 执行某处sql


## mysql配置文件my.ini
C:\ProgramData\MySQL\MySQL Server 5.7\my.ini<br>
@[name]				用户变量<br>
@@[name]			系统变量<br>
mysql 5.6 添加追踪日志<br>
vi /etc/my.cnf<br>
添加以下两行<br>
general_log=ON<br>
general_log_file=runtime.log<br>
service mysqld restart			重启mysqld服务<br>
在以下路径，可看到runtime.log日志文件<br>
/var/lib/mysqlmysql二进制文件路径<br>
<br>
sc delete MySQL57<br>
<br>
mysql> CREATE USER 'monty'@'localhost' IDENTIFIED BY 'some_pass';<br>
mysql> GRANT ALL PRIVILEGES ON *.* TO 'monty'@'localhost'<br>
-> WITH GRANT OPTION;<br>
mysql> CREATE USER 'monty'@'%' IDENTIFIED BY 'some_pass';<br>
mysql> GRANT ALL PRIVILEGES ON *.* TO 'monty'@'%'<br>
-> WITH GRANT OPTION;<br>
flush privileges;<br>
