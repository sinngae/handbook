//无名管道
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int arg, char * args[])
{
    //定义文件描述符数组
    int fdarr[2] = { 0 };
    int no = 0;
    int status;
    //create the conduit  创建一个管道 并且打开两个文件描述符
    //管道中，第一个文件描述符只读,第二个文件描述符只写
    no = pipe(fdarr);
    if (no == -1)
    {
        printf("pipe() is failed ! message :%s\n", strerror(errno));
        return -1;
    }
    //创建父子进程
    pid_t child = fork();
    if (child == -1)
    {
        printf("system is game over !\n");
        return -1;
    }
    //定义缓存字符串数组
    char buf[100] = { 0 };
    if (child == 0)
    {
        /*
         管道和文件一样，文件read函数以O_RDONLY方式打开也会阻塞，但是文件数据在本地，读取非常快，感觉不到阻塞，
         但是管道以O_RDONLY方式打开，会阻塞进程,read()函数会等待管道另一端写入数据，直到另一端关闭文件描述符
         */
        //关闭子进程中的写文件描述符--对于父子进程共享文件描述符，只在单个进程中关闭，只能将文件描述符引用减一，
        //因为父子进程中，文件描述符被引用了两次，所以需要在父子进程中分别关闭，才能使文件描述符引用次数减一
        close(fdarr[1]);
        while (read(fdarr[0], buf, sizeof(buf)) > 0)
        {
            printf("%s", buf);
            //清空缓存区
            memset(buf, 0, sizeof(buf));
        }
        //关闭子进程中读文件描述符
        close(fdarr[0]);
    } else
    {
        //关闭父进程中的读描述符
        close(fdarr[0]);
        //将键盘输入数据写入到管道中
        strcpy(buf,"fly on air!\n");
        write(fdarr[1], buf, strlen(buf));
        //关闭管道写文件描述符
        close(fdarr[1]);
        //等待子进程结束
        wait(&status);
        printf("child process is close ! message :%d\n", WEXITSTATUS(status));
    }
    return 10;
}
