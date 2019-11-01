#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    char buf[] = "i am write process\n";
    int fd = open("my_fifo",O_WRONLY);
    write(fd,buf,strlen(buf));
    close(fd);
    return 0;
}
