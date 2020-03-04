/*************************************************************************
	> File Name: myserver_handler.h
	> Author: renzq
	> Mail: zqren@foxmail.com
	> Created Time: Tue 29 May 2018 07:16:46 PM HKT
 ************************************************************************/

#ifndef _MYSERVER_HANDLER_H
#define _MYSERVER_HANDLER_H


#include "gen-cpp/MyServer.h"

using namespace  ::myserver;

class MyServerHandler : virtual public MyServerIf {
 public:
  MyServerHandler() {
    // Your initialization goes here
  }

  void login(Response& _return, const LoginRequest& request) {
    // Your implementation goes here
    printf("%s login\n", request.username.c_str());
    _return.ret_code = RetCode::OK;
    _return.ret_msg = "hi, thrift service";
  }

  void logout(Response& _return, const Request& request) {
    // Your implementation goes here
    printf("logout\n");
  }

};

#endif
