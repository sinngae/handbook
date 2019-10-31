/* @desc:   handle_sigbus
 *  mmap时访问截短的文件，观察是否收到SIGBUS信号
 *  echo 12345 > aaa.txt 
 *  ./a.out
 *  另一终端运行 echo 1 > aaa.txt
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>

void handle_sigbus(int sig)
{
    printf("SIGBUS!\n");
    _exit(0);
}

int main()
{
    int i;
    char *p, tmp;
    struct stat st;
    int fd = -1;

    fd = open("aaa.txt", O_RDWR);
    stat("aaa.txt", &st);
    p = (char*)mmap(NULL, st.st_size, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
    signal(SIGBUS, handle_sigbus);

    getchar();

    for (i = 0; i < st.st_size; i++) {
        tmp = p[i];
        printf("%x\n", tmp);
    }

    printf("ok\n");

    return 0;
}

