#include "Graph.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

// Helper to print weight cleanly (Integer if whole number, 1 decimal otherwise)
void printWeight(double weight) {
    // Check if weight is effectively an integer (handling small float errors)
    if (std::abs(weight - std::round(weight)) < 0.0001) {
        cout << (long long)std::round(weight) << endl;
    } else {
        cout << fixed << setprecision(1) << weight << endl;
    }
}

int main() {
    // Optimization: fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    Graph graph;
    string command;
    
    while(cin >> command) {
        
        if(command == "LOAD") {
            string filename, type;
            cin >> filename >> type;
            
            if(type == "entities") {
                graph.loadEntities(filename);
            } else if(type == "relationships") {
                graph.loadRelationships(filename);
            }
            cout << "success" << endl;
            
        } else if(command == "ENTITY") {
            string id, name, type;
            cin >> id >> name >> type;
            
            graph.insertEntity(id, name, type);
            cout << "success" << endl;
            
        } else if(command == "RELATIONSHIP") {
            string sourceId, label, destId;
            double weight;
            cin >> sourceId >> label >> destId >> weight;
            
            bool result = graph.insertRelationship(sourceId, label, destId, weight);
            if(result) {
                cout << "success" << endl;
            } else {
                cout << "failure" << endl;
            }
            
        } else if(command == "PRINT") {
            string id;
            cin >> id;
            
            try {
                // Check validity first
                if(!graph.isValidId(id)) throw illegal_exception();
                
                Node* node = graph.getNode(id);
                if(node == nullptr) {
                    cout << "failure" << endl;
                } else {
                    vector<string> adjacent = graph.printAdjacent(id);
                    for(int i = 0; i < adjacent.size(); i++) {
                        if(i > 0) cout << " ";
                        cout << adjacent[i];
                    }
                    cout << endl;
                }
            } catch(illegal_exception& e) {
                cout << "illegal argument" << endl;
            }
            
        } else if(command == "DELETE") {
            string id;
            cin >> id;
            
            try {
                bool result = graph.deleteNode(id);
                if(result) {
                    cout << "success" << endl;
                } else {
                    cout << "failure" << endl;
                }
            } catch(illegal_exception& e) {
                cout << "illegal argument" << endl;
            }
            
        } else if(command == "PATH") {
            string id1, id2;
            cin >> id1 >> id2;
            
            try {
                double totalWeight = 0;
                vector<string> path = graph.findPath(id1, id2, totalWeight);
                
                if(path.size() == 0) {
                    cout << "failure" << endl;
                } else {
                    for(int i = 0; i < path.size(); i++) {
                        cout << path[i] << " ";
                    }
                    printWeight(totalWeight);
                }
            } catch(illegal_exception& e) {
                cout << "illegal argument" << endl;
            }
            
        } else if(command == "HIGHEST") {
            double totalWeight = 0;
            vector<string> path = graph.findHighest(totalWeight);
            
            if(path.size() == 0) {
                cout << "failure" << endl;
            } else {
                // Output first and last node IDs
                cout << path[0] << " " << path[path.size() - 1] << " ";
                printWeight(totalWeight);
            }
            
        } else if(command == "FINDALL") {
            string fieldType, fieldString;
            cin >> fieldType >> fieldString;
            
            vector<string> results = graph.findAll(fieldType, fieldString);
            
            if(results.size() == 0) {
                cout << "failure" << endl;
            } else {
                for(int i = 0; i < results.size(); i++) {
                    if(i > 0) cout << " ";
                    cout << results[i];
                }
                cout << endl;
            }
            
        } else if(command == "EXIT") {
            break;
        }
    }
    
    return 0;
}