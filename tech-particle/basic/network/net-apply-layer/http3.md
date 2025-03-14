HTTP3 
----
HTTP3 基于QUIC

基于UDP实现可靠传输？

参考：https://zhuanlan.zhihu.com/p/431672713
https://zhuanlan.zhihu.com/p/405387352

# HTTP2的问题

## 有序字节流的对头阻塞 Head-of-line blocking
使得HTTP2的多路复用能力大打折扣

## TCP与TLS叠加握手时延
建立连接的时长还可以减少。
度量时延：RTT, Round-Trip Time, 从发送数据到接受确认 总共经历的延时。
1. TCP 三次握手，1.5个RTT
2. TLS 两次握手，1个RTT, 密钥协商


## 移动切换入网
基于TCP四元组确定一个连接，适用于固定接入点，不适合移动互联网。
后者意味着IP地址的频繁变动导致TCP连接、TLS会话反复握手

# HTTP3
解决了：
1. 基于UDP协议重新定义了连接，实现了无序、并发字节流传输问题，解决了对头阻塞问题（基于QPack解决了动态表的对头阻塞）
2. 重新定义TLS协议加密Quic头部的方式，提高了网络攻击成本、减少了建立连接耗时
3. 将Packet、QuicFrame、HTTP3 Frame分离，实现连接迁移功能。实现5G移动设备的连接维护成本的降低。


注意：HTTP2与HTTP3采用二进制、静态表、动态表与Huffman算法对HTTP Header编码，已经提供了高压缩率，还加快了发送端编码、接收端解码的速度。


+ QUIC层由https://tools.ietf.org/html/draft-ietf-quic-transport-29描述，它定义了连接、报文的可靠传输、有序字节流的实现；
+ TLS协议会将QUIC层的部分报文头部暴露在明文中，方便代理服务器进行路由。https://tools.ietf.org/html/draft-ietf-quic-tls-29规范定义了QUIC与TLS的结合方式；
+ 丢包检测、RTO重传定时器预估等功能由https://tools.ietf.org/html/draft-ietf-quic-recovery-29定义，目前拥塞控制使用了类似TCP New RENO的算法，未来有可能更换为基于带宽检测的算法（例如BBR）；
+ 基于以上3个规范，https://tools.ietf.org/html/draft-ietf-quic-http-29定义了HTTP语义的实现，包括服务器推送、请求响应的传输等；
+ 在HTTP2中，由HPACK规范定义HTTP头部的压缩算法。由于HPACK动态表的更新具有时序性，无法满足HTTP3的要求。在HTTP3中，QPACK定义HTTP头部的编码：https://tools.ietf.org/html/draft-ietf-quic-qpack-16。注意，以上规范的最新草案都到了29，而QPACK相对简单，它目前更新到16。


自1991年诞生的HTTP/0.9协议已不再使用，但1996推出的HTTP/1.0、1999年推出的HTTP/1.1、2015年推出的HTTP2协议仍然共存于互联网中（HTTP/1.0在企业内网中还在广为使用，例如Nginx与上游的默认协议还是1.0版本），即将面世的HTTP3协议的加入，将会进一步增加协议适配的复杂度。

## 报文结构
UDP头 + Packet Header + QUIC Frame Header + HTTP3 Frame Header + HTTP Message

+ Packet Header实现了可靠的连接。UDP报文丢失后，通过其中的Packet Number实现重传，连接也是通过其中的Connection ID字段定义的
+ QUIC Frame Header在无序的Packet报文中，基于QUIC Stream概念实现了有序的字节流，允许HTTP消息可以像TCP连接上一样传输
+ HTTP3 Frame Header 定义了HTTP Header、 Body的格式，服务器推送、QPACK编解码流等功能
+ Packet Header 分两种：Long Packet Header 用于首次建立连接；Short Packet Header用于后续传输数据；

## 移动切换重连
对于HTTP的TCP，一个连接是通过四元组确定，断开后需要重新建立连接（2个RTT建立连接、TCP慢启动 先指数后线性）。
对于HTTP3的QUIC，允许IP地址变更，只要保有上下文信息（连接ID、TLS密钥），就可以复用原连接。



# 前向安全
前向安全，用来产生会话密钥的长期密钥泄露后，也不会泄露以前的通讯内容。