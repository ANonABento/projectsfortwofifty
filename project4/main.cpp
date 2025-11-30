// main.cpp
// Main program for testing graph

#include "Graph.h"
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

int main() {
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
            if(result == true) {
                cout << "success" << endl;
            } else {
                cout << "failure" << endl;
            }
            
        } else if(command == "PRINT") {
            string id;
            cin >> id;
            
            try {
                // check if node exists first
                Node* node = graph.getNode(id);
                if(node == nullptr) {
                    cout << "failure" << endl;
                } else {
                    vector<string> adjacent = graph.printAdjacent(id);
                    if(adjacent.size() == 0) {
                        cout << endl; // blank line if no neighbors
                    } else {
                        for(int i = 0; i < adjacent.size(); i++) {
                            cout << adjacent[i];
                            if(i < adjacent.size() - 1) {
                                cout << " ";
                            }
                        }
                        cout << endl;
                    }
                }
            } catch(illegal_exception& e) {
                cout << "illegal argument" << endl;
            }
            
        } else if(command == "DELETE") {
            string id;
            cin >> id;
            
            try {
                bool result = graph.deleteNode(id);
                if(result == true) {
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
                    cout << fixed << setprecision(1) << totalWeight << endl;
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
                for(int i = 0; i < path.size(); i++) {
                    cout << path[i] << " ";
                }
                cout << fixed << setprecision(1) << totalWeight << endl;
            }
            
        } else if(command == "FINDALL") {
            string fieldType, fieldString;
            cin >> fieldType >> fieldString;
            
            vector<string> results = graph.findAll(fieldType, fieldString);
            
            if(results.size() == 0) {
                cout << "failure" << endl;
            } else {
                for(int i = 0; i < results.size(); i++) {
                    cout << results[i];
                    if(i < results.size() - 1) {
                        cout << " ";
                    }
                }
                cout << endl;
            }
            
        } else if(command == "EXIT") {
            break;
        }
    }
    
    return 0;
}