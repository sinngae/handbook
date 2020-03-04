#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "async_server.c"

#include "server.h"

const unsigned int REQUEST_HAED_SIZE = 4096;

struct s_server_t
{
    /* control info */
    int keep_running;

    int listenfd;
    /* socket info */
    unsigned short port;
};

threadpool g_server_worker;

typedef struct s_client_new_t
{
    struct sockaddr_in sock_addr;
    socklen_t sock_len;
    int connfd;
} client_new_t;

void *accept_handle(void *arg)
{
	int connfd = *(int *)arg;
	char buff[4096] = {0};
    /* will block a worker ? */
	while (1) {
		int n = 0; 
		n = recv(connfd, buff, cfg_maxline, 0);
        if (n < 0) {
            printf("accept_handle() recv error!\n");
            break;
        }
		else if (n == 0) {// 断开
            printf("accept_handle() client exit!\n");
            break;
        }
		/* TODO:post request to async workers */
		buff[n] = '\0';
    	printf("recv msg from client: %s\n", buff);
		if (send(connfd, buff, strlen(buff), 0) < 0) {
			break;
		}
    }
	close(connfd);
}

static int server_listen(server_t server)
{
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
	    printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
	    return -1;
	}

	struct sockaddr_in sock_addr;
	memset(&sock_addr, 0, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	sock_addr.sin_port = htons(server->port);
    int retval = bind(listenfd, (struct sockaddr*)&sock_addr, sizeof(sock_addr));
	if (retval == -1) {
		printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
		return -2;
	}

    retval = listen(listenfd, 10);
	if (retval == -1) {
		printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
		return -3;
	}

    server->listenfd = listenfd;

    return 0;
}

static int server_loop(server_t *server)
{
	while (server->keep_running) {
        client_new_t *client_new = (client_new_t *)malloc(sizeof (client_new_t));
        client_new->sock_len  = sizeof(struct sockaddr);
        client_new->connfd = accept(listenfd, (struct sockaddr *)&(client_new->sock_addr), &(client_new->sock_len));
        if (client_new->connfd == -1) {
	        printf("accept socket error: %s(errno: %d)", strerror(errno), errno);
	        return -1;
        }
        printf("client on [ip:%s, port:%d]\n", inet_ntoa(client_new->sock_addr.sin_addr), client_new->sock_addr.sin_port);

        AsyncTask *task = new_async_task(accept_handle, NULL, NULL, client_new);
        if (async_server_add_task(g_server_worker, task) != 0) {
            delete_async_task(task);
        }
	}
}

int server_exit()
{
	close(listenfd);
}

server_t *new_server(unsigned short port)
{
    server_t *server = (server_t *)malloc(sizeof (server_t));
    memset(server, 0x00, sizeof (server_t));
    //server->keep_running = 1;
    server->port = port;
}

int server_run(server_t *server)
{
    g_server_worker = async_server_init(4); /* why 4? default set */

    server_listen(server);
    server_loop(server);
    server_exit(server);
}
