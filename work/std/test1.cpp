/*************************************************************************
	> File Name: test1.cpp
	> Author: renzq
	> Mail: zqren@foxmail.com
	> Created Time: 2018年06月27日 星期三 16时04分51秒
 ************************************************************************/

#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    char a[0];
    cout << sizeof(a) << endl;
    char *p = new char[0];
    cout << sizeof(*p) << endl;
    return 0;
}
