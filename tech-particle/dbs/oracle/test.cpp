/*************************************************************************
	> File Name: test.cpp
	> Author: 
	> Mail: 
	> Created Time: Thu 04 Jan 2018 04:58:35 AM EST
 ************************************************************************/

#include<iostream>
#include <string>
#define LINUXOCCI
#include <occi.h>

using namespace std;
using namespace oracle::occi;

int main(int argc, char *argv[])
{
    Environment *env = Environment::createEnvironment(Environment::DEFAULT);
    try
    {
        Connection *conn = env->createConnection("work10g", "passwd");
        env->terminateConnection(conn);
    }
    catch (SQLException e)
    {
        return -1;
    }
    return 0;
}
