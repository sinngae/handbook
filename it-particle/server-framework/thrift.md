# thrift

## 先导
1. Trift的三个主要组件
+ protocal：定义了消息是怎么序列化的
+ transport：定义了消息是怎样在客户端和服务器端之间通信的
+ server：用于从transport接收序列化的消息，
    + 根据protocal反序列化，调用用户定义的消息处理器，
    + 序列化消息处理器的响应，将他们写回tansport

2. TProtocal 协议层 定义数据传输格式
    + TBinaryProtocal：二进制编码格式
    + TCompactProtocal：压缩的、密集的，基于Variable-length quantity的zigzag编码格式
    + TJSONProtocal：json
    + TSimpleJSONProtocal：提供JSON只读协议，生成的文件易于通过脚本语言解析
    + TDebugProtocal：使用易懂的可读的文本格式，便于debug

3. TTransport 传输层 定义数据传输方式，TCP/IP传输，内存共享、或者文件共享。被用作运行时库？
    + TSocket：阻塞性socket
