# Software
软件单体

## RPC/Web框架
一个RPC框架怠速（运行echo函数）消耗1%，优化了50%，整体服务性能也只提高了0.5%而已，基本上gRPC/Tars/brpc等框架的怠速消耗都不多，主要还是业务代码的消耗。那选择方案的时候，应该考虑框架的什么特性呢

framework:|gRPC|Tars|brpc|openresty|thrift
-|-|-|-|-|-
HTTP2<br>最新的HTTP协议支持|-|-|-|-|-
云原生<br>主流的持续交付支持|
上传/下载文件|
双向流|
集成功能<br>认证/多语言客户端/<br>序列化及反序列化|-


### gRPC
### Tars
https://github.com/Tencent/Tars/
### brpc
百度开源rpc

### thrift

### openresty
### gin
### beego