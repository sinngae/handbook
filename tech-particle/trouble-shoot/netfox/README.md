# netfox
做一只网络狐狸，灵巧地嗅探网络，监听网络的一举一动

抓包工具/网络嗅探sniffer/监听vs反监听

## ping

## telnet

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

## 其他
+ tcpdump 
+ wireshark
    + npcap winpcap  
Nmap Packet Capture (NPcap)
NPcap is the Windows version of the libpcap library; it includes a driver to support capturing packets.
Wireshark can use this library to capture live network data on Windows.
General information about the NPcap project can be found at the NPcap web site.
The libpcap file format description can be found at: Development/LibpcapFileFormat
The (development) releases can be downloaded from https://github.com/nmap/npcap/releases. As it's still in development it's not considered stable at this time.
Npcap is the Nmap Project's packet sniffing library for Windows. It is based on the wonderful Winpcap / Libpcap libraries, but with improved improved speed, portability, security, and efficiency. In particular, Npcap offers:
NDIS 6 Support
WinPcap compatibility
Loopback Packet Capture

+ nmap
+ netwox netwag
+ fiddler + postman