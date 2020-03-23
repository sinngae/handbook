# Distributed system
网络解决了计算机之间的通讯，而分布式系统要解决的是多个计算机（或虚拟计算机）更高层次的协作。

分布的多个计算机，节点自治，任务分解并行在各节点，共同提供统一的服务。

难点：
1.分区故障导致的系统瘫痪
2.性能和可靠性依赖网络
3.统一控制，机制与策略，动态扩容/缩容
4.资源的利用率低
5.安全，服务安全/数据安全

## 分布式理论
CAP（比对经典关系数据库ACID）

Base

共识算法：
Paxos算法/Raft算法

ZK vs etcd

## 分布式ID生成算法
UUID/snowflake

## 负载均衡算法
一致性hash算法

Redis vs memcache

## 分布式数据存储
MMP大规模并行处理
MMP vs NoSql
MMP vs Hadoop

Ceph

## 代理机制
agent/proxy/broker/delegate
主动代理/透传代理/中介代理/委托代理

nginx/haproxy/openresty

## 微服务
业务拆分=》中台化

## 虚拟化技术及容器
资源虚拟化=》服务治理
### Dokcer

## Service Mesh
### k8s
### Istio

## 全链路技术
全链路跟踪/压测
### ELK

