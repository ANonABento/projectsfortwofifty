#ifndef GRAPH_H
#define GRAPH_H
#include "Node.h"
#include "Edge.h"
#include <string>
#include <vector>

class illegal_exception {
public:
    illegal_exception() {}
};

class Graph {
private:
    std::vector<Node*> nodes;
    std::vector<Edge*> edges;
    
    // Hash table for fast node lookup
    std::vector<std::vector<Node*>> nodeMap;
    int mapSize;
    
    // Hash function for node IDs
    unsigned int hash(const std::string& id);
    
    // Helper to find a node by ID
    Node* findNodeById(const std::string& id);
    
    // Helper to find an edge between two nodes
    Edge* findEdge(Node* n1, Node* n2);
    
public:
    Graph();
    ~Graph();
    
    // Load from files
    void loadEntities(std::string filename);
    void loadRelationships(std::string filename);
    
    // Entity operations
    bool insertEntity(std::string id, std::string name, std::string type);
    bool deleteNode(std::string id);
    
    // Relationship operations
    bool insertRelationship(std::string sourceId, std::string label, 
                           std::string destId, double weight);

    // Check if ID contains only alphanumeric characters
    bool isValidId(const std::string& id);
    
    // Query operations
    std::vector<std::string> printAdjacent(std::string id);
    std::vector<std::string> findPath(std::string id1, std::string id2, double& totalWeight);
    std::vector<std::string> findHighest(double& totalWeight);
    std::vector<std::string> findAll(std::string fieldType, std::string fieldString);
    
    // Utility functions
    Node* getNode(std::string id);
    bool isEmpty();
};

#endif