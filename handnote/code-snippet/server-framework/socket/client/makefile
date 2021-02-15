CC=g++ -g -Wall

all:client

server:server.o
	$(CC) -o server server.o

server.o:server.cpp
	$(CC) -c server.cpp

client:client.o
	$(CC) -o client client.o

client.o:client.cpp
	$(CC) -c client.cpp

.PHONY:clean
clean:
	rm -rf *.o
	rm -rf server client
