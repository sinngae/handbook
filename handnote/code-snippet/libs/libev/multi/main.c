#include <ev.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>
//全局只能有一个默认的 struct ev_loop ，在线程中，需要通过 ev_loop_new() 再新建一个
#define log_info(fmt, args...) printf("%ju lwpid(%lu) " fmt, time(NULL), syscall(SYS_gettid), ##args)

static void repeate_hook(EV_P_ ev_timer *w, int revents)
{
    (void) w;
    (void) revents;
    (void) loop;

    log_info("repeate\n");
}

void *child1(void *arg)
{
    (void) arg;
    EV_P = ev_loop_new(0);

    ev_timer wtimer;

    log_info("child thread started.\n");

    ev_timer_init(&wtimer, repeate_hook, 0., 1.);
    ev_timer_start(EV_A_ &wtimer);

    ev_run(EV_A_ 0);

    return NULL;
}

int main (void)
{
    //EV_DEFAULT_DEC; /* default */
    struct ev_loop *loop = EV_DEFAULT;
    ev_timer wtimer;
    pthread_t tid1;

    pthread_create(&tid1, NULL, child1, NULL);

    ev_timer_init(&wtimer, repeate_hook, 0., 1.);
    ev_timer_start(EV_A_ &wtimer);

    /* now wait for events to arrive. */
    ev_run(EV_A_ 0);

    pthread_join(tid1, NULL);

    return 0;
}
