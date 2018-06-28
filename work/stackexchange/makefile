CC = g++ -std=c++11

all:test

test:test.o
	$(CC) -o test test.o

test.o:test.cpp
	$(CC) -c test.cpp

.PHONY:clean
clean:
	rm test test.o
