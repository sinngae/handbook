# nohup失效
`nohup servlet > /dev/null &`

以root用户身份登录到Shell，但是退出Shell后，servlet终止了。之前一直这么用，都是可以运行到1号进程下的。

详见：https://unix.stackexchange.com/questions/420594/why-process-killed-with-nohup

## nohup是什么

## 两种情况
有两种情况可能导致进程在受到的保护后被终止nohup，并且每种情况都有不同的解决方法。

一种可能性是系统使用systemd，其中logind.conf配置了KillUserProcesses=yes。在这种情况下，关闭终端不会造成问题，但是注销系统会造成问题。在这种情况下，解决方法是使用
`$ systemd-run --scope --user [command]`。这基本上只是告诉systemd它不应终止该进程。

另一种可能性是，产生的进程实现了自己的处理程序，SIGHUP该处理程序将覆盖的保护nohup。在这种情况下，即使您保持登录状态，退出Shell程序也会立即出现问题。您可以使用以下方法进行检查：
```shell script
$ nohup [command] &
$ grep Sig /proc/$!/status
# 应该看到一条线,（或其他十六进制数字字符串）
#   SigIgn: 0000000000000001
```
SIGHUP是信号编号1，因此，如果此大端十六进制数字设置了它的第一个（最低有效）位（即，最后一位是1、3、5、7、9，B，D或F中的一个），然后SIGHUP被忽略。否则，程序将安装自己的处理程序，以覆盖的保护nohup。

在这种情况下，解决方案是使用disown：`nohup [command] & disown`。
这会将进程从Shell的作业列表中删除，从而避免SIGHUP了首先发送到该进程。