#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
 
#pragma pack(1)
struct packet{
    unsigned char enable;
    int data;
};
#pragma pack()
 
void bad_sig_handle(int sig)
{
    printf("rcv signal: %d\n", sig);
}
 
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
 
    signal(SIGBUS, bad_sig_handle);
 
    pkt.data = 12;
 
    printf("pkt.data = %d\n", pkt.data);
 
    return 0;
}
