CC=gcc -g -Wall
INCLUDE_DIR=-I.
LIB_DIR=-lpthread

all:test

test:main.o
	$(CC) -o test main.o $(LIB_DIR)

main.o:main.c
	$(CC) -c main.c

.PHONY:clean
clean:
	rm -rf *.o test
