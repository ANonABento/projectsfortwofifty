# CITATION:
# This Makefile format was made from previous Makefiles that recieved help
# from chat.openai.com for examples of how to compile multiple .cpp files

all: a.out

a.out: main.o Trie.o TrieNode.o ece250_socket.o
	g++ -std=c++17 main.o Trie.o TrieNode.o ece250_socket.o -o a.out

main.o: main.cpp Trie.h ece250_socket.h
	g++ -std=c++17 -c main.cpp

Trie.o: Trie.cpp Trie.h TrieNode.h ece250_socket.h
	g++ -std=c++17 -c Trie.cpp

TrieNode.o: TrieNode.cpp TrieNode.h
	g++ -std=c++17 -c TrieNode.cpp

ece250_socket.o: ece250_socket.cpp ece250_socket.h
	g++ -std=c++17 -c ece250_socket.cpp

clean:
	rm -f *.o a.out