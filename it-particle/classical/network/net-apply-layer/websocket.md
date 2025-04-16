websocket
----

# 概念

## 背景
很多站点为了实现推送功能，采用的技术都是轮询（浏览器每隔一段时间向服务器发起HTTP请求，服务端返回最新的数据给客户端）。
轮询分为短轮询和长轮询（前者定时发起，立即返回；后者定时发起，有变更或超时时返回）。
轮询会消耗很多网络带宽资源，而起效的数据只是很小一部分。
短轮询足够短，可以实现近似实时。


# Websocket 是什么
HTML5 采用 WebSocket协议。

Websocket 使用 ws 或 wss 开头的同一资源标识符URI (wss 是使用了TSS的Websocket)。如，wss://www.baidu.com
Websocket 与 HTTP/HTTPS 使用相同的TCP端口，可以绕过大多防火墙的限制。
默认使用80端口，wss使用443端口。


## websocket简介
RFC 6455，应用层传输协议

Websocket使得服务端可以主动向客户端传输数据。
在浏览器和服务端握手之后，建立双向连接


## 适用场景：
+ 弹幕
+ 即时通讯，聊天
+ 协同编辑
+ 基于位置的应用
+ 体育实况更新
+ 股票基金报价实时更新
+ 可视化大屏，监控、数据实时更新
+ web客服
+ web游戏

websocket最适合浏览器上高频率低延迟的场景下的事件数据的交换和处理。

如果不是有高频率低延迟的需求，建议还是使用HTTP 开发复杂度低。


## 优点

http websocket 都是基于TCP的应用层协议

1. websocket 数据驱动主动推送数据，满足实时性要求
2. 浏览器和服务器全双工通信，两端都可主动发送数据，和socket一样，节省资源和带宽。
2. HTTP协议报文的头部太大，成百上千字节的头部大部分都是重复的，很多无效的内容，大多是浪费资源，而websocket 无需多次重复
4. 

## 缺点

1. 对开发者要求高，要求前端具备数据驱动js编程、维持ws连接。要求后端：1.长连接，（不可重启？可以重启，需要前端自动重连、不断重试，但是不能经常crash）；2.数据驱动推送消息；3.成熟的http生态下有大量的组件可以复用，而websocket还很新。
3. 服务器长时间挂着连接，肯定是消耗一定资源的。设置不活跃超时时间
4. 长连接使得后端的服务有了状态，那服务端如何做服务的水平扩展
5. 如何向指定的一个或多个客户端推送数据

从来只有客户端主动和服务端建立连接，而没有服务端主动和客户端建立连接。
服务端和客户端可以互相传输数据。

## websocket 报文格式
RFC标准要求 websocket是 message-based的协议，会自动处理数据分片、数据组装，不会出现粘包、半包问题，无需开发者关心tcp数据包缓存和组装。
具体的websocket实现可能不符，可能有各种问题，所以选择合适的轮子或者自己造。


握手报文
```
GET /chat HTTP/1.1
Host: server.example.com
Upgrade: websocket
Connection: Upgrade
Sec-Websocket-key: dsfsaeeuijkaluadfbuzsdflj==     # base64的密文，用于简单的认证密钥
Origin: http://example.com
Sec-Websocket-Protocol: chat, superchat
Sec-Websocket-Version: 13
```

## websocket 如何配置代理

websocket nginx 代理

```


map $http_upgrade $connection_upgrade {
    default upgrade;
    '' close;
}

upstream websocket {
    # 服务端口号
    server 172.0.0.1:58588;
}

... ...

server {
    listen 80;
    server_name 172.0.0.1;

    location / {
        root /var/www/sass-ui;
        index index.html index.html;

        # websocket 代理
        location /socketurl {
            proxy_pass http://websocket;
            proxy_http_version 1.1;
            
            # http/https 升级为 websocket ws/wss
            proxy_set_header Upgrade $http_upgrade;
            proxy_set_header Connection "connection_upgrade";
        }
    }
}
```

## websocket 服务重启，前端需要刷新页面