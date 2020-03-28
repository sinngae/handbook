/* @desc:   show errno and errmsg
 */
#include <stdio.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    int i = 0;
    ++i;
    printf("errno:%ld, errmsg:%s\n", errno, strerror(errno));

    return 0;
}
