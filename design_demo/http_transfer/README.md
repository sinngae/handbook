http transfer
----
# 背景
对于HTTP协议，haproxy/nginx等网络接入层不能实现api级别的注册？
常用的是将某一规则的url path经路由转发到对应的后端服务集群。但是，后端服务集群多次重构之后，urlpath变化巨大，和原有的路由规则差别太大。
