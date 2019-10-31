#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>

#include <ev.h>
//通常用于多个线程之间的事件同步，该事件允许在不同的线程中发送事件消息，内部使用 PIPE 进行通讯
#define log_info(fmt, args...) printf("%ju lwpid(%lu) " fmt, time(NULL), syscall(SYS_gettid), ##args)

struct ev_loop *work_loop = NULL;
static struct ev_async wasync;

static void async_cb(EV_P_ struct ev_async *w, int revents)
{
    (void) w;
    log_info("async hook call, event %d loop %p.\n", revents, loop);
}

void *ev_create(void *p)
{
    (void) p;

    log_info("worker thread start!\n");

    sleep(3);
    ev_async_init(&wasync, async_cb);
    ev_async_start(work_loop, &wasync);
    ev_run(work_loop, 0);

    return NULL;
}

int main(void)
{
    int num = 0;
    pthread_t tid;

    work_loop = ev_loop_new(EVFLAG_AUTO);

    pthread_create(&tid, NULL, ev_create, NULL);
    log_info("main thread start!\n");

    while(1) {
        log_info("send async #%d times.\n", num);
        ev_async_send(work_loop, &wasync);
        sleep(1);
        num++;
    }

    return 0;
}
