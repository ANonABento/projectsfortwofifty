# CITATION:
# This Makefile format was written with help from chat.openai.com
# after requesting an example of how to compile multiple .cpp files

all: a.out

a.out: main.o Graph.o Node.o Edge.o
	g++ -std=c++17 main.o Graph.o Node.o Edge.o -o a.out

main.o: main.cpp Graph.h
	g++ -std=c++17 -c main.cpp

Graph.o: Graph.cpp Graph.h Node.h Edge.h
	g++ -std=c++17 -c Graph.cpp

Node.o: Node.cpp Node.h Edge.h
	g++ -std=c++17 -c Node.cpp

Edge.o: Edge.cpp Edge.h Node.h
	g++ -std=c++17 -c Edge.cpp

clean:
	rm -f *.o a.out