HTTP big body
---
大body的HTTP请求如何实现？

+ P0 - gzip压缩，60%压缩比，只压缩文本数据，不压缩多媒体文件
+ P1 - 拆分成多个请求
+ P2 - 使用文件交互
+ P3 - 使用？


## Transfer-Encoding: chunked
只用于应答body的大文件；且很少使用，有技术风险。

“Transfer-Encoding: chunked”和“Content-Length”这两个字段是互斥的，也就是说响应报文里这两个字段不能同时出现，一个响应报文的传输要么是长度已知，要么是长度未知（chunked）
（优先当作Context-Length处理）