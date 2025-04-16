# svn


## 常用命令
```sh
# 设置LC_CTYPE以支持中文
export LC_CTYPE="zh_CN.UTF-8"
svn checkout [svn://192.168.1.12/demo] --username me --password 1234 /demo
svn co
# 查看状态
svn status [-v] [file|dir|null]
svn st
svn info [path]
# 同步远程文件
svn update
svn up
svn up -r m path # 更新到某个版本
# 提交文件
svn add [file]
svn commit -m "xxx" [-N] [--no-unlock] [path] # --no-unlock 保持锁的提交
svn ci
# 加锁/解锁
svn lock -m "msg" [--force] [path]
svn unlock [path]
# 删除
svn delete [path] -m "msg"
# 日志
svn log [path]
# 比较
svn diff [path]
svn di
# merge
svn merge -r m:n [path]
# revert
svn revert [path]
svn revert -R . # 回退所有修改
svn status --no-ignore | grep -E '(^\?)|(^\I)' | sed -e 's/^. *//' | sed -e 's/\(.*\)/"\1"/' | xargs rm -rf # 删除所有未trace的文件
# tag
svn cp trunk/ tags/platform_2020 
# 其他
svn ls
svn mkdir
svn switch [url path] # 切换仓库
svn cleanup # svn update失败，清理锁
# svn 钩子
```