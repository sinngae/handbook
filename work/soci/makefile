CC=g++ -g -Wall
INCLUDE_DIR=-I/opt/local/mysqlclient/include \
			-I/opt/opensource/soci-3.2.3/core \
			-I/opt/opensource/soci-3.2.3/backends
LIB_DIR=-L/opt/local/mysqlclient/lib -lmysqlclient \
		-L/opt/local/soci/lib64 -lsoci_core -lsoci_mysql

all:test

test:test.o
	$(CC) -o test test.o $(INCLUDE_DIR) $(LIB_DIR)

test.o:test.cpp
	$(CC) -c test.cpp $(INCLUDE_DIR)

.PHONY:clean
clean:
	rm -rf *.o
	rm -rf test
