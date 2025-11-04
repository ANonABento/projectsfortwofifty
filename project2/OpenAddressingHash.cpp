/*
CITATION:
Consulted ChatGPT (chat.openai.com) about how to handle the case 
where we need to distinguish between empty slots and deleted slots 
during search operations. The probing logic and implementation details 
are my own based on course material and the project specification.
*/

#include "OpenAddressingHash.h"

OpenAddressingHash::OpenAddressingHash(unsigned int size) : HashTable(size) {
    table = new FileBlock[m];
    deleted = new bool[m];
    // initialize deleted array
    for(unsigned int i = 0; i < m; i++) {
        deleted[i] = false;
    }
}

OpenAddressingHash::~OpenAddressingHash() {
    delete[] table;
    delete[] deleted;
}

bool OpenAddressingHash::store(unsigned int id, const std::string& data) {
    unsigned int pos = h1(id);
    unsigned int step = h2(id);
    unsigned int count = 0;
    
    // probe until we find a spot or check entire table
    while(count < m) {
        if(table[pos].isOccupied() == false || deleted[pos] == true) {
            // found empty or deleted slot -> can insert here
            table[pos].setData(id, data.c_str());
            deleted[pos] = false;
            return true;
        } else {
            // slot is occupied -> check if duplicate key
            if(table[pos].getId() == id) {
                return false; // key already exists
            }
        }
        // move to next position using double hashing
        pos = (pos + step) % m;
        count++;
    }
    // table is full
    return false;
}

int OpenAddressingHash::search(unsigned int id) {
    unsigned int pos = h1(id);
    unsigned int step = h2(id);
    unsigned int count = 0;
    
    while(count < m) {
        if(table[pos].isOccupied() == true) {
            // check if this is the id we want
            if(table[pos].getId() == id) {
                return pos;
            }
        } else {
            // empty slot
            if(deleted[pos] == false) {
                // not deleted, so key doesn't exist
                return -1;
            }
        }
        pos = (pos + step) % m;
        count++;
    }
    return -1; // not found
}

bool OpenAddressingHash::deleteBlock(unsigned int id) {
    int pos = search(id);
    if(pos == -1) {
        return false;
    } else {
        table[pos].clear();
        deleted[pos] = true; // mark as deleted
        return true;
    }
}

bool OpenAddressingHash::corrupt(unsigned int id, const std::string& data) {
    int pos = search(id);
    if(pos == -1) {
        return false;
    } else {
        table[pos].corruptData(data.c_str());
        return true;
    }
}

std::string OpenAddressingHash::validate(unsigned int id) {
    int pos = search(id);
    if(pos == -1) {
        return "failure";
    } else {
        bool valid = table[pos].validateChecksum();
        if(valid == true) {
            return "valid";
        } else {
            return "invalid";
        }
    }
}