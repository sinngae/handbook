HTTP2
----
gRPC使用HTTP2作为传输协议。

HTTP2具有最大程度的服务兼容性，主流代理服务都支持。

HTTP1.1 相比之前提供了keep-alive、缓存、丰富的header语义。
有下列缺点
+ 对头阻塞 - Head-of-Line Blocking(No pipeling)
    + 既一个连接之只能跑一个请求，在该请求没有返回完数据之前，一直占用连接
    + 如果当前请求被阻塞，则该连接一直不可用
    + http1.1也提供了pipeline，但是多数浏览器都不支持，也未解决问题。

HTTP/2的好处
+ 多路复用 - 把每个请求封装成不同的stream，可以并发交错请求、响应，互不影响。
    + 降低了时延
+ header压缩
+ 流控
+ 优先级
+ 服务端推送

HTTP/2是一个经过实践检验的公开的标准。
CORBA是一个厂商们讨论出的标准，混乱而不可用。

HTTP/2 vs websocket?
?

HTTP/2 问题
+ 握手成本
    + 1 RTT（round trip time） TCP握手
    + 2 RTT LTS
+ TCP HOL（header-of-line） Blocking

HTTP/3 future特性
+ 使用UDP协议
+ 仅需 1 RTT + 1RTT（TLS 1.3）