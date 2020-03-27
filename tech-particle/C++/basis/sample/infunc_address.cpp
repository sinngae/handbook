#include <stdio.h>

using namespace std;

char *ret_array()
{
    char a[] = "hi";
    return a;
}

char *ret_point()
{
    char *p = "hi";
    return p;
}


int main()
{
    char *astr = ret_array();
    char *pstr = ret_point();
    printf("%p\n", ret_array());
    printf("%p\n", pstr);

    return 0;
}
