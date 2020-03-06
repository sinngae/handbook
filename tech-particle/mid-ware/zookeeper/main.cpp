/*************************************************************************
	> File Name: main.cpp
	> Author: renzq
	> Mail: zqren@foxmail.com
	> Created Time: Thu 10 May 2018 05:45:19 PM HKT
 ************************************************************************/

#include <iostream>
#include <string.h>
#include <sstream>
#include <stdint.h>

#include <boost/asio.hpp>
#include <boost/date_time.hpp>
#include <boost/thread.hpp>

#include "src/zookeeper_client.h"
#include "src/zookeeper_client_safe.h"

using namespace std;
boost::asio::io_service g_io_s;
boost::asio::io_service::strand g_strand(g_io_s);
boost::asio::io_service::work g_work(g_io_s);

uint32_t g_count = 0;

void usage()
{
    cout << "usage :\n"
            "   test a [without heartbeat];\n"
            "        b [with heartbeat]\n"
            "        c [with callback and heartbeat]\n"
            "        d [multithread but unsafe]\n"
            "        e [multithread and safe]\n"
            "        f [safe and extended heartbeat]\n" << endl;
}

void switch_to_bak_cb(void)
{
    cout << "hi, switch to backup callback" << endl;
}

void switch_to_main_cb(void)
{
    cout << "hi, switch to main callback" << endl;
}

void do_task()
{
    while (true)
    {
        cout << "hi, do_task " << g_count++ << endl;
        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
    }
    cout << "end of do_task" << endl;
}

void test_zkclient(log4cplus::Logger &logger, bool beating = false)
{
    zookeeper_client zk_client(logger, "192.168.1.172:2181,192.168.1.173:2181,192.168.1.174:2181");
    zk_client.login("test");

    while (true)
    {
        cout << "------------------------------------------" << g_count++ << endl;
        if (beating)
            if (zk_client.heartbeat() != ZK_CLIENT_OK)
                break;
        if (zk_client.is_elected() == ZKCLI_ISELECTED)
            cout << "do something" << endl;
        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
    }
}
   
void test_zkclient_cb(log4cplus::Logger &logger, bool beating = false)
{
    zookeeper_client zk_client(logger, "192.168.1.172:2181,192.168.1.173:2181,192.168.1.174:2181", 
                              30000, 3000, switch_to_main_cb, switch_to_bak_cb);
    zk_client.login("test");

    while (true)
    {
        cout << "------------------------------------------" << g_count++ << endl;
        if (beating)
            if (zk_client.heartbeat() != ZK_CLIENT_OK)
                break;
        if (zk_client.is_elected() == ZKCLI_ISELECTED)
            cout << "do something" << endl;
        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
    }
}

void do_task(zookeeper_client &zk_client)
{
    while (true)
    {
        cout << "-------------------------------------------" << g_count++ << endl;
        if (zk_client.heartbeat() != ZK_CLIENT_OK)
            break;
        if (zk_client.is_elected() == ZKCLI_ISELECTED)
            cout << boost::this_thread::get_id() << " -- do something" << endl;
        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
    }
}

void do_task_safe()
{
    while (true)
    {
        cout << "--------------------------------------------" << g_count++ << endl;
        if (g_zk_client_safe.heartbeat_safe() != ZK_CLIENT_OK)
            break;
        if (g_zk_client_safe.is_elected() == ZKCLI_ISELECTED)
            cout << boost::this_thread::get_id() << " -- do something" << endl;
        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
    }
}

void test_zkclient_mt(log4cplus::Logger &logger)
{
    zookeeper_client zk_client(logger, "192.168.1.172:2181,192.168.1.173:2181,192.168.1.174:2181", 
                              30000, 3000, switch_to_main_cb, switch_to_bak_cb);
    zk_client.login("test");
    boost::thread_group thread_group;

    for (int i = 0; i < 3; i++)
    {
        thread_group.create_thread(do_task_safe);
    }

    thread_group.join_all();
    g_zk_client_safe.logout();
}

void test_zkclient_safe_mt(log4cplus::Logger &logger)
{
    g_zk_client_safe.init(logger, "192.168.1.172:2181,192.168.1.173:2181,192.168.1.174:2181", 
                              30000, 3000, switch_to_main_cb, switch_to_bak_cb);
    g_zk_client_safe.login("test");
    boost::thread_group thread_group;

    for (int i = 0; i < 3; i++)
    {
        thread_group.create_thread(do_task_safe);
    }

    thread_group.join_all();
    g_zk_client_safe.logout();
}

void test_zkclient_safe_extend_heartbeat(log4cplus::Logger &logger)
{
    zookeeper_client zk_client(logger, "192.168.1.172:2181,192.168.1.173:2181,192.168.1.174:2181", 
                              30000, 3000, switch_to_main_cb, switch_to_bak_cb);
    zk_client.login("test");
    
    boost::posix_time::time_duration time_now(boost::posix_time::second_clock::local_time().time_of_day());
    for (int i = 0; i < 10; i++)
    {
        g_zk_client_safe.add(make_pair(time_now, time_now + boost::posix_time::seconds(10)));
        time_now += boost::posix_time::seconds(20);
    }
    g_zk_client_safe.start(1);
    boost::this_thread::sleep(boost::posix_time::seconds(30));
    g_zk_client_safe.stop();
    g_zk_client_safe.logout();
    g_zk_client_safe.clear_timepairs();
}

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

    // 日志
    Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("marka"));
    PropertyConfigurator::doConfigure("log4cplus.properties");
    
    char type = argc>1?argv[1][0]:'*';
    switch (type)
    {
    case 'a':// 错误用法
        test_zkclient(logger);
        break;
    case 'b':
        test_zkclient(logger, true);
        break;
    case 'c':
        test_zkclient_cb(logger, true);
        break;
    case 'd':
        test_zkclient_mt(logger);
        break;
    case 'e':
        test_zkclient_safe_mt(logger);
        break;
    case 'f':
        test_zkclient_safe_extend_heartbeat(logger);
        break;
    default:
        usage();
        break;
    }

    return 0;
}
