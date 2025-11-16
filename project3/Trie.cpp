#include "Trie.h"
#include "ece250_socket.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

Trie::Trie() {
    root = new TrieNode("");
    classificationCount = 0;
}

Trie::~Trie() {
    delete root;
}

void Trie::load(std::string filename) {
    std::ifstream file(filename);
    std::string line;
    
    while(std::getline(file, line)) {
        // try to insert each line
        try {
            insert(line);
        } catch(illegal_exception& e) {
            // if illegal, skip and continue parsing
        }
    }
    file.close();
}

bool Trie::insert(std::string classification) {
    // check for uppercase letters
    for(int i = 0; i < classification.length(); i++) {
        if(classification[i] >= 'A' && classification[i] <= 'Z') {
            throw illegal_exception();
        }
    }
    
    // parse the classification string
    std::istringstream stream(classification);
    std::string className;
    std::vector<std::string> classes;
    
    while(std::getline(stream, className, ',')) {
        classes.push_back(className);
    }
    
    if(classes.size() == 0) {
        return false;
    }
    
    // traverse and insert
    TrieNode* current = root;
    for(int i = 0; i < classes.size(); i++) {
        int childIndex = current->findChildIndex(classes[i]);
        
        if(childIndex == -1) {
            // need to create new child
            // find first empty spot
            int emptyIndex = -1;
            for(int j = 0; j < 15; j++) {
                if(current->getChild(j) == nullptr) {
                    emptyIndex = j;
                    break;
                }
            }
            
            if(emptyIndex == -1) {
                // no space for new child
                return false;
            }
            
            TrieNode* newNode = new TrieNode(classes[i]);
            current->setChild(emptyIndex, newNode);
            current = newNode;
        } else {
            // child already exists
            current = current->getChild(childIndex);
        }
    }
    
    // check if already terminal
    if(current->getTerminal() == true) {
        return false; // already exists
    }
    
    current->setTerminal(true);
    classificationCount++;
    return true;
}

std::string Trie::classify(std::string input) {
    // check for uppercase
    for(int i = 0; i < input.length(); i++) {
        if(input[i] >= 'A' && input[i] <= 'Z') {
            throw illegal_exception();
        }
    }
    
    std::string result = "";
    TrieNode* current = root;
    
    while(current->hasChildren()) {
        // get all child class names
        std::string candidates = "";
        int count = 0;
        
        for(int i = 0; i < 15; i++) {
            TrieNode* child = current->getChild(i);
            if(child != nullptr) {
                if(count > 0) {
                    candidates = candidates + ",";
                }
                candidates = candidates + child->getClassName();
                count++;
            }
        }
        
        if(candidates == "") {
            break;
        }
        
        // call classifier
        std::string bestClass = labelText(input, candidates);
        
        if(bestClass == "") {
            break;
        }
        
        // add to result
        if(result != "") {
            result = result + ",";
        }
        result = result + bestClass;
        
        // move to that child
        int childIndex = current->findChildIndex(bestClass);
        if(childIndex == -1) {
            break;
        }
        current = current->getChild(childIndex);
    }
    
    return result;
}

bool Trie::erase(std::string classification) {
    // check for uppercase
    for(int i = 0; i < classification.length(); i++) {
        if(classification[i] >= 'A' && classification[i] <= 'Z') {
            throw illegal_exception();
        }
    }
    
    if(isEmpty()) {
        return false;
    }
    
    // parse classification
    std::istringstream stream(classification);
    std::string className;
    std::vector<std::string> classes;
    
    while(std::getline(stream, className, ',')) {
        classes.push_back(className);
    }
    
    if(classes.size() == 0) {
        return false;
    }
    
    // traverse to find the node
    TrieNode* current = root;
    std::vector<TrieNode*> path;
    path.push_back(current);
    
    for(int i = 0; i < classes.size(); i++) {
        int childIndex = current->findChildIndex(classes[i]);
        if(childIndex == -1) {
            return false; // not found
        }
        current = current->getChild(childIndex);
        path.push_back(current);
    }
    
    // check if terminal
    if(current->getTerminal() == false) {
        return false;
    }
    
    // mark as not terminal
    current->setTerminal(false);
    classificationCount--;
    
    // delete nodes that have no children and aren't terminal
    for(int i = path.size() - 1; i > 0; i--) {
        TrieNode* node = path[i];
        if(node->hasChildren() == false && node->getTerminal() == false) {
            TrieNode* parent = path[i-1];
            // find and remove this child from parent
            for(int j = 0; j < 15; j++) {
                if(parent->getChild(j) == node) {
                    delete node;
                    parent->setChild(j, nullptr);
                    break;
                }
            }
        } else {
            break;
        }
    }
    
    return true;
}

void Trie::printHelper(TrieNode* node, std::string current, std::vector<std::string>& results) {
    if(node->getTerminal() == true) {
        results.push_back(current);
    }
    
    for(int i = 0; i < 15; i++) {
        TrieNode* child = node->getChild(i);
        if(child != nullptr) {
            std::string newCurrent = current;
            if(current != "") {
                newCurrent = newCurrent + ",";
            }
            newCurrent = newCurrent + child->getClassName();
            printHelper(child, newCurrent, results);
        }
    }
}

void Trie::print() {
    if(isEmpty()) {
        std::cout << "trie is empty" << std::endl;
        return;
    }
    
    std::vector<std::string> results;
    printHelper(root, "", results);
    
    for(int i = 0; i < results.size(); i++) {
        std::cout << results[i];
        if(i < results.size() - 1) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
}

bool Trie::isEmpty() {
    if(classificationCount == 0) {
        return true;
    } else {
        return false;
    }
}

void Trie::clearHelper(TrieNode* node) {
    for(int i = 0; i < 15; i++) {
        TrieNode* child = node->getChild(i);
        if(child != nullptr) {
            clearHelper(child);
            delete child;
            node->setChild(i, nullptr);
        }
    }
}

void Trie::clear() {
    clearHelper(root);
    classificationCount = 0;
}

int Trie::size() {
    return classificationCount;
}