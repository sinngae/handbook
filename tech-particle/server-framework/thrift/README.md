# Thrift
Facebook开发后捐赠给Apache的跨语言的RPC服务框架，支持Java/C++/Python

IDL  
（Interface description language），接口描述语言，是用来描述软件组件介面的一种计算机语言。 IDL通过一种中立的方式来描述接口，使得在不同平台上运行的对象和用不同语言编写的程序可以相互通信交流。

支持二进制序列化

多工作模式，单线程/线程池/非阻塞

## 结构
TCP传输层/thrift协议层/业务层

thrift调用过程：  
客户端调用->协议层封装->传输层传输->协议层解封->processor->调用业务方法->协议层封装->传输层传输->协议层解封->客户端应答
