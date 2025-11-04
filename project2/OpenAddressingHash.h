#ifndef OPENADDRESSINGHASH_H
#define OPENADDRESSINGHASH_H

#include "HashTable.h"
#include "FileBlock.h"

class OpenAddressingHash : public HashTable {
private:
    FileBlock* table;
    bool* deleted;

public:
    OpenAddressingHash(unsigned int size);
    ~OpenAddressingHash();
    
    bool store(unsigned int id, const std::string& data);
    int search(unsigned int id);
    bool deleteBlock(unsigned int id);
    bool corrupt(unsigned int id, const std::string& data);
    std::string validate(unsigned int id);
};

#endif