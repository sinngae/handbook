# Linux shell

## 类型与数据结构

## 流程控制

## sh/fork/exec/source/(./)的区别

## 环境变量
linux是多用户操作系统，每个用户登录都有专有的一组环境变量，提供专用的运行环境。
常见的环境变量有PATH/HOME/HISTSIZE/LOGNAME/SHELL/LANG/MAIL/PS1。

### set命令
```sh
# set命令
#	仅对当前脚本进程起作用，对子进程不起作用
set			# 显示本地/当前变量
set -[x]	# 开启x指定的模式
set +[x]	# 关闭x指定的模式
set -o		# 查看打开的模式
#	常用：
#	-f	取消使用通配符，会造成通配符匹配文件失败
#	-e	若指令传回值不等于0，则立即退出shell
#		如set -e；ls xxx；xxx不存在，则退出终端
#	-m	使用监视模式
#	-v	打印读入shell的输入行
#	-x	执行命令前打印命令；执行指令后，打印
#	-e/-x对于调试shell特别实用
set first second third	# 依次给shell参数赋值
echo $3 $2 $1
```
```txt
# set -o返回的shell模式
allexport      	off
braceexpand    	on
emacs          	on
errexit        	off
errtrace       	off
functrace      	off
hashall        	on
histexpand     	on
history        	on
ignoreeof      	off
interactive-comments	on
keyword        	off
monitor        	on
noclobber      	off
noexec         	off
noglob         	off
nolog          	off
notify         	off
nounset        	off
onecmd         	off
physical       	off
pipefail       	off
posix          	off
privileged     	off
verbose        	off
vi             	off
xtrace         	on
```

**总结set/declare/evn/export命令**
+ set 显示本地变量/当前变量/当前用户的变量
+ env 显示环境变量/当前用户的变量
+ export 显示/设置环境变量，显示当前导出成用户变量的shell变量

## shell script
```sh
$# # 是传给脚本的参数个数
$0 # 是脚本本身的名字
$1 # 是传递给该shell脚本的第一个参数
$2 # 是传递给该shell脚本的第二个参数
$@ # 是传给脚本的所有参数的列表
$* # 是以一个单字符串显示所有向脚本传递的参数，与位置变量不同，参数可超过9个
$$ # 是脚本运行的当前进程ID号
$? # 是显示最后命令的退出状态，0表示没有错误，其他表示有错误
$@ # 规则的目标所对应的所有文件。表示规则中的目标文件集。在模式规则中，如果有多个目标，那么"$@"就是匹配于目标中模式定义的集合。

# $* 和 $@
#   都表示传递给函数或脚本的所有参数。
#   不被双引号(" “)包含时，都以”$1" “$2"…"$n” 的形式输出所有参数。
#   当它们被双引号(" “)包含时，”$*" 会将所有的参数作为一个整体，以"$1 $2 … $n"的形式输出所有参数；"@" 会将各个参数分开，以"$1" “$2"…"$n” 的形式输出所有参数。

```
```sh
# return/exit的异同
# shell脚本中函数调用会新建进程来执行（通过管道传递数据），所以exit只是退出当前进程，而不是退出整个脚本的执行。
#!/usr/bin/env bash
export mainpid=$$
trap "exit 1" TERM

function Fun2()
{
	echo "Fun2 " # 此处$$不能获取进程pid
	#kill -s TERM $mainpid
	kill -9 -$mainpid # 杀死整个进程组
	#kill -9 0	# 杀死当前进程组
	exit 2
	echo "how" >&2
}

function Fun()
{
	res=$(Fun2)
	echo "Fun "$$   # 此处$$不能获取其进程pid
	echo "are" >&2
	exit 1
}

echo "main "$$
#echo "test "getpid
res=$(Fun)
echo "you"

sleep 20

```

## 管道&xargs
管道把上一个命令的标准输出作为下一个命令的标准输入。如果下一个命令不是从标准输入读取数据，而是从其参数读取数据，就需要xargs把上一个命令的标准输出转成下一个目标命令的参数（多个参数）。
```sh
seq 9	# 输出1\n2\n3...9
seq 9|xargs	# 输出1 2 3..9
seq 9|xargs -n3 # 输出1 2 3\n4 5 6\n7 8 9

seq 9|xargs test.sh 
# test.sh
#	#!/bin/sh
#	for item in $@; do
#		cat $item
#	done
```