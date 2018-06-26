/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: Tue 09 Jan 2018 01:04:25 AM EST
 ************************************************************************/

#include <iostream>
#include <sys/types.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// new version
#define gettidv1() syscall(__NR_gettid)
// traditional version
#define gettidv2() syscall(SYS_gettid)

using namespace std;

bool is_running = true;

void cout_info(const char *arg)
{
    cout << "arg:" << arg << endl;
    cout << "getpid:" << getpid() << endl;
    cout << "getppid:" << getppid() << endl;
    cout << "gettidv1:" << gettidv1() << endl;
    cout << "gettidv2:" << gettidv2() << endl;
    cout << "pthread_self:" << pthread_self() << endl;
    cout << "end" << endl;
}

void *do_action(void *arg)
{
    cout_info("do_action");
    while(is_running)
    {
        sleep(1);
    }
    cout << "do_action end" << endl;
    return NULL;
}

void test_fork()
{
    int count = 1;
    int child;
    child = fork();
    if (child < 0)
    {
        cout << "fork error !" << endl;
    }
    else if (child == 0) // child
    {
        cout_info("child");
        printf("child.count:%d(%p)\n", ++count, &count);
    }
    else
    {
        cout_info("parent");   
        printf("parent.count:%d(%p)\n", count, &count);
    }
}

void *stack;
int do_thing(void *arg)
{
    cout_info("do_thing");
    free(stack);
    return 0;
}

void test_clone()
{
    stack = malloc(8192);
    if (!stack)
    {
        cout << "malloc stack error" << endl;
        exit(0);
    }
    clone(&do_thing, (char*)stack+8192, CLONE_VM|CLONE_VFORK, 0);
    exit(0);
}

void test_pthread()
{
    pthread_t id;
    int ret = 0;
    ret = pthread_create(&id, NULL, do_action, NULL);
    if (ret)
    {
        cout << "create pthread error!" << endl;
        exit(-1);
    }

    pthread_join(id, NULL);
}

int main(int argc, char *argv[])
{
    cout_info("main");

    //test_pthread();
    //test_fork();
    test_clone();

    return 0;
}
