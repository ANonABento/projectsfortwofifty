#include "Trie.h"
#include "ece250_socket.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
    Trie trie;
    string command;
    
    while(cin >> command) {

        if(command == "LOAD") {
            string filename;
            cin >> filename;
            trie.load(filename);
            cout << "success" << endl;
        } 
        else if(command == "INSERT") {
            string classification;
            // clear the newline left by 'cin >> command' before using getline
            cin.ignore(); 
            getline(cin, classification);
            
            try {
                bool result = trie.insert(classification);
                if(result == true) {
                    cout << "success" << endl;
                } 
                else {
                    cout << "failure" << endl;
                }
            } 
            catch(illegal_exception& e) {
                cout << "illegal argument" << endl;
            }
            
        } 
        else if(command == "CLASSIFY") {
            string input;
            // clear newline for getline
            cin.ignore(); 
            getline(cin, input);
            
            try {
                string result = trie.classify(input);
                cout << result << endl;
            } 
            catch(illegal_exception& e) {
                cout << "illegal argument" << endl;
            }
            
        } 
        else if(command == "ERASE") {
            string classification;
            // clear newline for getline
            cin.ignore(); 
            getline(cin, classification);
            
            try {
                bool result = trie.erase(classification);
                if(result == true) {
                    cout << "success" << endl;
                } 
                else {
                    cout << "failure" << endl;
                }
            } 
            catch(illegal_exception& e) {
                cout << "illegal argument" << endl;
            }
            
        } 
        else if(command == "PRINT") {
            trie.print();
            
        } 
        else if(command == "EMPTY") {
            if(trie.isEmpty() == true) {
                cout << "empty 1" << endl;
            } 
            else {
                cout << "empty 0" << endl;
            }
            
        } 
        else if(command == "CLEAR") {
            trie.clear();
            cout << "success" << endl;
            
        } 
        else if(command == "SIZE") {
            cout << "number of classifications is " << trie.size() << endl;
            
        } 
        else if(command == "EXIT") {
            break;
        }
    }
    
    return 0;
}