/*************************************************************************
	> File Name: test1.cpp
	> Author: renzq
	> Mail: zqren@foxmail.com
	> Created Time: 2018年06月27日 星期三 16时04分51秒
 ************************************************************************/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

//typedef (void *)(*pfn)(void *) pfunc;

void *func(void *)
{
    return NULL;
}

int main(int argc, char *argv[])
{
    char buff[1024] = {0};
    char *cache = (char *)malloc(sizeof buff);
    void * pf = (void *)&func;
    void * pf1 = (void *)func;
    printf("buff addr:%p, cache addr:%p, pf:%p, pf1:%p, func:%p\n", buff, cache, pf, pf1, func);

    return 0;
}
