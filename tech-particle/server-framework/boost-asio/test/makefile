INC=-I/opt/local/boost/include
LIB=-L/opt/local/boost/lib -lboost_system


client:client.o
	g++ -o client client.o $(LIB)

client.o:client.cpp
	g++ -c client.cpp $(INC)

.PHONY:clean
clean:
	@rm -f client
	@rm -f *.o
