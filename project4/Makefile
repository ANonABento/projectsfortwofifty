# CITATION:
# This Makefile format was written with help from chat.openai.com
# after requesting an example of how to compile multiple .cpp files

CXXFLAGS = -std=c++17 -g -O0

all: a.out

a.out: main.o Graph.o Node.o Edge.o
	g++ $(CXXFLAGS) main.o Graph.o Node.o Edge.o -o a.out

main.o: main.cpp Graph.h
	g++ $(CXXFLAGS) -c main.cpp

Graph.o: Graph.cpp Graph.h Node.h Edge.h
	g++ $(CXXFLAGS) -c Graph.cpp

Node.o: Node.cpp Node.h Edge.h
	g++ $(CXXFLAGS) -c Node.cpp

Edge.o: Edge.cpp Edge.h Node.h
	g++ $(CXXFLAGS) -c Edge.cpp

clean:
	rm -f *.o a.out