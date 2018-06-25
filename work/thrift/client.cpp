/*************************************************************************
	> File Name: client.cpp
	> Author: 
	> Mail: 
	> Created Time: Wed 03 Jan 2018 03:13:50 AM EST
 ************************************************************************/

#include <iostream>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <boost/shared_ptr.hpp>

#include "gen-cpp/MyServer.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace myserver;

int main(int argc, char *argv[])
{
    stdcxx::shared_ptr<TSocket> socket(new TSocket("localhost", 9090));
    stdcxx::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    //boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    stdcxx::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

    transport->open();

    MyServerClient client(protocol);
    LoginRequest req;
    req.base.msg_id = 1L;
    req.base.session_id = 1L;
    req.base.func = FuncCode::LOGIN;
    req.username = "Marina";
    req.password = "123456";

    Response resp;
    client.login(resp, req);
    cout << resp.msg_id << endl;
    cout << resp.session_id << endl;
    cout << resp.func << endl;
    cout << resp.ret_code << endl;
    cout << resp.ret_msg << endl;

    transport->close();

    return 0;
}
