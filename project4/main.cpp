#include <iostream>
#include <string>

using namespace std;

int main() {
    string command;
    
    while(cin >> command) {
        if(command == "LOAD") {
            cout << "hello world" << endl;
        }
    }
    
    return 0;
}