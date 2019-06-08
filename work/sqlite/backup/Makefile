CC=gcc
CFLAG=
INC=-I/home/local/include
LIB=-L/home/local/lib -lsqlite3

CUR_SOURCE = $(wildcard *.c)
CUR_OBJS = $(patsubst %.c, %.o, $(CUR_SOURCE))

all:main

main:$(CUR_OBJS)
	$(CC) -o main $(CUR_OBJS) ${LIB}

$(CUR_OBJS):%.o:%.c
	$(CC) -c $^ -o $@

.PHONY:
clean:
	rm -rf *.o
	rm -rf main
