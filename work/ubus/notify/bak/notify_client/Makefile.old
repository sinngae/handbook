CC = gcc -g

INC_DIR = -I. \
	  -I/opt/local_bk/libubox/include \
	  -I/opt/local_bk/ubus/include #\
	  #-I/opt/local_bk/ubus_app/include \
	  #-I/opt/local_bk/config/include

LIB_DIR = -L/opt/local_bk/libubox/lib/ -lblobmsg_json -lubox -ljson_script \
	  -L/opt/local_bk/ubus/lib/ -lubus #\
	  #-L/opt/local_bk/ubus_app/lib/ -lubus_app \
	  -L/opt/local_bk/config/lib/ -lconfig \
	  -ljson-c -lzlog -lpthread -lsqlite3 -lcurl -lblkid -lm

CUR_SOURCE = ${wildcard *.c}
CUR_OBJS = ${patsubst %.c, %.o, $(CUR_SOURCE)}

BIN = server client

all:$(CUR_OBJS) $(BIN)

client:notify_client.o
	$(CC) -o client notify_client.o $(LIB_DIR)

server:notify_server.o
	$(CC) -o server notify_server.o $(LIB_DIR)

$(CUR_OBJS):%.o:%.c
	$(CC) -c $^ -o $@ $(INC_DIR)

.PHONY:clean
clean:
	rm -rf *.o
	rm -rf $(BIN)

