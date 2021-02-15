CC=g++ -g -Wall
INCLUDE_DIR=-I/opt/local/fastcgipp/include
LIB_DIR=-L/opt/local/fastcgipp/lib -lfastcgipp

all:test

test:test.o
	$(CC) -o test test.o $(INCLUDE_DIR) $(LIB_DIR)

test.o:test.cpp
	$(CC) -c test.cpp $(INCLUDE_DIR)

.PHONY:clean
clean:
	rm -rf test
	rm -rf *.o
