# zookeeper
是什么？  
zookeeper=文件系统+通知机制
一个通用的无单点问题的分布式协调框架，使开发人员专注于框架下的业务服务。

场景  
用于分布式中1.记录状态；2.事件通知；3.中央调度器；4.易用的数据管理；

## 功能
**数据发布与订阅**   
分布式通知/协调  
watcher注册与回调  
心跳检测、控制台/推送、工作汇报/任务分发

**命名服务**  
分布式命名服务，像DNS URL一样做资源标识，唯一统一定位位置

**配置管理**  
统一配置，主动通知，动态更新  
索引信息、系统日志、全局的配置、地址列表、offsets

**集群管理**  
当有机器动态加入，选举master  
临时/永久、有序/无序

**分布式锁**  
强一致性文件系统，保持独占，控制时序

**队列管理**  
1.同步队列 队列聚齐才可用，否则等待  
2.FIFO队列

**负载均衡**

## 实现原理
分布式节点 => 数据复制 => 容错、可扩展、高性能
1. 写主write master  
读无限制，主从复制，读写分离
2. 写任意write any（master）  
zk集群节点的角色和变化是透明的

## 性能
zk集群的tps达1w（每秒可以响应1w次节点注册/退出）  
单zk节点连接数6k，watcher数量30w  
吞吐量还可以，延时比较严重

etcd？
zab->raft
go GC
restful API

## 假死与脑裂
zookeeper框架基于FastPaxos算法，该算法存在活锁问题（礼貌性互让，反复加锁释放）。
zookeeper做了选举leader的优化，只有leader才能提交proposer。

大规模集群的长期运行中，必然产生宕机、断网等导致master不可用的情况。master/slaver模式下，slaver快速获知并迅速切为master。zookeeper用watcher和分布式锁机制解决监控切换问题。但存在脑裂问题（假死导致的，多批节点间通讯故障，互认为已死）。

怎么判断假死
zookeeper使用心跳判断节点是否活着。
如果一些节点连上新的master，而另有一些节点连着原master，问题就严重了（裂开的zk之间数据不一致）（zk集群与zkclient判断并不完全同步造成的）
如何避免
slaver切换master并不立马进行，而是等待一段时间（zk定义的超时时间），确保原master获知变更且做了shutdown操作，再切换。（此间，zk集群不可用）

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