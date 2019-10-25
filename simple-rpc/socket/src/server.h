/************************************************
  > File Name: server.h
  > Author: Philo
  > Mail: 
  > Created Time: Thu 24 Oct 2019 07:39:09 PM CST
*************************************************/
#ifndef MY_SERVER_H
#define MY_SERVER_H

typedef struct s_server_t server_t;

/* @desc:   create connection, the minist rpc server
 */
int server_init(unsigned short port);

int server_run();

int server_destory();

#endif // end of  MY_SERVER_H
