#ifndef GRAPH_H
#define GRAPH_H

#include "Node.h"
#include "Edge.h"
#include <string>
#include <vector>
#include <map>

class Graph {
private:
    std::map<std::string, Node*> nodes; // map ID to node
    std::vector<Edge*> edges; // all edges in graph
    
    Edge* findEdge(Node* n1, Node* n2);

public:
    Graph();
    ~Graph();
    
    void loadEntities(std::string filename);
    void loadRelationships(std::string filename);
    
    bool insertEntity(std::string id, std::string name, std::string type);
    bool insertRelationship(std::string sourceId, std::string label, std::string destId, double weight);
    
    bool deleteNode(std::string id);
    std::vector<std::string> printAdjacent(std::string id);
    
    std::vector<std::string> findPath(std::string id1, std::string id2, double& totalWeight);
    std::vector<std::string> findHighest(double& totalWeight);
    std::vector<std::string> findAll(std::string fieldType, std::string fieldString);
    
    Node* getNode(std::string id);
    bool isEmpty();
};

class illegal_exception {
    // empty exception class
};

#endif