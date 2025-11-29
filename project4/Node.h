#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>

class Edge; // forward declaration

class Node {
private:
    std::string id;
    std::string name;
    std::string type;
    std::vector<Edge*> edges; // all edges connected to this node

public:
    Node();
    Node(std::string nodeId, std::string nodeName, std::string nodeType);
    
    std::string getId();
    std::string getName();
    std::string getType();
    
    void setName(std::string newName);
    void setType(std::string newType);
    
    void addEdge(Edge* edge);
    void removeEdge(Edge* edge);
    std::vector<Edge*> getEdges();
};

#endif