CC=g++ -g -Wall
INCLUDE_DIR=-I.
LIB_DIR=-lpthread

all:test

test:test.o
	$(CC) -o test test.o $(LIB_DIR)

test.o:test.cpp
	$(CC) -c test.cpp

.PHONY:clean
clean:
	rm -rf *.o
	rm -rf test
