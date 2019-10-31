#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#include "log_print.h"

#define LISTENQ 20
#define INFTIM 1000

/* servert_t */
struct s_server_t 
{
    /* control info */
    int keep_running;

    /* socket info */
    unsigned short port;
    int sockfd;
    int epollfd;
};

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

/* 仅做调试用 */
void print_sockname(int sock)
{
#ifdef JUST_DEBUG
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
#endif
}

int server_listen(server_t *server, unsigned short port)
{
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd <= 0) {
	    printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
        return -1;
    }
    /* 把socket设置为非阻塞方式 */
    setnonblocking(listenfd);

	struct sockaddr_in serveraddr;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_aton(LOCAL_HOST, &(serveraddr.sin_addr));
	serveraddr.sin_port = htons(port);

	socklen_t sock_len;
	if (bind(listenfd, (sockaddr*)&serveraddr, &sock_len) < 0) {
		printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
        return -2;
    }

	if (listen(listenfd, 5) < 0) { /* 5是未被accepted的待连接队列容量，满之后，将拒绝客户端连接请求 */
		printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
        return -3;
    }

    server->port = port;
    server->sockfd = listenfd;
    return 0;
}

static void accept_handle(server_t *server)
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int connfd = accept(server->sockfd, (struct sockaddr *)&client_addr, &client_len);
    if (connfd == -1) {
        LOG_PRINT(EN_LOG_ERROR, "accept failed, errno:%d, error:%s", errno, strerror(errno));
        return;
    }
    print_sockname(connfd);
    setnonblock(connfd);

    struct epoll_event ev;
    ev.data.fd = connfd;
    ev.events = EPOLLIN | EPOLLET;
    int retval = epoll_ctl(server->epollfd, EPOLL_CTL_ADD, connfd, &ev);
    if (retval != 0) {
        LOG_PRINT(EN_LOG_ERROR, "accept failed, errno:%d, error:%s", errno, strerror(errno));
        return;
    }
}

/* client_t */
struct s_client_t 
{
    int sockfd;
    int epollfd;
};

static void read_handle(void *data)
{
    client_t *client = (client_t *)data;
	char buff[1024] = {0};
    int readlen = read(client->sockfd, buff, sizeof(buff));
    if (readlen < 0) { /* readline error */
        //if (errno == ECONNRESET) { /* ECONNRESET */
        close(sockfd);
        return ;
    } 
    else if (readlen == 0) { /* read 0, client close */
        close(sockfd);
        return ;
    }
    else {
        char *data = (char *)malloc(1024);
        snprintf(data, 1024, "recv: %s", buff);
        
        struct epoll_event ev;
        ev.data.fd = sockfd;
        ev.events = EPOLLOUT | EPOLLET;
        ev.data.ptr = data;
        epoll_ctl(client->epollfd, EPOLL_CTL_MOD, sockfd, &ev);
    }
}

static void write_handle(void *data)
{
    client_t *client = (client_t *)data;
    int wrlen = write(client->sockfd, client->data, client->len);

    struct epoll_event ev;
    ev.data.fd = sockfd;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, sockfd, &ev);
}

const char LOCAL_HOST[] = "127.0.0.1";
void server_loop(server_t *server)
{
	int epollfd = epoll_create(256); /* 256无实际意义，大于0即可 */
    if (epollfd == -1) {
	    printf("create epoll error: %s(errno: %d)\n", strerror(errno), errno);
	    return -1;
    }

	struct epoll_event ev;
	ev.data.fd = server->listenfd;
	ev.events = EPOLLIN | EPOLLET; /* EPOLLIN - 新用户连接； EPOLLET - 边缘触发 */
	int retval = epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);
    if (retval == -1) {
	    printf("epoll ctl error: %s(errno: %d)\n", strerror(errno), errno);
	    return -1;
    }
    server->epollfd = epollfd;

	for (server->keep_running) {
        const int event_size = 20;
        struct epoll_event events[event_size]; /* 大于0即可。对性能的影响？ */
        int timeout = 500; /* >0 - ms to wait; <0 - 永远等待; =0 - 立即返回 */
        int nfds = epoll_wait(epollfd, events, event_size, timeout);
        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == server->listenfd) { /* new client connect */
                AsyncTask *task = new_async_task(accept_handle, NULL, default_tkdestroyer, server);
                if (async_server_add_task(g_server_worker, task) != 0) {
                    delete_async_task(&task);
                }
            } 
            else if (events[i].events & EPOLLET) { /* 边缘事件 */
                client_t *client = (client_t *)malloc(sizeof(client_t));
                client->sockfd = events[i].data.fd;
                client->epollfd = epollfd;

                AsyncTask *task = new_async_task(read_handle, NULL, read_tkdestroyer, client);
                if (async_server_add_task(g_server_worker, task) != 0) {
                    delete_async_task(&task);
                }
            }
            else if (events[i].events & EPOLLOUT) { /* EPOLLOUT */
                client_t *client = (client_t *)malloc(sizeof(client_t));
                client->sockfd = events[i].data.fd;
                client->data = events[i].data.ptr;
                client->epollfd = epollfd;

                AsyncTask *task = new_async_task(write_handle, NULL, client_destroyer, client);
                if (async_server_add_task(g_server_worker, task) != 0) {
                    delete_async_task(&task);
                }
            }
        }
    }
}

void server_exit(server_t *server)
{
	close(server->listenfd);
}

int server_run(int port)
{
    g_server_worker = async_server_init(4);

    server_t server;
    server_listen(&server, port);

    server_loop(&server);

    server_exit(&server);
}
