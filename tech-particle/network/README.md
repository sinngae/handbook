# 网络技术
网络是什么？网线 + 网卡（有线/无线/蓝牙）+ 交换机 + 路由器 + 调制解调器等硬件，加上网络协议

网络协议：
TCP/IP 协议族包括 TCP 协议、IP 协议和 ICMP 协议和 HTTP 协议等

## 网络接口层

## 网际层


## 传输层
TCP/IP

## 应用层

### HTTP超文本传输协议
websocket 和 HTTP2中的长连接的区别与优劣？
websocket基于tcp长连接的标准
websocket+HTML5
websocket握手

http2 长连接

WebRTC，web realtime communication

### 实时音视频传输协议
1. quic 
google
quick udp internet connection
2. hls
http live streaming
3. RTMP

### 电子邮件传输协议

### NAT穿透协议
STUN，Session Traversal Utilities for NAT，NAT会话穿越应用程序。
是一种网络协议，它允许位于NAT（或多重NAT）后的客户端**找出自己的公网地址**，查出自己位于哪种类型的NAT之后以及NAT为某一个本地端口所绑定的Internet端端口。这些信息被用来在两个同时处于NAT路由器之后的主机之间创建UDP通信。

TURN，Traversal Using Relay NAT，即通过Relay方式穿越NAT，又称SPAN(Simple Protocol for Augmenting NATs）方式。TURN应用模型通过**分配TURNServer的地址和端口作为客户端对外的接受地址和端口**，即私网用户发出的报文都要经过TURNServer进行Relay转发。TURN的局限性在于所有报文都必须经过TURNServer转发，增大了包的延迟和丢包的可能性。

### 局域网发现
UPnP
UPnP协议允许PC间的点对点连接、网际互连和无线设备。它是一种基于TCP/IP、UDP和HTTP的分布式、开放体系。
UPnP使得任意两个设备能在LAN控制设备的管理下相互通信。

DLNA

mDNS

## TCP/IP协议栈安全缺陷与攻击
1. 网络接口层
+ 以太网  共享传输媒介并明文传输
+ 以太网  缺乏MAC身份验证 MAC欺骗
+ PPP     明文传输

2. 网络层
+ IPV4	缺乏IP地址身份认证（IP地址欺骗/处理IP分片逻辑错误/IP分片攻击）
+ ICMP	ICMP路由重定向缺乏身份认证（ICMP路由重定向）
        广播地址对Ping的放大器效应（Ping Flood,Smurf）
+ ARP	采用广播问询且无验证机制	ARP欺骗
+ BGP	缺乏较强的身份认证机制	路由欺骗攻击

3. 传输层
+ TCP	三次握手存在连接队列瓶颈	TCP SYN Flood
+ TCP会话对身份认证不够安全	TCP RST 攻击
+ TCP会话对身份认证不够安全	TCP 会话劫持
+ UDP	N/A	UDP Flood

4. 应用层
+ DNS 验证机制不够安全	DNS欺骗
+ SMB	SMB协议的NTLM认证机制存在安全缺陷	SMB中间人攻击
+ HTTP	URL明文,缺乏完整性保护,编码滥用	钓鱼
+ HTTP 内嵌链接滥用