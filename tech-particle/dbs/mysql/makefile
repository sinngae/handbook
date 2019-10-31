CC=g++ -g -Wall
INCLUDE_DIR=-I/opt/local/mysqlclient/include
LIB_DIR=-L/opt/local/mysqlclient/lib -lmysqlclient

all:test

test:test.o
	$(CC) -o test test.o $(INCLUDE_DIR) $(LIB_DIR)

test.o:test.cpp
	$(CC) -c test.cpp $(INCLUDE_DIR)
