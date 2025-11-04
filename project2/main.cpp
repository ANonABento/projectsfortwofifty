/*
CITATION:
Asked ChatGPT (chat.openai.com) about using cin.get() vs getline() and
how to properly ignore whitespace. The command processing structure
and overall program flow are my own implementation.
*/

#include <iostream>
#include <string>
#include "OpenAddressingHash.h"
#include "SeparateChainingHash.h"

using namespace std;

int main() {
    HashTable* hashTable = nullptr;
    string command;
    
    while(cin >> command) {
        
        if(command == "NEW") {
            unsigned int n, t;
            cin >> n >> t;
            
            // t=0 means open addressing, t=1 means separate chaining
            if(t == 0) {
                hashTable = new OpenAddressingHash(n);
            } else if(t == 1) {
                hashTable = new SeparateChainingHash(n);
            }
            cout << "success" << endl;
            
        } else if(command == "STORE") {
            unsigned int id;
            cin >> id;
            cin.ignore(); // skip space after id
            
            // read data until we hit '!'
            string data = "";
            char c;
            while(cin.get(c)) {
                if(c == '!') {
                    break;
                }
                data = data + c;
            }
            
            bool result = hashTable->store(id, data);
            if(result == true) {
                cout << "success" << endl;
            } else {
                cout << "failure" << endl;
            }
            
        } else if(command == "SEARCH") {
            unsigned int id;
            cin >> id;
            
            int pos = hashTable->search(id);
            if(pos != -1) {
                cout << "found ID in " << pos << endl;
            } else {
                cout << "not found" << endl;
            }
            
        } else if(command == "DELETE") {
            unsigned int id;
            cin >> id;
            
            bool result = hashTable->deleteBlock(id);
            if(result == true) {
                cout << "success" << endl;
            } else {
                cout << "failure" << endl;
            }
            
        } else if(command == "CORRUPT") {
            unsigned int id;
            cin >> id;
            cin.ignore(); // skip space
            
            // read data until '!'
            string data = "";
            char c;
            while(cin.get(c)) {
                if(c == '!') {
                    break;
                }
                data = data + c;
            }
            
            bool result = hashTable->corrupt(id, data);
            if(result == true) {
                cout << "success" << endl;
            } else {
                cout << "failure" << endl;
            }
            
        } else if(command == "VALIDATE") {
            unsigned int id;
            cin >> id;
            
            string result = hashTable->validate(id);
            cout << result << endl;
            
        } else if(command == "PRINT") {
            unsigned int index;
            cin >> index;
            
            // cast to separate chaining to call print
            SeparateChainingHash* chainHash = dynamic_cast<SeparateChainingHash*>(hashTable);
            if(chainHash != nullptr) {
                chainHash->printChain(index);
            }
            
        } else if(command == "EXIT") {
            break;
        }
    }
    
    // cleanup
    if(hashTable != nullptr) {
        delete hashTable;
    }
    
    return 0;
}