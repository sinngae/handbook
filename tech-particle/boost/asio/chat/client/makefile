CC = g++ -g -Wall
INCLUDE_DIR = -I/opt/local/boost/include
LIB_DIR = -L/opt/local/boost/lib -lboost_system -lboost_date_time -lboost_timer -lboost_filesystem -lboost_thread -lpthread
CUR_SOURCE=${wildcard *.cpp}
CUR_OBJS=${patsubst %.cpp, %.o, $(CUR_SOURCE)}

all:main $(CUR_OBJS)

main:$(CUR_OBJS)
	$(CC) -o main $(CUR_OBJS) $(LIB_DIR)

$(CUR_OBJS):%.o:%.cpp
	$(CC) -c $^ -o $@ $(INCLUDE_DIR)

.PHONY:clean
clean:
	@rm -rf *.o
	@rm main

.PHONY:ech
ech:
	@echo $(CUR_SOURCE)
	@echo $(CUR_OBJS)
