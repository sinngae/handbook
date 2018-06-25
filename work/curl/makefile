CC=g++ -g -Wall
INCLUDE_DIR=-I/opt/local/curl/include 
LIB_DIR=-L/opt/local/curl/lib -lcurl

all:client

client:client.cpp srvc_demo.o server_types.o 
	$(CC) -o client client.cpp srvc_demo.o server_types.o $(INCLUDE_DIR) $(LIB_DIR)

srvc_demo.o:./gen-cpp/srvc_demo.cpp 
	$(CC) -c ./gen-cpp/srvc_demo.cpp $(INCLUDE_DIR)

server_types.o:./gen-cpp/server_types.cpp 
	$(CC) -c ./gen-cpp/server_types.cpp $(INCLUDE_DIR)

.PHONY:clean
clean:
	rm -rf *.o
	rm -rf server client
