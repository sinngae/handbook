/*************************************************************************
	> File Name: server.cpp
	> Author: 
	> Mail: 
	> Created Time: Mon 05 Feb 2018 09:37:47 PM EST
 ************************************************************************/

#include <iostream>
#include <string.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

using namespace std;

int main(int argc, char *argv[])
{
    string ip = "127.0.0.1";
    int port = 9999;
    char option = '\0';
    while ((option = getopt(argc, argv, "i:p:vh")) != -1)
    {
        switch (option)
        {
        case 'v':
            ostringstream oss;
            oss << "v1.0.0.1  build@" << __TIME__<< " " << __DATE__;
            cout << argv[0] << "\t" << oss.str() << endl;
            return 0;
        case 'i':
            ip = optarg;
            break;
        case 'p':
            port = atoi(optarg.c_str());
            break;
        case '?':
            cout << "unknown args:" << optopt << endl;
        case 'h':
            cout << "-i [ip]" << endl
                << "-p [port]" << endl
                << "-v    version" << endl
                << "-h    help" << endl;
            return 0;
        }
    }
    cout << "ip:" << ip.c_str() << ", port:" << port << endl;

    int ret = 0;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address, sin_family = AF_INET;
    inet_pton(AF_INET, ip.c_str(), &address.sin_addr);
    address.sin_port = htons(port);
    int lsn_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (lsn_fd < 0)
    {
        cout << "fail to create listen socket!\n" << endl;
        return -1;
    }

    ret = bind(lsn_fd, (struct sockaddr*)&address, sizeof(address));
    if (ret == -1)
    {
        cout << "fail to bind socket!" << endl;
        return -1;
    }

    ret = listen(lsn_fd, 5);
    if (ret == -1)
    {
        cout << "fail to listen socket!" << endl;
        return -1;
    }
    
    struct sockaddr_in clt_addr;
    socklen_t clt_addlen = sizeof(clt_addr);
    int cnn_fd = accept(lsn_fd, (struct sickaddr*)&clt_addr, &clt_addrlen);
    if (cnn_fd < 0)
    {
        cout << "fail to accept!" << endl;
        close(lsn_fd);
        return -1;
    }
    
    char buff[1024];
    fd_set read_fds;

    struct timeval tv;
    int err;

    FD_ZERO(&rd);
    FD_SET(0, &rd);

    tv.tv_sec = 5;
    tv.tv_usec = 0;
    err = select(1, &rd, NULL, NULL, &tv);
    if (err == 0)
    {
        cout << "select time out!" << endl;
    }
    else if (err == -1)
    {
        cout << "fail to select!" << endl;
    }
    else{
        cout << "data is available!" << endl;
    }

    return 0;
}
