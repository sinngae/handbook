CC=/usr/bin/g++ -g -Wall
INC_CFG=-I. \
		-I/opt/local/log4cplus/include 

LIB_CFG=-L/opt/local/log4cplus/lib -llog4cplus

OBJS=test.o

all:test

test:$(OBJS)
	$(CC) -o test $(OBJS) $(LIB_CFG)

test.o:test.cpp
	$(CC) -c test.cpp $(INC_CFG)

.PHONY:clean
clean:
	rm -rf *.o
	rm -rf test
