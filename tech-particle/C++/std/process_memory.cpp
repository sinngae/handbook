/*************************************************************************
	> File Name: process_memory.cpp
	> Author: renzq
	> Mail: zqren@foxmail.com
	> Created Time: 2018年06月26日 星期二 16时51分27秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>

int bss_var;
int data_var0 = 1;

int main(int argc, char *argv[])
{
    printf("test location:\n");
    printf("\tAddress of main(Code Segment):%p\n", main);
    printf("------------------------------------\n");
    int stack_var0 = 2;
    printf("stack location:\n");
    printf("\tinitial end of stack:%p\n", &stack_var0);
    int stack_var1 = 3;
    printf("\tnew end of stack:%p\n", &stack_var1);
    printf("------------------------------------\n");
    printf("data location:\n");
    printf("\taddress of data_var(data segment):%p\n", &data_var0);
    static int data_var1 = 4;
    printf("\tnew end of data_var(data segment):%p\n", &data_var1);
    printf("------------------------------------\n");
    printf("bss location:\n");
    printf("\taddress of bss_var:%p\n", &bss_var);
    printf("------------------------------------\n");
    printf("heap location:\n");
    char *p = (char *)malloc(10);
    printf("\taddress of head_var:%p\n", p);
    
    return 0;
}
