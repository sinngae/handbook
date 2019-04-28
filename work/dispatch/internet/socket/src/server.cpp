#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <list>

using namespace std;
#define MAXLINE 4096

typedef enum EN_SERVER_STATUS
{
    SERVER_INIT = 0,
    SERVER_RUNNING = 1,
    SERVER_STOP = 2,
    SERVER_PAUSE = 3,
    SERVER_ABNORMAL = 4
}server_status;

server_status g_server_status;

void serve(int connfd)
{
	char buff[MAXLINE];
    int n = 0;
    while (g_server_status == SERVER_RUNNING)
    {
		n = recv(connfd, buff, MAXLINE, 0);
        if (n == 0)
        {
            break;
        }
		buff[n] = '\0';
		printf("recv msg from client: %s\n", buff);
        // do something
        sprintf(buff, "%s", "do something ok");
        if (send(connfd, buff, strlen(buff), 0) < 0)
        {
            printf("send msg error %s(errno:%d)\n", strerror(errno), errno);
            //exit(0);
        }
        usleep(1000);
    }
    close(connfd);
    cout << "connfd exit " << connfd << endl;
}

int main(int argc, char *argv[])
{
    cout << argv[0] << " start ok" << endl;

	int listenfd, connfd;
	struct sockaddr_in servaddr;
    unsigned int port = 6666;

    //if (argc > 1) 
    //    port = atoi(argv[1]);
    
    //if ( (listenfd = socket(AF_INET, SOCK_STREAM|SOCK_NONBLOCK, IPPROTO_TCP)) == -1 ) {
    if ( (listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1 ) {
	    printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
	    exit(0);
	}

	memset(&servaddr, 0x00, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);

	if ( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
		printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	if ( listen(listenfd, 10) == -1) {
		printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

    printf("======waiting for client's request======\n");
    g_server_status = SERVER_RUNNING;
    //thread thrd(serve);
    list<thread*> thread_list;
    
    int i  = 0;
	while(1) 
    {
	    if( (connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1) 
        {
            if (errno == EAGAIN)
            {
                usleep(500000);
                cout << "eagin <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< " << i++ << endl;
                continue;
            }
		    printf("accept socket error: %s(errno: %d)", strerror(errno), errno);
		    continue;
		}
        thread *thrd = new thread(serve, connfd);
        thread_list.push_back(thrd);
        usleep(1000000);
	}
    
	close(listenfd);

	return 0;
}
