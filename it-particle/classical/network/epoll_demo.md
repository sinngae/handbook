```c
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define MAX_EVENTS 64
#define PORT 8080
#define BUFFER_SIZE 1024

//设置文件描述符为非阻塞模式
void set_nonblocking(int fd) {
    int flags = fcntl(fd,F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK)；
}

int main() f
    //创建服务器套接字
    int server_fd = socket(AF_INET, SOCK_STREAM, O);
    if (server_fd<0) {
        perror("socket creation failed");
        exit（EXIT_FAILURE);
    }
    //设置SO_REUSEADDR避免端口占用
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    //绑定地址
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(PoRT)，
        .sin_addr.s_addr = INADDR_ANY
    };
    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0){
        perror("bind failed");
        close(server_fd);
        exit（EXIT_FAILURE);
    }

    //开始监听
    if (listen(server_fd, SOMAxcoNN)< 0){
        perror("listen failed");
        close(server_fd);
        exit（EXIT_FAILURE);
    ｝
    printf（"Server listening on port sdIn", PoRT);
    //创建epoll实例
    int epoll_fd = epoll_createl(0);
    if (epoll_fd< 0）{
        perror（"epoll_createl failed");
        close(server_fd);
        exit（EXIT_FAILURE);
    }
    //添加服务器套接字到epoll
    struct epoll_event ev;
    ev.events=EPOLLIN；//监听可读事件(新连接）
    ev.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) < 0){
        perror("epoll_ctl: server_fd");
        close(server_fd);
        close(epoll_fd);
        exit（EXIT_FAILURE);
    }
    //事件存储数组
    struct epoll_event events[MAX_EVENTS];
    //主事件循环
    while (1） {
        //等待事件发生（无限期阻塞）
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1）{
            //处理信号中断
            if (errno == EINTR) continue;
            perror（"epoll_wait failed");
            break;
        }

        // 处理所有就绪事件
        for (int i = 0; i < nfds; i++){
            //1．处理新连接
            if (events[i].data.fd == server_fd) {
                struct sockaddr_in client_addr;
                socklen_t len = sizeof(client_addr);
                //接受新连接
                int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len);
                if (client_fd < 0){
                    perror("accept failed");
                    continue;
                }
                //设置为非阻塞模式
                set_nonblocking(client_fd);

                //添加客户端到epoll
                ev.events=EPOLLIN|EPOLLET； //边缘触发模式
                ev.data.fd = client_fd;

                if (epoll_ctl(epoll_fd,EPOLL_CTL_ADD,client_fd,&ev)< 0){
                    perror("epoll_ctl: client_fd");
                    close(client_fd);
                } else {
                    printf（"New connection: fd=sd(n", client_fd);
                }
            }
            //2．处理客户端数据
            else {
                int client_fd = events[i].data.fd;
                // 检查连接关闭或错误
                if (events[i].events & （EPOLLERR | EPOLLHUP| EPOLLRDHUP)）{
                    printf（"Connection closed:fd=%d(n", client_fd);
                    epoll_ctl(epoll_fd,EPOLL_CTL_DEL,client_fd, NULL);
                    close(client_fd);
                    continue;
                }
                // 处理可读事件（边缘触发模式）
                if (events[i].events & EPOLLIN){
                    char buffer[BUFFER_SIZE];
                    ssize_t bytes_read;

                    //ET模式必须循环读取直到EAGAIN
                    while (1） {
                        bytes_read = read(client_fd, buffer, sizeof(buffer));
                        if (bytes_read > 0) {
                            //简单回显处理
                            write(client_fd, buffer, bytes_read);
                        }
                        else if (bytes_read== 0) {
                            //对端关闭连接
                            printf("Client closed connection:fd=sd(n", client_fd);
                            epoll_ctl(epoll_fd,EPOLL_CTL_DEL,client_fd,NULL);
                            close(client_fd);
                            break;
                        }
                        else {
                            //处理错误或非阻塞返回
                            if (errno ==EAGAIN || errno== EWOULDBLOCK){
                                //数据已读完，等待下次事件
                                break;
                            }

                            //真实错误
                            perror("read error");
                            epoll_ctl(epoll_fd, EPOLL_CTL_DEL,client_fd, NULL);
                            close(client_fd);
                            break;
                        }
                    }
                }
            }
        }
    }

    // 清理资源
    close(server_fd);
    close(epoll_fd);
    return 0;
}
```