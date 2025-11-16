#ifndef TRIENODE_H
#define TRIENODE_H

#include <string>

class TrieNode {
private:
    TrieNode* children[15]; // 15 children max
    bool isTerminal;
    std::string className;

public:
    TrieNode();
    TrieNode(std::string name);
    ~TrieNode();
    
    TrieNode* getChild(int index);
    void setChild(int index, TrieNode* child);
    bool getTerminal();
    void setTerminal(bool terminal);
    std::string getClassName();
    int findChildIndex(std::string name);
    bool hasChildren();
};

#endif