CC=g++ -g -Wall
INCLUDE_DIR=-I/usr/include/oracle/10.2.0.5/client64/
LIB_DIR=-L/usr/lib/oracle/10.2.0.5/client64/lib -locci -lclntsh -lnnz10

all:test

test:test.o
	$(CC) -o test test.o $(INCLUDE_DIR) $(LIB_DIR)

test.o:test.cpp
	$(CC) -c test.cpp $(INCLUDE_DIR)

.PHONY:clean
clean:
	rm -rf *.o
	rm -rf test
