/*************************************************************************
	> File Name: test.cpp
	> Author: 
	> Mail: 
	> Created Time: Tue 02 Jan 2018 10:04:24 PM EST
 ************************************************************************/

#include<iostream>
#include <signal.h>
#include <unistd.h>
#include <cstring>
#include <cassert>
#include <cstdio>

using namespace std;

void sigdo(int sig)
{
    switch (sig)
    {
    case SIGHUP:
        cout << "sighup" << endl;
        signal(SIGHUP, SIG_IGN);
        break;
    case SIGINT:
        cout << "sigint" << endl;
        signal(SIGINT, SIG_IGN);
        break;
    case SIGTERM:
        cout << "sigterm" << endl;
        signal(SIGTERM, SIG_IGN);
        break;
    default:
        cout << "signal:" << sig << endl;
        break;
    }
}

void test_signal()
{
    cout << "pid" << getpid() << endl;
    signal(SIGHUP, sigdo);
    signal(SIGINT, sigdo);
    signal(SIGTERM, sigdo);
    for (;;);
}

int main(int argc, char *argv[])
{
    cout << stdout << endl;
    cout << stdin << endl;
    cout << cout << endl;

    return 0;
}
