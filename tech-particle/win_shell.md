# 网络
## 查看使用端口的进程信息
netstat -ano|findstr "49157"

# 进程
## 查看进程信息
tasklist|findstr "2720"

## 杀掉进程
tastkill /f /t /im 360safe.exe
