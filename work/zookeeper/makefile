CC = /usr/bin/g++ -g -Wall
INC_DIR = -I. -I/opt/local/log4cplus/include/ \
		  -I/opt/local/zookeeper/include \
		  -I/opt/local/boost/include

LIB_DIR = -L/opt/local/log4cplus/lib -llog4cplus \
		  -L/opt/local/boost/lib -lboost_chrono \
		  -lboost_date_time -lboost_filesystem -lboost_reget \
		  -lboost_thread -lboost_timer -lboost_serialization 

OBJS = main.o zookeeper_client.o

all:test

test:$(OBJS)
	$(CC) -o test $(OBJS) $(LIB_DIR)

main.o:main.cpp
	$(CC) -c main.cpp $(INC_DIR)

zookeeper_client.o:./src/zookeeper_client.cpp 
	$(CC) -c ./src/zookeeper_client.cpp $(INC_DIR)

zookeeper_client_safe.o:./src/zookeeper_client_safe.cpp 
	$(CC) -c ./src/zookeeper_client_safe.cpp $(INC_DIR)

.PHONY:clean
clean:
	-rm -f *.o
	-rm test
