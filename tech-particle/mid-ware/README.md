# middle-ware
为什么使用它们？
它们解决了什么问题？
它们有什么特点？
使用中出现了什么问题？

1.场景
2.是什么？
3.做什么？
4.怎么做到的？

## 代理
代理服务：|apache|nginx|lighttpd|haproxy
-|-|-|-|-
proxy代理|非常好|非常好|一般|-
rewriter|好|非常好|一般|-
fcgi|不好|好|好|-
热部署|不支持|支持|不支持|-
怠速消耗|很大|很小|比较小|-
稳定性|好|非常好|不好|-
安全性|好|一般|一般|-
技术支持|非常好|一般|很少|-
静态文件处理|一般|非常好|好|-
vhosts虚拟主机|支持|不支持|支持|-
反向代理|一般|非常好|一般|-
session sticky|支持|不支持|不支持|-
### nginx
### haproxy
### lvm

## 消息队列
### kafka
### nsq
### flume

## Nosql/K-V
### etcd
### redis

## service mgr
### zk

## elk根据日志级别触发警告
es插件watcher / elastAlert
ERROR/FATAL级别日志 触发钉钉/微信/邮件告警

DEBUG日志仅用于调试

INFO日志用于链路跟踪

### 钉钉告警

### 微信告警

### 短信告警

### 邮件告警
