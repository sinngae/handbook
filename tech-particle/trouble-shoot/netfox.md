# netfox
做一只网络狐狸，灵巧地嗅探网络，监听网络的一举一动

抓包工具/网络嗅探sniffer/监听vs反监听

## ping

## nmap

## netwox

## tcpdump
对网络上的数据包进行截获的包分析工具。 tcpdump可以将网络中传送的数据包的“头”完全截获下来提供分析。它支持针对网络层、协议、主机、网络或端口的过滤，并提供and、or、not等逻辑语句来帮助你去掉无用的信息。
```sh
# 默认模式 - 监视第一个网络接口（网卡？）上所有的数据包
tcpdump
# 选项
#   -i [eth name]   - 指定网络接口
#   
```
## curl
```sh
curl选项
# 上传文件
curl "http://127.0.0.1:8801/media/sda3/.tmp/" -F "file=@a.mp4"
# 查看详细
curl "http://127.0.0.1:8801/media/sda3/.tmp/" -F "file=@a.mp4" -v

# 下载文件
curl -gO "http://127.0.0.1:8801/media/sda1/onecloud/tddownload/[阳光电影-www.ygdy8.com]十二夜-01.mp4"
curl -g "http://127.0.0.1:8801/media/sda1/onecloud/tddownload/[阳光电影-www.ygdy8.com]十二夜-01.mp4" -o a.mp4

# POST请求
curl localhost:9999/api/daizhige/article -X POST -d "title=comewords&content=articleContent" -H 'Authorization:adfasdfasdf'

# curl访问unix domain socket
# 当前openwrt的curl为7.38，暂不支持;最低版本要求：curl:7.40;
curl --unix-socket /var/log/nginx/socket/fdrawer.sock -g 'http:/fdrawer?opt=getimg'
curl --no-buffer -XGET --unix-socket /docker.sock http:/events

# cookie
--cookies "key=value"
curl --cookies "sessionid=ws001.1F4A8047BBD38F237BD4E217F125AE49;userid=1000001028;" http://local.api-game-ssl.xunlei.com/dailytask/list
```
## wget