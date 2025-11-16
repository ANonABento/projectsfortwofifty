/*
CITATION:
Used ChatGPT and video example to better understand how to use istringstream
to parse comma-separated strings and how to properly read from a file
line by line. Also asked about exception handling syntax with try-catch
blocks and how to check for uppercase letters in strings.
*/

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
        try {
            insert(line);
        } catch(illegal_exception& e) {
            // if illegal, skip and continue
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
    
    // parse the classification string by comma delimiter
    std::istringstream stream(classification);
    std::string className;
    std::vector<std::string> classes;
    
    while(std::getline(stream, className, ',')) {
        classes.push_back(className);
    }
    
    if(classes.size() == 0) {
        return false;
    }
    
    // traverse down the trie following the classification path
    TrieNode* current = root;
    for(int i = 0; i < classes.size(); i++) {
        int childIndex = current->findChildIndex(classes[i]);
        
        if(childIndex == -1) {
            // if child doesn't exist, need to create it
            // find first empty spot in the 15 child array
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
            // child already exists, just traverse to it
            current = current->getChild(childIndex);
        }
    }
    
    // check if this exact classification already exists
    if(current->getTerminal() == true) {
        return false;
    }
    
    // mark this node as a terminal classification
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
    
    // keep traversing deeper as long as there are children to check
    while(current->hasChildren()) {
        // collect all child class names to send to classifier
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
        
        // call the language model classifier
        std::string bestClass = labelText(input, candidates);
        
        if(bestClass == "") {
            break;
        }
        
        // add this classification level to result
        if(result != "") {
            result = result + ",";
        }
        result = result + bestClass;
        
        // move down to that child for next iteration
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
    
    // need keep track of entire path, we need the path so we can delete upward
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
    
    // make sure it's actually a terminal node
    if(current->getTerminal() == false) {
        return false;
    }
    
    // mark as not terminal
    current->setTerminal(false);
    classificationCount--;
    
    // delete nodes that have no children and aren't terminal
    // work backwards from the deleted node up to root
    for(int i = path.size() - 1; i > 0; i--) {
        TrieNode* node = path[i];
        if(node->hasChildren() == false && node->getTerminal() == false) {
            TrieNode* parent = path[i-1];
            // find this child in parent's array and delete it
            for(int j = 0; j < 15; j++) {
                if(parent->getChild(j) == node) {
                    delete node;
                    parent->setChild(j, nullptr);
                    break;
                }
            }
        } else {
            // stop if node has children or is terminal
            break;
        }
    }
    
    return true;
}

void Trie::printHelper(TrieNode* node, std::string current, std::vector<std::string>& results) {
    // only print if terminal and has no children cuz internal nodes can't be terminal so only print actual leaf classifications
    if(node->getTerminal() == true && node->hasChildren() == false) {
        results.push_back(current);
    }
    
    // recursively visit all children
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
    
    // print with underscores between classifications and keep the trailing underscore
    for(int i = 0; i < results.size(); i++) {
        std::cout << results[i] << "_";
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
    // recursively delete all children first
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