CC=g++ -std=c++11 -g -Wall
INCLUDE_DIR=-I./ -I/opt/local/thrift/include/ -I./gen-cpp/ -I/opt/local/boost_1_59/include/ 
LIB_DIR=-L/opt/local/thrift/lib -lthrift -lthriftnb -lthriftz -levent -L/opt/local/boost_1_59/lib -lboost_system
OBJS=myserver_constants.o myserver_types.o MyServer.o

all:server client

server:server.o $(OBJS)
	$(CC) -o server server.o $(OBJS) $(INCLUDE_DIR) $(LIB_DIR)

server.o:server.cpp 
	$(CC) -c $< -o $@ $(INCLUDE_DIR)

client:client.o $(OBJS)
	$(CC) -o client client.o $(OBJS) $(INCLUDE_DIR) $(LIB_DIR)

client.o:client.cpp
	$(CC) -c $< -o $@ $(INCLUDE_DIR)

myserver_constants.o:./gen-cpp/myserver_constants.cpp
	$(CC) -c $< -o $@ $(INCLUDE_DIR)

MyServer_server.skeleton.o:./gen-cpp/MyServer_server.skeleton.cpp
	$(CC) -c $< -o $@ $(INCLUDE_DIR)

MyServer.o:./gen-cpp/MyServer.cpp 
	$(CC) -c $< -o $@ $(INCLUDE_DIR)

myserver_types.o:./gen-cpp/myserver_types.cpp 
	$(CC) -c $< -o $@ $(INCLUDE_DIR)

.PHONY:clean
clean:
	rm -rf *.o
	rm -rf server client
