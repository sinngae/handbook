#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <string.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

int main(int argc, char *argv[]) 
{
    char inotify_path[1024] = "/home";
    if (argc >= 2) {
        strncpy(inotify_path, argv[1], 1024);
    }

    struct pollfd pollfds[1];
    pollfds[0].fd = inotify_init();
    pollfds[0].events = POLLIN;
    if ( pollfds[0].fd < 0 ) {
        perror("inotify_init");
        exit(1);
    }

    int wd = inotify_add_watch(pollfds[0].fd, inotify_path, IN_MODIFY|IN_CREATE|IN_DELETE|IN_MOVE|IN_CLOSE_WRITE);
    if (wd == -1) {
        perror("inotify_add_watch");
        exit(1);
    }

    int length, i = 0;
    char buffer[BUF_LEN];
    while (1) {
        length = poll(pollfds, 1, 1000); // 
        if (length == 0) {
            printf( "timeout time:%ld\n", time(NULL));
            continue;
        }
        else if (length < 0) {
            if ((errno == EINTR) || (errno == EAGAIN))
              continue;
            printf( "poll read failed errno:%d\n", errno);
            continue; // break?
        }

        length = read(pollfds, buffer, BUF_LEN);
        buffer[BUF_LEN-1] = '\0';
        printf( "read() length:%d\n", length);       
        if ( length < 0 ) {
            perror("read");
            exit(1);
        }  

        while (i < length) {
            struct inotify_event *event = (struct inotify_event *)&buffer[i];
            if (event->len) {
                if ( event->mask & IN_CREATE ) {
                    if ( event->mask & IN_ISDIR ) {
                        printf( "The directory %s was created.\n", event->name );       
                    }
                    else {
                        printf( "The file %s was created.\n", event->name );
                    }
                }
                else if ( event->mask & IN_DELETE ) {
                    if ( event->mask & IN_ISDIR ) {
                        printf( "The directory %s was deleted.\n", event->name );       
                    }
                    else {
                        printf( "The file %s was deleted.\n", event->name );
                    }
                }
                else if ( event->mask & IN_MODIFY ) {
                    if ( event->mask & IN_ISDIR ) {
                        printf( "The directory %s was modified.\n", event->name );
                    }
                    else {
                        printf( "The file %s was modified.\n", event->name );
                    }
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }

    ( void ) inotify_rm_watch( fd, wd );
    ( void ) close( fd );

    exit( 0 );
}
