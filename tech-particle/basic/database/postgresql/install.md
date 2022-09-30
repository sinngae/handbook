# postgreSql

## 二进制安装
```sh
# 1.新增用户组/用户/密码/
sudo groupadd sql
sudo useradd -g sql postgres
sudo passwd postgres

# 2.新建postgres目录
mkdir -p pgsq/{data,log}
sudo mkdir -p /home/SQL/Data/pgsql/{data,log}
sudo chown -R postgres.sql /home/SQL/Data/pgsql/

# 3.数据库服务初始化
initdb -E utf8 -D /home/aren/pgsq/data/
pg_ctl -D /home/aren/pgsq/data/ -l /home/aren/pgsq/log/postgres.log start
#   或者用
postgres -D /home/SQL/PostgreSQL/Data/data > //home/SQL/PostgreSQL/Data/log/postgres.log &）

# 4.创建数据库
createdb [username/dbname]

## psql
psql -h 127.0.0.1 -d mydb -U postgres -p 5432
```