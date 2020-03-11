# Windows Power shell
```sh
## 网络
# 查看使用端口的进程信息
netstat -ano|findstr "49157"
# 配置host
#   超级管理员启动文本编辑工具打开C:\Windows\System32\drivers\etc\hosts  
#   添加"127.0.0.1 www.baidu.com"

## 进程
# 查看进程信息
tasklist|findstr "2720"
# 杀掉进程
tastkill /f /t /im 360safe.exe

## md5sum
certutil -hashfile yourfilename.ext MD5
```