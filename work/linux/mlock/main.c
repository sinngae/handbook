#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

const unsigned int mem_size = 2<<20; // 2MB

int main(int argc, char *argv[])
{
    int count = atoi(argv[1]);
    int i = 0;
    for (; i < count; ++i) {
        char *memory = malloc(mem_size);
        if (mlock(memory, mem_size) == -1) {
            perror("mlock failed\n");
            return -1;
        }
        perror("mlock 2mb\n");
        sleep(1);
    }

    return 0;
}
