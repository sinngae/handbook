/*************************************************************************
	> File Name: test.cpp
	> Author: renzq
	> Mail: zqren@foxmail.com
	> Created Time: Mon 23 Apr 2018 05:05:26 PM HKT
 ************************************************************************/

#include <iostream>
#include <string.h>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])
{
    ostringstream oss;
    oss << "v1.0.0.1  build@" << __TIME__<< " " << __DATE__;
    string version = oss.str();
    if (argc > 1 && strcmp(argv[1], "-v") == 0)
    {
        cout << argv[0] << "\t" << version.c_str() << endl;
        return 0;
    }

    /*int *iptr;
    char *cptr;

    #if defined(__GNUC__)
    #  if defined(__i386__)
    __asm__("pushf\norl $0x40000,(%esp)\npopf");
    #  elif defined(__x86_64__)
    __asm__("pushf\norl $0x40000,(%rsp)\npopf");
    #  endif
    #endif
    cptr = (char *)malloc(sizeof(int)+1);
    iptr = (int*) ++cptr;
    *iptr = 42;*/
    cout << sizeof(int*) << endl;
    int ss[100] = "123456789";

    return 0;
}
