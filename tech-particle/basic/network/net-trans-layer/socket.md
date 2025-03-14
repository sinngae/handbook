Socket
-----

## Socket
socket中文翻译为插口、插座。

Socket 技术用语所谓套接字，是对网络中主机上应用进程之间进行的双向通信的端点的抽象
```
CPU的插座 Socket 370\Socket 462\Socket AM3，是实体插座。

ARPANET:
a "connection" is a pair: a "send socket" at one end and a "receive socket" at the other.
... ...
Further, the ARPANET is just beginning to evolve a view of "process-to-process" protocols where cooperating programs on dissimilar systems communicate over network sockets in a true use of sockets as interprocess communication media.

Unix BSD TCP/IP:
Socket layer
The socket layer deals with the interprocess communications facilities provided by the system. A socket is a bidirectional endpoint of communication which is "typed" by the semantics of communication it supports.

套接管，socket pipe，用于套接的水管
```
socket是一种进程间通信机制，提供一种供应用程序访问通信协议的操作系统调用，并且通过将socket与Unix系统文件描述符相整合，使得网络读写数据 和 读写文件一样操作。socket不再是硬件上的物件，而是一组操作“指令”。按汉语理解，具备“套接”（建立进程间网络通讯）和“字”（可读写交互的）。
也曾被翻译成 媒介字、信口、软插口。


## 
### 1. 流套接字 sock_stream
面向连接，可靠的数据传输服务。
使用的是传输控制协议，TCP the Transmission Control Protocol。
序列号、确认应答、超时重传、流量控制、拥塞控制。
### 2. 数据报套接字 sock_dgram
无连接、不可靠的服务。
不能保证数据传输的可靠性，数据可能丢失、重复、乱序。
UDP, User Datagram Protocol

### 3. 原始套接字 sock_raw
可以读写内核没有处理的IP数据包（流套接字 TCP协议报文，数据报套接字 UDP协议报文）
需要访问其他协议发送的数据。


## UDS, Unix Domain Socket
本地的进程间通信。

## 基于UDP协议实现可靠传输 QUIC

详见HTTP3


## 报文大小限制总结

HTTP报文不限制大小，TCP协议有最大报文段长度，HTTP大文件会被拆分成多个传输。
HTTP get url没有明确限制，但一般不超过2kB；post 可以处理数十MB 或更大的请求体。

nginx默认请求体大小 1MB，如果HTTP POST请求体的数据超过1MB会被nginx拒绝，413 Request entity too large错误。（client_max_body_size，重启生效）
tomcat 请求体大小默认2MB




