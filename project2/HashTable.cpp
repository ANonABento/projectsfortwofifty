#include "HashTable.h"

HashTable::HashTable(unsigned int size) {
    m = size;
}

HashTable::~HashTable() {
}

unsigned int HashTable::h1(unsigned int k) {
    unsigned int result = k % m;
    return result;
}

unsigned int HashTable::h2(unsigned int k) {
    unsigned int result = (k / m) % m;
    // make sure its odd
    if(result % 2 == 0) {
        result = result + 1;
    }
    return result;
}