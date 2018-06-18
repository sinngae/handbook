#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

using namespace std;

#define MAXLINE 4096

int main(int argc, char *argv[])
{
	int sockfd;
//    int n;
//	char recvline[4096];
    char sendline[4096];
	struct sockaddr_in servaddr;

    string addr = "127.0.0.1";
	if( argc > 1){
        addr = argv[1];
	}

	if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
		exit(0);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(2370);
	if( inet_pton(AF_INET, addr.c_str(), &servaddr.sin_addr) <= 0){
		printf("inet_pton error for %s\n", addr.c_str());
	    exit(0);
	}

	if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
		printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
	    exit(0);
	}

    while(1)
    {
	    printf("send msg to server: \n");
    	fgets(sendline, 4096, stdin);
	    if( send(sockfd, sendline, strlen(sendline), 0) < 0) {
		    printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
    		exit(0);
        }    
	}
        
	close(sockfd);
	
	return 0;
}
