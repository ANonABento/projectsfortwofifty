#include <iostream>
#include <cmath>
#include <string>
using namespace std;

class Map {
private:
    int r, c;

public:
    Map() {
        r = c = 0;
    }

    ~Map() {
        destructor();
    }

    void destructor() {
        r = c = 0;
    }

    bool create(int n, int m) {
        destructor();
        if (n <= 0 || m <= 0) return false;
        r = n;
        c = m;
        return true;
    }
};

int main() {
    Map map;
    string input;

    while (cin >> input) {
        if (input == "CREATE") {
            int n, m;
            cin >> n >> m;
            if (map.create(n, m)) cout << "success\n";
            else cout << "failure\n";

        } else if (input == "POINT") {
            
        } else if (input == "MOVE") {
            
        } else if (input == "CLEAR") {
            
        } else if (input == "UPDATE") {
    
        } else if (input == "EXIT") {
            break;
        }
    }

    return 0;
}
