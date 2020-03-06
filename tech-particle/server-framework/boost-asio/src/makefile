CC=g++
INCLUDE=-I/opt/local/boost/include
LIBRARY=-L/opt/local/boost/lib -lboost_system

all:server

server:main.o
	$(CC) -o server main.o $(LIBRARY)

main.o:main.cpp
	$(CC) -c main.cpp $(INCLUDE)

.PHONY:clean
clean:
	@rm -f server
	@rm -f *.o
