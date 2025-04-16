# cmd
使用go语言开发的工具，使用下列库：
+ github.com/spf13/cobra

## hh
hh命令，hand-helper/help-hand/short-hand，快捷命令。
```shell script
# install
go get github.com/sinngae/handtool/cmd/hh
# 生成分表sql
hh depart -f a.sql -s 199
```
已支持：
+ 数据库8零分表