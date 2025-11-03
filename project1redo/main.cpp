#include <iostream>
#include "cpu.h"

using namespace std;

/*
CITATION:
I referenced my own work from lab quiz 0 and 1 for the string parsing and
command-handling structure in this file.
ChatGPT was used to check if my nested if statements were organized correctly.
*/
int main() {
    cpu cpu1;
    string input;

    while (cin >> input) {
        if (input == "ON") {
            int n; 
            cin >> n;
            if (cpu1.on(n)) {
                cout << "success\n";
            }
            else {
                cout << "failure\n";
            }
        }
        else if (input == "EXIT"){
            break;
        }
        else if (input == "SPAWN") {
            int pid; 
            cin >> pid;
            int cid;
            if (cpu1.spawn(pid, cid)) {
                cout << "core " << cid << " assigned task " << pid << endl;
            } 
            else {
                cout << "failure\n";
            }
        }
        else if (input == "RUN") {
            int cid; 
            cin >> cid;
            int pid = -1; bool empty = false;
            if (!cpu1.run(cid, pid, empty)) {
                cout << "failure\n";
            }
            else {
                if (empty) {
                    cout << "core " << cid << " has no tasks to run\n";
                }
                else {
                    cout << "core " << cid << " is running task " << pid << endl;
                }
            }
        }
        else if (input == "SLEEP") {
            int cid; 
            cin >> cid;
            if (!cpu1.sleep(cid)) {
                cout << "failure\n";
            }
        }
        else if (input == "SHUTDOWN") {
            if (!cpu1.shutdown()) {
                cout << "failure\n";
            }
        }
        else if (input == "SIZE") {
            int cid; 
            cin >> cid; 
            int s;
            if (cpu1.size(cid, s)) {
                cout << "size is " << s << endl;
            }
            else {
                cout << "failure\n";
            }
        }
        else if (input == "CAPACITY") {
            int cid; 
            cin >> cid; 
            int c;
            if (cpu1.capacity(cid, c)) {
                cout << "capacity is " << c << endl;
            }
            else {
                cout << "failure\n";
            }
        }
    }
    return 0;
}
