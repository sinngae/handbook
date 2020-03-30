# lsof
```sh
# 查看进程打开了哪些文件
lsof -c mysql
lsof -u username
lsof -p pid
lsof -i             # 列出所有网络连接
lsof -i tcp         # 列出所有tcp连接
lsof -i :3306       # 列出所有使用端口的信息
lsof -i tcp:80
lsof -a -u test -i  # 列出某个用户的所有活跃的网络端口
```