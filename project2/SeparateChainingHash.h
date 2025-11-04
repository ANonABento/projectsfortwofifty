#ifndef SEPARATECHAININGHASH_H
#define SEPARATECHAININGHASH_H

#include "HashTable.h"
#include "FileBlock.h"

class Node {
public:
    FileBlock block;
    Node* next;
    
    Node(unsigned int id, const std::string& data);
};

class SeparateChainingHash : public HashTable {
private:
    Node** table;

public:
    SeparateChainingHash(unsigned int size);
    ~SeparateChainingHash();
    
    bool store(unsigned int id, const std::string& data);
    int search(unsigned int id);
    bool deleteBlock(unsigned int id);
    bool corrupt(unsigned int id, const std::string& data);
    std::string validate(unsigned int id);
    void printChain(unsigned int index);
};

#endif