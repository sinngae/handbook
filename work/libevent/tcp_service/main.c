#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <event.h>

struct event_base *base;

/* 读事件
 */
void on_read(int icli, short event, void *arg)
{
    printf("%d time out!\n", ++count);

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    // 重新添加定时事件 （定时事件触发后默认自动删除）
    event_add((struct event *)arg, &tv);
}

int main(int argc, char *argv[])
{
    event_init();   // 初始化

    // 设置定时事件
    struct event ev_timer;
    evtimer_set(&ev_timer, on_time, &ev_timer);

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    event_add(&ev_timer, &tv);

    // 事件循环
    event_dispatch();

    return 0;
}
