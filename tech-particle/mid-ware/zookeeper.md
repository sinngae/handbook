# zookeeper

## 命令速记
```
bin/zkServer.sh start

bin/zkCli.sh -server 127.0.0.1:2181
	ls /
	create /zk_test my_data
	get /zk_test
	set /zk_test junk
	delete /zk_test
```