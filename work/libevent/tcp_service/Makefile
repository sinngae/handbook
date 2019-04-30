CC=gcc
CFLAG=
INC=-I/home/local/include
LIB=-L/home/local/lib -levent

all:main

main:main.o
	$(CC) -o main main.o ${LIB}

main.o:main.c
	$(CC) -c main.c ${INC}

.PHONY:
clean:
	rm -rf *.o
	rm -rf main
