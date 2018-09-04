#!/bin/bash
gcc -c invoke_client.c -I. -I/opt/local/libubox/include -I/opt/local/ubus/include
gcc -o client invoke_client.o -L/opt/local/libubox/lib/ -lblobmsg_json -lubox -ljson_script -L/opt/local/ubus/lib/ -lubus

gcc -c invoke_server.c -I. -I/opt/local/libubox/include -I/opt/local/ubus/include
gcc -o server invoke_server.o -L/opt/local/libubox/lib/ -lblobmsg_json -lubox -ljson_script -L/opt/local/ubus/lib/ -lubus

