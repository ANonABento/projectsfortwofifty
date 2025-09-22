#include <iostream>
#include <cmath>
#include <string>
using namespace std;

class Map {
private:
    int r, c;
    char **pointer;

public:
    Map() {
        r = c = 0;
        pointer = nullptr;
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
        pointer = new char*[r];
        for (int i = 0; i < r; i++) {
            pointer[i] = new char[c];
            for (int k = 0; k < c; k++) {
                pointer[i][k] = 0;
            }
        }
        return true;
    }

    bool addPoint(char type, int x, int y) {
        if (x < 0 || y < 0 || x >= r || y >= c) return false;
        if (type != 'G' && type != 'O') return false;
        pointer[x][y] = type;
        recompute();
        return true;
    }

    void recompute(){
        // do stuff later
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
            char t; 
            int x, y;
            cin >> t >> x >> y;
            if (map.addPoint(t, x, y)) cout << "success\n";
            else cout << "failure\n";

        } else if (input == "MOVE") {
            
        } else if (input == "CLEAR") {
            
        } else if (input == "UPDATE") {
    
        } else if (input == "EXIT") {
            break;
        }
    }

    return 0;
}
