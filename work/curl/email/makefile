CC=g++ -g -std=c++11 -Wall

INCLUDE_DIR=-I/opt/local/curl/include \
			-I/opt/local/mimetic/include

LIB_DIR=-L/opt/local/curl/lib -lcurl \
		-L/opt/local/mimetic/lib -lmimetic

OBJS=client.o

all:client

client:$(OBJS)
	$(CC) -o client $(OBJS) $(LIB_DIR)

client.o:client.cpp
	$(CC) -c client.cpp $(INCLUDE_DIR)

.PHONY:clean
clean:
	rm -rf *.o
	rm -rf client
