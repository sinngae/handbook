CC = gcc -g

INC_DIR = -I. \
	  -I/opt/local/libubox/include \
	  -I/opt/local/ubus/include #\
	  #-I/opt/local/ubus_app/include \
	  #-I/opt/local/config/include

LIB_DIR = -L/opt/local/libubox/lib/ -lblobmsg_json -lubox -ljson_script \
	  -L/opt/local/ubus/lib/ -lubus #\
	  #-L/opt/local/ubus_app/lib/ -lubus_app \
	  -L/opt/local/config/lib/ -lconfig \
	  -ljson-c -lzlog -lpthread -lsqlite3 -lcurl -lblkid -lm

CUR_SOURCE = ${wildcard *.c}
CUR_OBJS = ${patsubst %.c, %.o, $(CUR_SOURCE)}

BIN = server client

all:$(CUR_OBJS) $(BIN)

client:invoke_client.o
	$(CC) -o client invoke_client.o $(LIB_DIR)

server:invoke_server.o
	$(CC) -o server invoke_server.o $(LIB_DIR)

$(CUR_OBJS):%.o:%.c
	$(CC) -c $^ -o $@ $(INC_DIR)

.PHONY:clean
clean:
	rm -rf *.o
	rm -rf $(BIN)

