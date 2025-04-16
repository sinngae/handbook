# mariadb

## 命令行
```sh
# 1.启动
systemctl start mariadb
# 2.设置开机启动
systemctl enable mariadb
# 3.简单配置
mysql_secure_installation
# 设置密码、删除匿名用户、禁止root远程登录、删除test数据库、重新加载权限表

# 4.测试登录
mysql -uroot -p[密码]

# 5.配置mariadb的字符集
#   5.1 设置客户端
vim /etc/my.cnf.d/mysql-clients.cnf
#   [mysql]下添加
#   default-character-set=utf8

#   5.2 设置服务端
vim /etc/my.cnf.d/server.cnf
#   [mysqld]下添加
init_connect='SET collation_connection=utf8_general_ci'
init_connect='SET NAMES utf8'
character-set-server=utf8
collation-server=utf8-general_ci
skip-character-set-client-handshake
#   开启慢查询
#   slow_query_log = ON
#   slow_query_log_file = /usr/local/mysql/data/slow.log
#   long_query_time = 1

# 6.重启mariadb
systemctl restart mariadb
# 7.进入mariadb，查看字符集
#   mysql>show variables like "%character%";
#   mysql>show variables like "%collation%";

# 8.添加用户，设置权限
#   8.1 创建用户
#   mysql>create user quant@localhost identified by 'quant1234';
#   8.2 授权 数据权限
#   mysql>grant all on *.* to quant@localhost identified by 'quant1234';
#   8.3 授予外网登录权限
#   mysql>grant all privileges on *.* to quant@'%' identified by 'quant1234';
#   8.4 授予二次授权权限
#   mysql>grant all privileges on *.* to quant@'hostname identified by 'quant1234';
## all privileges 或者 all 可以改为 select insert update delete create drop index alter grant references reload shutdown process file的一部分

#   mariadb重置密码
systemctl stop mariadb
mysqld_safe --skip-grant-tables &
ps -elf|grep mysql
mysql
#   mysql>use mysql;
#   mysql>update user set password=password('newpass') where user='root';
#   mysql>flush privileges;
#   mysql>exit
pkill mysql // kill 杀不死
systemctl start mariadb
```
