CC=g++ -std=c++11 -g -Wall
INCLUDE_DIR=-I./ -I/opt/local/thrift/include/ -I./gen-cpp/ -I/opt/local/boost_1_59/include/ 
LIB_DIR=-L/opt/local/thrift/lib -lthrift -lthriftnb -lthriftz -levent -L/opt/local/boost_1_59/lib -lboost_system
OBJS=main.o myserver_handler.o myserver_constants.o myserver_types.o MyServer.o

all:server client

server:$(OBJS)
	$(CC) -o server $(OBJS) $(INCLUDE_DIR) $(LIB_DIR)

main.o:main.cpp 
	$(CC) -c $< -o $@ $(INCLUDE_DIR)

myserver_handler.o:myserver_handler.cpp
	$(CC) -c $< -o $@ $(INCLUDE_DIR)

myserver_constants.o:./gen-cpp/myserver_constants.cpp
	$(CC) -c $< -o $@ $(INCLUDE_DIR)

MyServer.o:./gen-cpp/MyServer.cpp 
	$(CC) -c $< -o $@ $(INCLUDE_DIR)

myserver_types.o:./gen-cpp/myserver_types.cpp 
	$(CC) -c $< -o $@ $(INCLUDE_DIR)

.PHONY:clean
clean:
	rm -rf *.o
	rm -rf server client
