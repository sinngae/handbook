CC=g++ -g -Wall -std=c++11 -pthread

all:server 

server:main.o
	$(CC) -o server main.o

main.o:main.cpp
	$(CC) -c main.cpp

.PHONY:clean
clean:
	rm -rf *.o server
