1.timer
使用libevent的timer，每秒输出一段字符串
event_init()=>evtimer_set()=>event_add()=>event_dispatch()

2.tcp_service
监听本地8888端口，并输出客户端发送过来的信息
event_base_new()=>event_set()=>event_base_set()=>event_add()=>event_base_dispatch()
