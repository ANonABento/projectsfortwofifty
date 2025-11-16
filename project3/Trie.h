#ifndef TRIE_H
#define TRIE_H

#include "TrieNode.h"
#include <string>
#include <vector>

class Trie {
private:
    TrieNode* root;
    int classificationCount;
    
    void printHelper(TrieNode* node, std::string current, std::vector<std::string>& results);
    void clearHelper(TrieNode* node);

public:
    Trie();
    ~Trie();
    
    void load(std::string filename);
    bool insert(std::string classification);
    std::string classify(std::string input);
    bool erase(std::string classification);
    void print();
    bool isEmpty();
    void clear();
    int size();
};

class illegal_exception {
    // empty exception class for illegal arguments
};

#endif