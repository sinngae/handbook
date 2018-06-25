#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLINE 4096

int main(int argc, char *argv[])
{
	int listenfd, connfd;
	struct sockaddr_in6 servaddr;
	char buff[4096];
    if( (listenfd = socket(AF_INET6, SOCK_STREAM, 0)) == -1 ) {
	    printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
	    exit(0);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin6_family = AF_INET6;
	servaddr.sin6_addr = in6addr_any;
	servaddr.sin6_port = htons(6666);

	if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
		printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	if( listen(listenfd, 10) == -1) {
		printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}
    printf("======waiting for client's request======\n");
    
    struct sockaddr_in6 client;
    socklen_t socklen = sizeof(struct sockaddr);
    char client_ipv6[128];
    int n = 0; 
	while(1) {
        if( (connfd = accept(listenfd, (struct sockaddr *)&client, &socklen)) == -1) {
	        printf("accept socket error: %s(errno: %d)", strerror(errno), errno);
	        return -1;
        }
        printf("client on [ip:%s, port:%d]\n", 
               //inet_ntoa6(client.sin6_addr), 
               inet_ntop(AF_INET6, &client.sin6_addr, client_ipv6, sizeof(client_ipv6)),
               client.sin6_port);
        
        while (1) {
		    n = recv(connfd, buff, MAXLINE, 0);
            if (n > 0)
            {
		        buff[n] = '\0';
    		    printf("recv msg from client: %s\n", buff);
            }
            else if (n == 0)
            {
                break;
            }
            else
            {
                printf("recv error!\n");
                return -1;
            }
        }
	    close(connfd);
	}
    
	close(listenfd);

	return 0;

}
