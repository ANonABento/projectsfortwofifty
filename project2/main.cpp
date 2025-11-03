#include <iostream>
#include <string>
#include <sstream>
#include "OpenAddressingHash.h"
#include "SeparateChainingHash.h"

using namespace std;

string readUntilExclamation() {
    string result;
    char c;
    while (cin.get(c) && c != '!') {
        result += c;
    }
    return result;
}

int main() {
    HashTable* hashTable = nullptr;
    string command;
    
    while (cin >> command) {
        if (command == "NEW") {
            unsigned int n, t;
            cin >> n >> t;
            
            if (t == 0) {
                hashTable = new OpenAddressingHash(n);
            } else {
                hashTable = new SeparateChainingHash(n);
            }
            cout << "success" << endl;
            
        } else if (command == "STORE") {
            unsigned int id;
            cin >> id;
            cin.ignore(); // Ignore space after id
            
            string data = readUntilExclamation();
            
            if (hashTable->store(id, data)) {
                cout << "success" << endl;
            } else {
                cout << "failure" << endl;
            }
            
        } else if (command == "SEARCH") {
            unsigned int id;
            cin >> id;
            
            int pos = hashTable->search(id);
            if (pos != -1) {
                cout << "found ID in " << pos << endl;
            } else {
                cout << "not found" << endl;
            }
            
        } else if (command == "DELETE") {
            unsigned int id;
            cin >> id;
            
            if (hashTable->deleteBlock(id)) {
                cout << "success" << endl;
            } else {
                cout << "failure" << endl;
            }
            
        } else if (command == "CORRUPT") {
            unsigned int id;
            cin >> id;
            cin.ignore(); // Ignore space after id
            
            string data = readUntilExclamation();
            
            if (hashTable->corrupt(id, data)) {
                cout << "success" << endl;
            } else {
                cout << "failure" << endl;
            }
            
        } else if (command == "VALIDATE") {
            unsigned int id;
            cin >> id;
            
            cout << hashTable->validate(id) << endl;
            
        } else if (command == "PRINT") {
            unsigned int index;
            cin >> index;
            
            SeparateChainingHash* chainHash = dynamic_cast<SeparateChainingHash*>(hashTable);
            if (chainHash) {
                chainHash->printChain(index);
            }
            
        } else if (command == "EXIT") {
            break;
        }
    }
    
    delete hashTable;
    return 0;
}