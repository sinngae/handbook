/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: Tue 09 Jan 2018 01:04:25 AM EST
 ************************************************************************/
#define _GNU_SOURCE
#include <sched.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// new version
#define gettidv1() syscall(__NR_gettid)
// traditional version
#define gettidv2() syscall(SYS_gettid)

int is_running = 1;

void printf_info(const char *arg)
{
    printf("arg:\t%s,\tgetpid:\t%d,\tgetppid:\t%d,\tgettidv1:\t%ld,\tgettidv2:\t%ld,\tpthread_self:\t%ld\n", 
                arg, getpid(), getppid(), gettidv1(), gettidv2(), pthread_self());
}

void test_fork()
{
    int count = 1;
    int pid;
    pid = fork();
    if (pid == -1) {
        printf("fork failed, errno:%d, errmsg:%s\n", errno, strerror(errno));
        exit(-1);
    }
    if (pid == 0) { // child
        printf_info("forkchild");
        printf("XXXXXXXXXXXXXXXXXXXXXXXX          pid.count   :%d(%p)\n", ++count, &count);
    }
    else {
        //printf_info("parent  ");   
        printf("XXXXXXXXXXXXXXXXXXXXXXXX          parent.count:%d(%p), pid:%d\n", count, &count, pid);
    }
}

void *stack;
int do_thing(void *arg)
{
    printf_info("do_thing");
    free(stack);
    return 0;
}

void test_clone()
{
    stack = malloc(8192);
    if (! stack) {
        printf("malloc stack error\n");
        exit(0);
    }
    int retval = clone(&do_thing, (char*)stack+8192, CLONE_VM|CLONE_VFORK, 0);
    if (retval == -1) {
        printf("clone failed, errno:%d, errmsg:%s\n", errno, strerror(errno));
        exit(-1);
    }
    exit(0);
}

void *do_action(void *arg)
{
    printf_info("do_action");

    return NULL;
}

void test_pthread()
{
    pthread_t thread;
    int retval = pthread_create(&thread, NULL, do_action, NULL);
    if (retval != 0) {
        printf("pthread_create failed, retval:%d, errno:%d, errmsg:%s\n", retval, errno, strerror(errno));
        exit(-1);
    }

    pthread_join(thread, NULL);
}

int main(int argc, char *argv[])
{
    printf_info("main    ");

    int option = 0;
    if (argc > 1) 
      option = atoi(argv[1]);

    switch (option) {
        case 0:
            test_pthread();
            break;
        case 1:
            test_fork();
            break;
        case 2:
            test_clone();
            break;
        default:
            break;
    }

    return 0;
}
