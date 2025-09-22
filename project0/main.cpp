#include <iostream>
#include <cmath>
#include <string>
using namespace std;

class Map {
private:
    int r, c;
    char **pointer; // G goal / O obstacle / 0 for empty
    double **px;
    double **py;
    double potential;

public:
    Map() {
        r = c = 0;
        pointer = nullptr;
        px = py = nullptr;
        potential = 1;
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
        px = new double*[r];
        py = new double*[r];

        for (int i = 0; i < r; i++) {
            pointer[i] = new char[c];
            px[i] = new double[c];
            py[i] = new double[c];
            for (int k = 0; k < c; k++) {
                pointer[i][k] = 0;
                px[i][k] = 0.0;
                py[i][k] = 0.0;
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

    void recompute() {
        for (int i = 0; i < r; i++) {
            for (int k = 0; k < c; k++) {
                px[i][k] = 0.0;
                py[i][k] = 0.0;
            }
        }

        for (int i = 0; i < r; i++) {
            for (int k = 0; k < c; k++) {
                if (pointer[i][k] == 'G' || pointer[i][k] == 'O') {
                    int sign = (pointer[i][k] == 'G') ? -1 : 1;

                    for (int x = 0; x < r; x++) {
                        for (int y = 0; y < c; y++) {
                            if (x == i && y == k) continue;

                            double dx = x - i;
                            double dy = y - k;
                            double distance = sqrt(dx*dx + dy*dy);

                            if (distance > 0) {
                                double effect = potential / distance;
                                px[x][y] += sign * effect;
                                py[x][y] += sign * effect;
                            }
                        }
                    }
                }
            }
        }
    }

    bool move(int x, int y) {
        if (x < 0 || y < 0 || x >= r || y >= c) return false;
        cout << px[x][y] << " " << py[x][y] << endl;
        return true;
    }
};

int main() {
    Map map;
    string input;
    int n, m;
    char t; 
    int x, y;

    while (cin >> input) {
        if (input == "CREATE") {
            cin >> n >> m;
            if (map.create(n, m)) cout << "success\n";
            else cout << "failure\n";

        } else if (input == "POINT") {
            cin >> t >> x >> y;
            if (map.addPoint(t, x, y)) cout << "success\n";
            else cout << "failure\n";

        } else if (input == "MOVE") {
            cin >> x >> y;
            if (!map.move(x, y)) cout << "failure\n";
        } else if (input == "CLEAR") {
            
        } else if (input == "UPDATE") {
    
        } else if (input == "EXIT") {
            break;
        }
    }

    return 0;
}
