#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>
#include <netdb.h>

struct epoll_event  *events = NULL;
int epollFd = -1;
const int MAX_SOCK_NUM = 1024;

int epoll_init();
int epoll_socket(int domain, int type, int protocol);
int epoll_cleanup();
int epoll_new_conn(int sfd);

int main()
{
    struct sockaddr_in listenAddr;
    int listenFd = -1;

    if (-1 == epoll_init())
    {
        printf("epoll_init err\n");
        return -1;
    }

    if ((listenFd = epoll_socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        printf("epoll_socket err\n");
        epoll_cleanup();
        return -1;
    }

    listenAddr.sin_family = AF_INET;
    listenAddr.sin_port = htons(999);
    listenAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (-1 == bind(listenFd,(struct sockaddr*)&listenAddr,sizeof(listenAddr)))
    {
        printf("bind err %d\n",errno);
        epoll_cleanup();
        return -1;
    }

    if (-1 == listen(listenFd,1024))
    {
        printf("listen err\n");
        epoll_cleanup();
        return -1;
    }

    //Add ListenFd into epoll
    if(-1 == epoll_new_conn(listenFd))
    {
        printf("eph_new_conn err\n");
        close(listenFd);
        epoll_cleanup();
        return -1;
    }

    //LOOP
    while(1)
    {
        int n;
        n = epoll_wait(listenFd,events,MAX_SOCK_NUM,-1);
        for (int i = 0; i < n; i++)
        {
            if ( (events[i].events & EPOLLERR) || ( events[i].events & EPOLLHUP ) || !(events[i].events & EPOLLIN) )
            {
                printf("epoll err\n");
                close(events[i].data.fd);
                continue;
            }
            else if (events[i].data.fd == listenFd)
            {
                while (1)
                {
                    struct sockaddr inAddr;
                    char hbuf[1024],sbuf[NI_MAXSERV];
                    socklen_t inLen = -1;
                    int inFd = -1;
                    int s = 0;
                    int flag = 0;

                    inLen = sizeof(inAddr);
                    inFd = accept(listenFd,&inAddr,&inLen);
                    if (inFd == -1)
                    {
                        if ( errno == EAGAIN || errno == EWOULDBLOCK )
                        {
                            break;
                        }
                        else
                        {
                            printf("accept error\n");
                            break;
                        }
                    }

                    if (s ==  getnameinfo (&inAddr, inLen, hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV)) 
                    {
                        printf("Accepted connection on descriptor %d (host=%s, port=%s)\n", inFd, hbuf, sbuf);
                    }
                    //Set Socket to non-block
                    if ((flag = fcntl(inFd,F_GETFL,0)) < 0 || fcntl(inFd,F_SETFL,flag | O_NONBLOCK) < 0)
                    {
                        close(inFd);
                        return -1;
                    }
                    epoll_new_conn(inFd);
                }
            }
            else
            {
                while (1) 
                {
                    ssize_t count;
                    char buf[512];
                    count = read(events[i].data.fd, buf, sizeof buf);
                    if (count == -1) 
                    {
                        if (errno != EAGAIN)
                        { 
                            printf("read err\n");
                        }
                        break;
                    } 
                    else if (count == 0) 
                    {  
                        break;
                    }
                    write (1, buf, count); 
                }
            }
        }
    }

    epoll_cleanup();
}
 
 
int epoll_init()
{
    if(!(events = (struct epoll_event* ) malloc ( MAX_SOCK_NUM * sizeof(struct epoll_event))))
    {
        return -1;
    }
 
    if( (epollFd = epoll_create(MAX_SOCK_NUM)) < 0 )
    {
        return -1;
    }
 
    return 0;
}
 
int epoll_socket(int domain, int type, int protocol)
{
    int sockFd = -1;
    int flag = -1;
 
    if ((sockFd = socket(domain,type,protocol)) < 0)
    {
        return -1;
    }
 
    //Set Socket to non-block
    if((flag = fcntl(sockFd,F_GETFL,0)) < 0 || fcntl(sockFd,F_SETFL,flag | O_NONBLOCK) < 0)
    {
        close(sockFd);
        return -1;
    }
 
    return sockFd;
}
 
int epoll_cleanup()
{
    free(events);
    close(epollFd);
    return 0;
}
 
int epoll_new_conn(int sfd)
{
    struct epoll_event  epollEvent;
    memset(&epollEvent, 0, sizeof(struct epoll_event));
    epollEvent.events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLET;
    epollEvent.data.ptr = NULL;
    epollEvent.data.fd  = sfd;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, sfd, &epollEvent) < 0)
    {
      return -1;
    }
    
    epollEvent.data.fd  = sfd;
    
    return 0;
}
