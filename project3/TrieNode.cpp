#include "TrieNode.h"

TrieNode::TrieNode() {
    className = "";
    isTerminal = false;
    for(int i = 0; i < 15; i++) {
        children[i] = nullptr;
    }
}

TrieNode::TrieNode(std::string name) {
    className = name;
    isTerminal = false;
    for(int i = 0; i < 15; i++) {
        children[i] = nullptr;
    }
}

/*
CITATION:
Asked ChatGPT (chat.openai.com) how to properly initialize an array
of pointers to nullptr and how to recursively delete all children
in a tree structure. Used the explanation to write my destructor.
*/
TrieNode::~TrieNode() {
    // delete all children recursively
    for(int i = 0; i < 15; i++) {
        if(children[i] != nullptr) {
            delete children[i];
            children[i] = nullptr;
        }
    }
}

TrieNode* TrieNode::getChild(int index) {
    if(index >= 0 && index < 15) {
        return children[index];
    }
    return nullptr;
}

void TrieNode::setChild(int index, TrieNode* child) {
    if(index >= 0 && index < 15) {
        children[index] = child;
    }
}

bool TrieNode::getTerminal() {
    return isTerminal;
}

void TrieNode::setTerminal(bool terminal) {
    isTerminal = terminal;
}

std::string TrieNode::getClassName() {
    return className;
}

int TrieNode::findChildIndex(std::string name) {
    // look through all children to find one with matching name
    for(int i = 0; i < 15; i++) {
        if(children[i] != nullptr) {
            if(children[i]->getClassName() == name) {
                return i;
            }
        }
    }
    return -1; // not found
}

bool TrieNode::hasChildren() {
    for(int i = 0; i < 15; i++) {
        if(children[i] != nullptr) {
            return true;
        }
    }
    return false;
}