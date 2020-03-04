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

struct st_
{
	char type;
	unsigned int size;
	char data[10];
};

class empty
{
};

class cls
{
	void func();
};

class cls_char
{
	char value;
};

class cls_int
{
	int value;
};

class cls_cmb
{
	char value;
	int ivalue;
	char value1;
};

class cls_cmb1
{
	char value;
	long long llvalue;
	char value1;
	int ivalue;
};

class base0
{
	public:
	virtual void func();
};

class base1
{
	public:
	virtual void func0();
};

class derived : public base0, public base1
{
	
};

int main(int argc, char *argv[])
{
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
    cout << "sizeof(int*) " << sizeof(int*) << endl;
	
	cout << "sizeof(st_) " << sizeof(st_) << endl;
	cout << "sizeof(empty) " << sizeof(empty) << endl;
	cout << "sizeof(cls) " << sizeof(cls) << endl;
	cout << "sizeof(cls_char) " << sizeof(cls_char) << endl;
	cout << "sizeof(cls_int) " << sizeof(cls_int) << endl;
	cout << "sizeof(cls_cmb) " << sizeof(cls_cmb) << endl;
	cout << "sizeof(cls_cmb1) " << sizeof(cls_cmb1) << endl;
	cout << "sizeof(base0) " << sizeof(base0) << endl;
	cout << "sizeof(derived) " << sizeof(derived) << endl;

    return 0;
}
