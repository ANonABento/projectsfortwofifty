#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "FileBlock.h"
#include <string>

class HashTable {
protected:
    unsigned int m;
    
    unsigned int h1(unsigned int k);
    unsigned int h2(unsigned int k);

public:
    HashTable(unsigned int size);
    virtual ~HashTable();
    
    virtual bool store(unsigned int id, const std::string& data) = 0;
    virtual int search(unsigned int id) = 0;
    virtual bool deleteBlock(unsigned int id) = 0;
    virtual bool corrupt(unsigned int id, const std::string& data) = 0;
    virtual std::string validate(unsigned int id) = 0;
};

#endif