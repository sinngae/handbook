CC=g++ -g -Wall

OBJ=getopt signal

all:test main

test:test.o
	$(CC) -o test test.o

test.o:test.cpp
	$(CC) -c test.cpp

main:main.o
	$(CC) -o main main.o

main.o:main.cpp
	$(CC) -c main.cpp

.PHONY:clean
clean:
	rm -rf *.o
	rm -rf test main
