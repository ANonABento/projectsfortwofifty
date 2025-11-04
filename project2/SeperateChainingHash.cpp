/*
CITATION:
Used ChatGPT (chat.openai.com) to help debug my initial implementation
of sorted linked list insertion. I was having trouble with the insertion
logic when the new node needed to go in the middle of the list. ChatGPT
helped me understand the pointer manipulation needed, but I wrote and
tested the final implementation myself based on course lecture notes
about linked lists.
*/

#include "SeparateChainingHash.h"
#include <iostream>

Node::Node(unsigned int id, const std::string& data) {
    block.setData(id, data.c_str());
    next = nullptr;
}

SeparateChainingHash::SeparateChainingHash(unsigned int size) : HashTable(size) {
    table = new Node*[m];
    // initialize all chains to null
    for(unsigned int i = 0; i < m; i++) {
        table[i] = nullptr;
    }
}

SeparateChainingHash::~SeparateChainingHash() {
    // delete all nodes in all chains
    for(unsigned int i = 0; i < m; i++) {
        Node* current = table[i];
        while(current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete[] table;
}

bool SeparateChainingHash::store(unsigned int id, const std::string& data) {
    unsigned int pos = h1(id);
    
    // first check if id already exists
    Node* current = table[pos];
    while(current != nullptr) {
        if(current->block.getId() == id) {
            return false; // duplicate
        }
        current = current->next;
    }
    
    // create new node
    Node* newNode = new Node(id, data);
    
    // insert in sorted order (ascending by ID)
    if(table[pos] == nullptr) {
        // empty chain
        table[pos] = newNode;
        return true;
    } else if(table[pos]->block.getId() > id) {
        // insert at beginning
        newNode->next = table[pos];
        table[pos] = newNode;
        return true;
    } else {
        // find position in middle or end
        current = table[pos];
        while(current->next != nullptr && current->next->block.getId() < id) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
        return true;
    }
}

int SeparateChainingHash::search(unsigned int id) {
    unsigned int pos = h1(id);
    Node* current = table[pos];
    
    // traverse chain looking for id
    while(current != nullptr) {
        if(current->block.getId() == id) {
            return pos; // return chain index not position in chain
        }
        current = current->next;
    }
    return -1; // not found
}

bool SeparateChainingHash::deleteBlock(unsigned int id) {
    unsigned int pos = h1(id);
    Node* current = table[pos];
    Node* previous = nullptr;
    
    while(current != nullptr) {
        if(current->block.getId() == id) {
            // found it
            if(previous == nullptr) {
                // deleting first node
                table[pos] = current->next;
            } else {
                // deleting middle or end
                previous->next = current->next;
            }
            delete current;
            return true;
        }
        previous = current;
        current = current->next;
    }
    return false; // not found
}

bool SeparateChainingHash::corrupt(unsigned int id, const std::string& data) {
    unsigned int pos = h1(id);
    Node* current = table[pos];
    
    while(current != nullptr) {
        if(current->block.getId() == id) {
            current->block.corruptData(data.c_str());
            return true;
        }
        current = current->next;
    }
    return false; // not found
}

std::string SeparateChainingHash::validate(unsigned int id) {
    unsigned int pos = h1(id);
    Node* current = table[pos];
    
    while(current != nullptr) {
        if(current->block.getId() == id) {
            bool valid = current->block.validateChecksum();
            if(valid == true) {
                return "valid";
            } else {
                return "invalid";
            }
        }
        current = current->next;
    }
    return "failure"; // not found
}

void SeparateChainingHash::printChain(unsigned int index) {
    if(index >= m) {
        std::cout << "chain is empty" << std::endl;
        return;
    }
    
    Node* current = table[index];
    if(current == nullptr) {
        std::cout << "chain is empty" << std::endl;
        return;
    }
    
    // print all ids in chain separated by spaces
    int count = 0;
    while(current != nullptr) {
        if(count > 0) {
            std::cout << " ";
        }
        std::cout << current->block.getId();
        count++;
        current = current->next;
    }
    std::cout << std::endl;
}