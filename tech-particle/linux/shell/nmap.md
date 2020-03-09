# nmap命令
抓包工具/网络嗅探sniffer/监听vs反监听

## ping
## nmap
## tcpdump

## curl
```
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