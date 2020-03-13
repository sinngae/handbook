# accept()函数报错，errno提示 open too many files

查找问题过程  
+ 1.查看全局打开文件数量限制
```sh
cat /proc/sys/fs/file-max
#   64398
```
查看已分配的文件句柄数、（正在释放）闲置的文件句柄数、最大文件句柄数
```sh
cat /proc/sys/fs/file-nr
#   1621    0    64398
```

+ 2.查看单进程打开文件数量限制
```sh
ulimit -n
#   1024
```

+ 3.查看demo打开文件句柄
```sh
lsof -c demo|wc
#   1045   10382  109687
lsof -c demo|grep STREAM|wc
#   978    9779  103637
```
demo打开了大量 unix /var/run/nginx/socket/demo.sock type=STREAM达到单进程分配句柄上限，导致accept函数失败

+ 4.复现
设备上存有大量视频，利用app拉取视频列表，在不停来回滑动视频列表的过程中使用lsof查看demo的打开文件的句柄数不停上升直至超过1024最多1074。  
间隔一段时间后，lsof查看停留在977不再减少，但是不影响用户使用。  
lsof再次查看，为1042，删选过unix domain socket为977，三天时间泄漏的句柄并没有被释放。

+ 5.分析
应该是unix domain socket文件句柄泄漏  
nginx连接断开，之前的socket句柄没有释放导致的。需要查看为什么断开，以及为什么没有释放socket。

+ 6.查看代码中对demo.sock是否有适当的释放：
队列满错误处理中，没有关闭socket句柄。

日志中，demo从11:36:15到11:38:28一直报错http: Client: accept() failed, Too many open files  
demo的接口多个请求都报错502 Bad Gateway（客户端日志）

