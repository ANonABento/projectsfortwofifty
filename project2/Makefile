# CITATION:
# This Makefile format was written with help from chat.openai.com
# after requesting an example of how to compile multiple .cpp files

all: a.out

a.out: main.o FileBlock.o HashTable.o OpenAddressingHash.o SeparateChainingHash.o
	g++ -std=c++11 main.o FileBlock.o HashTable.o OpenAddressingHash.o SeparateChainingHash.o -o a.out

main.o: main.cpp OpenAddressingHash.h SeparateChainingHash.h
	g++ -std=c++11 -c main.cpp

FileBlock.o: FileBlock.cpp FileBlock.h
	g++ -std=c++11 -c FileBlock.cpp

HashTable.o: HashTable.cpp HashTable.h FileBlock.h
	g++ -std=c++11 -c HashTable.cpp

OpenAddressingHash.o: OpenAddressingHash.cpp OpenAddressingHash.h HashTable.h FileBlock.h
	g++ -std=c++11 -c OpenAddressingHash.cpp

SeparateChainingHash.o: SeparateChainingHash.cpp SeparateChainingHash.h HashTable.h FileBlock.h
	g++ -std=c++11 -c SeparateChainingHash.cpp

clean:
	rm -f *.o a.out