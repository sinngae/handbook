#include <ev.h>
#include <stdio.h>
#include <signal.h>

static void sigint_cb(EV_P_ ev_signal *w, int revents)
{
    (void) revents;

    printf("catch SIGINT, signal number %d.\n", w->signum);
    ev_break(EV_A_ EVBREAK_ALL);
}

int main (void)
{
    struct ev_loop *loop = EV_DEFAULT;
    ev_signal wsig;

    // use the default event loop unless you have special needs
    //EV_DEFAULT_DEC; /* OR ev_default_loop(0) */

    ev_signal_init(&wsig, sigint_cb, SIGINT);
    ev_signal_start(EV_A_ &wsig);

    ev_run(loop, 0); // now wait for events to arrive

    return 0;
}
