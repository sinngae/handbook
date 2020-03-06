#include <stdio.h>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[])
{
    for (int i = 0; i < 2; i++)
    {
        //fork();
        //printf("A:%d", getpid());
        fork();
        printf("A:%d\n", getpid());
    }

    return 0;
}
