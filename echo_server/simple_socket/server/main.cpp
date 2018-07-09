#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define INI_ADDR "127.0.0.1"
#define INI_PORT 2370
#define INI_MAXLINE 4096
#define INI_TRDCNT 10






int main(int argc, char *argv[])
{
	int listenfd, connfd;
	struct sockaddr_in servaddr;
	char buff[4096];
    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {
	    printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
	    exit(0);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(INI_PORT);

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
    int n = 0; 
	while(1) {
        if( (connfd = accept(listenfd, (struct sockaddr *)&client, &socklen)) == -1) {
	        printf("accept socket error: %s(errno: %d)", strerror(errno), errno);
	        return -1;
        }
        printf("client on [ip:%s, port:%d]\n", inet_ntoa(client.sin_addr), client.sin_port);
        
        while (1) {
		    n = recv(connfd, buff, INI_MAXLINE, 0);
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
