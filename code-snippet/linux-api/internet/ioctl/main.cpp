/*************************************************************************
	> File Name: main.cpp
	> Author: renzq
	> Mail: zqren@foxmail.com
	> Created Time: Wed 16 May 2018 06:32:13 PM HKT
 https://segmentfault.com/a/1190000005138358
 ************************************************************************/
#include <iostream>
#include <sstream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <netinet/in.h>
#include <errno.h>

#include <netinet/if_ether.h>
#include <net/if_arp.h>

#define BUFFSIZE 1024

using namespace std;

void list_network_interfaces();
void get_interface_ip_address(const char *ifname);
void get_interface_mac_address(const char *dev);

int main(int argc, char *argv[])
{
    ostringstream oss;
    oss << "v1.0.0.1  build@" << __TIME__<< " " << __DATE__;
    string version = oss.str();
    if (argc > 1 && strcmp(argv[1], "-v") == 0)
    {
        cout << argv[0] << "\t" << version.c_str() << endl;
        return 0;
    }

    char cmd = '*';
    if (argc > 1)
        cmd = argv[1][0];

    switch (cmd)
    {
    case 'a':
        list_network_interfaces();
        break;
    case 'b':
        if (argc > 2)
            get_interface_ip_address(argv[2]);
        break;
    case 'c':
        if (argc > 2)
            get_interface_mac_address(argv[2]);
        break;
    default:
        cout << "usage:"
            << "  -v version\n" 
            << "  a  list_network_interfaces\n"
            << "  b  [ifname] get_interface_ip_address by ifname\n"
            << "  c  [ifname] get_interface_mac_address by ifname\n"
            << "  d"
            << endl;
    }

    return 0;
}

void list_network_interfaces()
{
    int sfd, if_count, i;
    struct ifconf ifc;
    struct ifreq ifr[10];
    char ipaddr[INET_ADDRSTRLEN] = {'\0'};

    memset(&ifc, 0, sizeof(struct ifconf));
    sfd = socket(AF_INET, SOCK_DGRAM, 0);

    ifc.ifc_len = 10 * sizeof(struct ifreq);
    ifc.ifc_buf = (char *)ifr;

    /* SIOCGIFCONF is IP specific. see netdevice(7) */
    ioctl(sfd, SIOCGIFCONF, (char *)&ifc);

    if_count = ifc.ifc_len / (sizeof(struct ifreq));
    for (i = 0; i < if_count; i++) {
    printf("Interface %s : ", ifr[i].ifr_name);    
        inet_ntop(AF_INET, 
            &(((struct sockaddr_in *)&(ifr[i].ifr_addr))->sin_addr),
            ipaddr, INET_ADDRSTRLEN);
        printf("%s\n", ipaddr);
    }
    
    close(sfd);

    exit(EXIT_SUCCESS);
}

void get_interface_ip_address(const char *ifname)
{
    // ifname is device name, as is interface name
    int sfd, saved_errno, ret;
    struct ifreq ifr;
    char *ipaddr;

    ipaddr = (char *)malloc(INET_ADDRSTRLEN);
    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

    sfd = socket(AF_INET, SOCK_DGRAM, 0);

    errno = saved_errno;
    ret = ioctl(sfd, SIOCGIFADDR, &ifr);
    if (ret == -1) {
        if (errno == 19) {
            fprintf(stderr, "Interface %s : No such device.\n", ifname);
            exit(EXIT_FAILURE);
        }
        if (errno == 99) {
            fprintf(stderr, "Interface %s : No IPv4 address assigned.\n", ifname);
            exit(EXIT_FAILURE);
        }
    }
    saved_errno = errno;

    inet_ntop(AF_INET, &(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr), ipaddr, INET_ADDRSTRLEN);
    
    close(sfd);
    printf("Interface %s : %s \n", ifname, ipaddr);
    //return ipaddr;
}

void get_interface_mac_address(const char *dev)
{
    int sfd, ret, saved_errno, i;
    unsigned char *mac_addr;
    struct ifreq ifr;

    mac_addr = (unsigned char *)malloc(ETH_ALEN);

    sfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, dev, IFNAMSIZ);

    saved_errno = errno;
    ret = ioctl(sfd, SIOCGIFHWADDR, &ifr);
    if (ret == -1 && errno == 19) {
        fprintf(stderr, "Interface %s : No such device.\n", dev);
        exit(EXIT_FAILURE);
    }
    errno = saved_errno;

    if (ifr.ifr_addr.sa_family == ARPHRD_LOOPBACK) {
        printf("Interface %s : A Loopback device.\n", dev);
        printf("MAC address is always 00:00:00:00:00:00\n");
        exit(EXIT_SUCCESS);
    }

    if (ifr.ifr_addr.sa_family != ARPHRD_ETHER) {
        fprintf(stderr, "Interface %s : Not an Ethernet device.\n", dev);
        exit(EXIT_FAILURE);
    }

    memcpy(mac_addr, ifr.ifr_hwaddr.sa_data, ETH_ALEN);

    printf("Interface %s : %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n", 
           dev, *mac_addr, *(mac_addr+1), *(mac_addr+2), *(mac_addr+3), *(mac_addr+4), *(mac_addr+5));
    //return (unsigned char *)mac_addr;
}

void get_interface_flags(char *dev)
{
    int s;
    short flags;
    char ifname[IFNAMSIZ] = {'\0'};
    strncpy(ifname, argv[1], IFNAMSIZ-1);

    s = socket(AF_INET, SOCK_DGRAM, 0);

    int saved_errno, ret;
    short if_flags;
    struct ifreq ifr;

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, dev, IFNAMSIZ);

    saved_errno = errno;
    ret = ioctl(s, SIOCGIFFLAGS, &ifr);
    if (ret == -1 && errno == 19) {
        fprintf(stderr, "Interface %s : No such device.\n", dev);
        exit(EXIT_FAILURE);
    }
    errno = saved_errno;
    if_flags = ifr.ifr_flags;

    printf("Interface %s : ", dev);
    if (if_flags & IFF_UP)
        printf("UP ");

    if (if_flags & IFF_RUNNING)
        printf("RUNNING ");

    if (if_flags & IFF_LOOPBACK)
        printf("LOOPBACK ");

    if (if_flags & IFF_BROADCAST)
        printf("BROADCAST ");

    if (if_flags & IFF_MULTICAST)
        printf("MULTICAST ");

    if (if_flags & IFF_PROMISC)
        printf("PROMISC");

#ifndef IFF_LOWER_UP
#define IFF_LOWER_UP 0x10000
    if (if_flags & IFF_LOWER_UP)
        printf("LOWER_UP");
#endif

    printf("\n");

    close(s);
    exit(EXIT_SUCCESS);
    //return if_flags;
}

void set_interface_ip_address()
{
}

void set_interface_ip_address()
{
}

void change_ifname()
{
}
