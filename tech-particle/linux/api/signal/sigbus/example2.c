#include <stdlib.h>
 
#pragma pack(1)
struct packet{
    unsigned char enable;
    int data;
};
#pragma pack()
 
int main(int argc, char **argv) 
{
 
#if defined(__GNUC__)
# if defined(__i386__)
    /* Enable Alignment Checking on x86 */
    __asm__("pushf\norl $0x40000,(%esp)\npopf");
# elif defined(__x86_64__) 
    /* Enable Alignment Checking on x86_64 */
    __asm__("pushf\norl $0x40000,(%rsp)\npopf");
# endif
#endif
 
    struct packet pkt;
    pkt.data = 12;
 
    return 0;
}
