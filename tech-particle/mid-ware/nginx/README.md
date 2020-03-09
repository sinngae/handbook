# nginx
‘engine x’，基于BSD许可证

一款十分轻量级的http服务器

## 适用场景
http服务
后端反向代理服务
IMAP/POP3/SMTP服务器代理

### http服务的基本特性
a.处理静态文件，索引文件以及自动索引；打开文件描述符缓冲；
b.五缓存的反向代理加速
c.fast-cgi，简单的负载均衡和容错。
d.模块化的结构
包括gzipping，byte ranges，chunked reponses等filter
e.SSI-filter
SSI（Server Side Includes），是在HTML静态页面中使用的指令，当页面在服务端执行的时候是可以进服务端解析的。
可以分块动态生成内容而不在整个执行后进行的动态页面服务。
f.支持SSL和TLSSNI

## 事件驱动
支持内核select/poll/epoll模型
select/poll调用时，可配置每个进程连接数。 

## 模块库丰富，配置灵活
模块化，支持mod-fcgi、ssl、vhosts；
支持负载均衡、反向代理 

## 高性能，稳定，系统资源消耗低
响应静态资源速度非常快；
nginx专为性能优化开发，经受高负载考验，可支持5w连接数。
很多HTTP服务遇到访问峰值，或恶意慢连接，物理内存很快耗尽，页频繁交换，失去响应，只能重启服务。apache 200个连接以上，响应变慢。
nginx采取分阶段资源分配，CPU使用和内存占用很低，1w个没有活动的连接，仅占用2.5MB内存。
DOS攻击对nginx基本无用。

lighttpd性能也高，问题也多

## 热部署
启动容易，且可以7*24小时不间断运行数月不用重启，还支持平滑重启。

## master-slave多进程模型
充分利用SMP优势，减少worker进程在磁盘IO的阻塞延时。

## 代码质量高、规范、成熟、模块化易扩展

## 强大的Upstream
upstream为反向代理与其他服务通信模块的编写提供基础。

## Filter链
filter链的各filter不必等待前一个执行完，其输出即作为当前filter的输入，像unix的管线一样。
一个模块压缩从后端发送过来的应答，且在模块接收后端整个数据之前，把压缩流转向客户端。

## 采用最新OS特性，性能很大提高
sendfile（Linux 2.2+）
accept-filter（FreeBSD 4.1+）
TCP_DEFER_ACCEPT(Linux 2.4+)
