CC = g++ -g -Wall
INCLUDE_DIR = -I/opt/local/boost/include
LIB_DIR = -L/opt/local/boost/lib -lboost_system -lboost_date_time -lboost_timer -lboost_filesystem

all:ptime main

ptime:ptime.o
	$(CC) -o ptime ptime.o $(LIB_DIR)

ptime.o:ptime.cpp
	$(CC) -c ptime.cpp $(INCLUDE_DIR)

main:main.o
	$(CC) -o main main.o $(LIB_DIR)

main.o:main.cpp
	$(CC) -c main.cpp $(INCLUDE_DIR)

.PHONY:clean
clean:
	rm -rf *.o
	rm ptime main
