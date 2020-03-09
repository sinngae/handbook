# zookeeper

分布式中1.记录状态；2.事件通知；3.中央调度器；4.易用的数据管理；
zookeeper=文件系统+通知机制

**命名服务**
DNS URL 资源标识，唯一统一定位位置
**配置管理**
统一配置，主动通知
**集群管理**
当有机器动态加入，选举master
**分布式锁**
一致性文件系统，保持独占，控制时序
**队列管理**
1.同步队列 队列聚齐才可用，否则等待
2.FIFO队列


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