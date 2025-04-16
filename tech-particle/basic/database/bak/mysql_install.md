# mysql install
```sh
# 二进制安装
#   下载解压建立软链到/usr/local/mysql
mkdir -p /home/aren/mysql/data
/usr/local/mysql/bin/mysqld --user=mysql --basedir=/usr/local/mysql --datadir=/data/mysql --initialize-insecure
mysqld_safe --user=aren
mysql -uaren -p

# 添加开机启动
cp /usr/local/mysql/support-files/mysql.server /etc/init.d/mysqld
chmod +x /etc/init.d/mysqld
chkconfig --list
chkconfig --add mysqld
reboot

# 远程连接
firewall-cmd --zone=public --add-port=3306/tcp --permanent
firewall-cmd --reload

## sql
mysql>show databases;
mysql>use [db name]
mysql>show tables;
mysql>describe [table name];
mysql>source tbls_create.sql

## 阻塞查看
mysql>show processlist; // 查看数据库连接，不大靠谱
mysql>show engine innodb status; // 无法判断问题的根本原因

mysqladmin debug;		// 能看到所有产生锁的线程，无法判断哪个是主因
# 开启innodb_lock_monitor后，再使用show engine innodb status查看，可以找到锁阻塞的主因

# 导出表数据
mysqldump -u[user] -h[host ip|host domain|] -p[passwd] [db name] [tbl name] > t_game_info.sql
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