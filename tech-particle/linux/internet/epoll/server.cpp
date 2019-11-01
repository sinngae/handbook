#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

#define MAXLINE 5
#define OPEN_MAX 100
#define LISTENQ 20
#define INFTIM 1000

void setnonblocking(int sock)
{
	int opts;
	opts = fcntl(sock, F_GETFL);
	if (opts < 0) 
	{
		perror("fcntl fail");
		exit(1);
	}

	opts = opts|O_NONBLOCK;
	if (fcntl(sock, F_SETFL, opts) < 0)
	{
		perror("fcntl fail2");
		exit(1);
	}
}

void print_sockname(int sock)
{
    sockaddr_in addr;
    socklen_t addrlen;
    int ret = getsockname(sock, (sockaddr*)&addr, &addrlen);
    if (ret == 0)
    {
        char *str = inet_ntoa(addr.sin_addr);
        cout << "sockfd: " << sock 
            << " ip: " << str << endl;
    }
    else// ret == -1
    {
        cout << "errno: " << errno << endl;
    }
}

int main(int argc, char* argv[])
{
	int i, listenfd, connfd, sockfd, epfd, nfds, portnumber;
	size_t n;
	char line[MAXLINE];
	socklen_t clilen;

	if (2 ==  argc)
	{
		if ((portnumber = atoi(argv[1])) < 0)
		{
			fprintf(stderr, "Usage:%s portnumber\a\n", argv[0]);
			return 1;
		}
	}
	else
	{
		fprintf(stderr, "Usage:%s portnumber\a\n", argv[0]);
		return 1;
	}


	struct epoll_event ev, events[20];

	epfd = epoll_create(256);
	struct sockaddr_in clientaddr;
	struct sockaddr_in serveraddr;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
    // 把socket设置为非阻塞方式
    // setnonblocking(listenfd);

	ev.data.fd = listenfd;
	ev.events = EPOLLIN|EPOLLET;

	epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	char local_addr[] = "127.0.0.1";
	inet_aton(local_addr, &(serveraddr.sin_addr));

	serveraddr.sin_port = htons(portnumber);
	bind(listenfd, (sockaddr*)&serveraddr, sizeof(serveraddr));
	//maxi = 0;
	listen(listenfd, LISTENQ);

	for ( ; ; )
    {
        cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> new loop" << endl;
        nfds = epoll_wait(epfd, events, 20, 500);
        for (i = 0; i < nfds; ++i) 
        {
            cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>> epoll_wait" << endl;
            if (events[i].data.fd == listenfd)
            {
                cout << ">>>>>listenfd event" << endl;
                connfd = accept(listenfd, (sockaddr*)&clientaddr, &clilen);
                if (connfd < 0) 
                {
                    perror("connfd < 0");
                    exit(1);
                }
                
                char *str = inet_ntoa(clientaddr.sin_addr);
                cout << "accept a connection from " << str << endl;
                ev.data.fd = connfd;
                ev.events = EPOLLIN|EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
            } 
            else if (events[i].events & EPOLLET)
            {
                cout << ">>>>>EPOLLET" << endl;
                sockfd = events[i].data.fd;
                if (sockfd < 0)
                    continue;
                print_sockname(sockfd);
                n = read(sockfd, line, MAXLINE);
                if (n < 0)
                {
                    if (errno == ECONNRESET)
                    {
                        cout << "-----ECONNRESET" << endl;
                        close(sockfd);
                        events[i].data.fd = -1;
                    }
                    else
                    {
                        cout << "-----readline error" << endl;
                    }
                } 
                else if (n == 0)
                {
                    cout << "-----read 0, close" << endl;
                    close(sockfd);
                    events[i].data.fd = -1;
                }
                else
                {
                    line[n] = '\0';
                    cout << "read " << line << endl;
                 
                    ev.data.fd = sockfd;
                    ev.events = EPOLLOUT|EPOLLET;
                    epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
                }
            }
            else if (events[i].events&EPOLLOUT)
            {
                cout << ">>>>>EPOLLOUT" << endl;
                sockfd = events[i].data.fd;
                print_sockname(sockfd);

                write(sockfd, line, n);
                
                ev.data.fd = sockfd;
                ev.events = EPOLLIN|EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
            }
        }
    }
    return 0;
}
