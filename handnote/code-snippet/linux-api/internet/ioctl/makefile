CC=/usr/bin/g++ -g -Wall

all:test

test:main.o
	$(CC) -o test main.o

main.o:main.cpp
	$(CC) -c main.cpp

.PHONY:clean
clean:
	rm -rf test
	rm -rf *.o
