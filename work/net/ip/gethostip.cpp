/*************************************************************************
	> File Name: gethostip.cpp
	> Author: 
	> Mail: 
	> Created Time: Wed 03 Jan 2018 02:36:57 AM EST
 ************************************************************************/

#include<iostream>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

using namespace std;

bool GetLocalIP(char * pszIPBuf)
{
	#define MAXINTERFACES 16 

	bool result = false;

	int fd = -1;
	int intrface = 0; 
	struct ifreq buf[MAXINTERFACES]; 
	struct ifconf ifc; 

	if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) >= 0) 
	{ 
		ifc.ifc_len = sizeof buf; 
		ifc.ifc_buf = (caddr_t) buf; 
		if (!ioctl (fd, SIOCGIFCONF, (char *) &ifc)) 
		{ 
			//获取接口信息
			intrface = ifc.ifc_len / sizeof (struct ifreq); 
			//根据借口信息循环获取设备IP和MAC地址
			while (intrface-- > 0) 
			{     
				//获取设备名称
				//printf ("net device %s\n", buf[intrface].ifr_name); 

				if ( strstr(buf[intrface].ifr_name,"eth") != NULL )
				{
					//获取当前网卡的IP地址 
					if (!(ioctl (fd, SIOCGIFADDR, (char *) &buf[intrface]))) 
					{ 
						if ( inet_ntop(AF_INET,
									&(( (struct sockaddr_in*)(& (buf[intrface].ifr_addr) ))->sin_addr),
									pszIPBuf,16) )
						{
							return true;
						}
					}
				}
			}
		}
        else
            cout << "hi else" << endl;

		close (fd); 
	}
    else
        cout << "hi, end" << endl;

	return result;
}


int main()
{
	char pszIPBuf[20] = {0};
	GetLocalIP(pszIPBuf);
	if (pszIPBuf[0] != '\0')
		printf("local IP [%s]\n", pszIPBuf);
	else
		printf("Get local IP error!\n");

	return 0;
}
