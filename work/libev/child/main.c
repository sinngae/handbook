#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "ev.h"

static void child_cb (EV_P_ ev_child *w, int revents)
{
    (void) revents;
    ev_child_stop(EV_A_ w);
    printf ("process %d exited with status %x\n", w->rpid, w->rstatus);
}

int main (void)
{
    /* use the default event loop unless you have special needs */
    //EV_DEFAULT_DEC; /* OR ev_default_loop(0) */
    struct ev_loop *loop = EV_DEFAULT;
    ev_child cw;

    pid_t pid = fork();
    if (pid < 0) {  /* error */
        perror("fork()");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {  /* child, the forked child executes here */
        sleep(3);
        exit(EXIT_SUCCESS);
    }

    printf("parent %d child %d forked.\n", getpid(), pid);

    /* parent */
    ev_child_init(&cw, child_cb, pid, 0);
    ev_child_start(EV_A_ &cw);

    /* now wait for events to arrive */
    ev_run(loop, 0);

    return 0;
}
