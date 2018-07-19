#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <list>

const char *cfg_addr = "127.0.0.1";
const unsigned short cfg_port = 2370;
const unsigned int cfg_maxline = 4096;
const unsigned int cfg_threadnum = 10;

void *handle_accept(void *);

int main(int argc, char *argv[])
{
	int listenfd, connfd;
	struct sockaddr_in servaddr;
    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {
	    printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
	    exit(0);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(cfg_port);

	if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
		printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	if( listen(listenfd, 10) == -1) {
		printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}
    printf("======waiting for client's request======\n");
    
    struct sockaddr_in client;
    socklen_t socklen = sizeof(struct sockaddr);
	std::list<pthread_t> thread_list;
	while(1) {
        if( (connfd = accept(listenfd, (struct sockaddr *)&client, &socklen)) == -1) {
	        printf("accept socket error: %s(errno: %d)", strerror(errno), errno);
	        return -1;
        }
        printf("client on [ip:%s, port:%d]\n", inet_ntoa(client.sin_addr), client.sin_port);
 
		pthread_t thread;
		int ret = 0;
		if (ret = pthread_create(&thread, NULL, handle_accept, &connfd)) {
			printf("pthread_create error\n");
			continue;
		}
		thread_list.push_back(thread);
	}
    
	close(listenfd);

	for (std::list<pthread_t>::iterator iter = thread_list.begin();
			iter != thread_list.end(); ++iter) {
		pthread_join(*iter, NULL);
	}

	return 0;
}

void *handle_accept(void *arg)
{
	int connfd = *(int *)arg;
	char buff[4096] = {0};
	while (1) {
		int n = 0; 
		n = recv(connfd, buff, cfg_maxline, 0);
        if (n < 0)
        {
            printf("handle_accept() recv error!\n");
            break;
        }
		else if (n == 0)// 断开
        {
            printf("handle_accept() client exit!\n");
            break;
        }
		
		buff[n] = '\0';
    	printf("recv msg from client: %s\n", buff);

		if (send(connfd, buff, strlen(buff), 0) < 0){
			break;
		}
    }
	close(connfd);
}
