#include <ev.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

static void fork_callback(EV_P_ ev_fork *w, int revents)
{
    (void) w;
    (void) revents;

    printf("[%d] fork callback\n", getpid());
}

static void timeout_callback(EV_P_ ev_timer *w,int revents)
{
    (void) w;
    (void) revents;

    printf("[%d] time out\n", getpid());
}

int main(void)
{
    struct ev_loop *loop;
    ev_fork wfork;
    ev_timer wtimer;

    loop = ev_default_loop(EVFLAG_FORKCHECK);

    ev_fork_init(&wfork, fork_callback);
    ev_fork_start(EV_A_ &wfork);

    ev_timer_init(&wtimer, timeout_callback, 1., 1.);
    ev_timer_start(EV_A_ &wtimer);

    pid_t pid;

    pid = fork();
    if (pid < 0) {
        return -1;
    } else if (pid == 0) {
        printf("[%d] Child\n", getpid());
        //ev_loop_fork(EV_A);
        ev_run(EV_A_ 0);
        ev_loop_destroy(EV_A);
        return 0;
    }

    printf("[%d] Parent\n", getpid());

    ev_run(EV_A_ 0);
    ev_loop_destroy(EV_A);

    return 0;
}
