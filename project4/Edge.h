#ifndef EDGE_H
#define EDGE_H

#include <string>

class Node; // forward declaration

class Edge {
private:
    Node* source;
    Node* destination;
    std::string label;
    double weight;

public:
    Edge();
    Edge(Node* src, Node* dest, std::string lbl, double wt);
    
    Node* getSource();
    Node* getDestination();
    std::string getLabel();
    double getWeight();
    
    void setLabel(std::string newLabel);
    void setWeight(double newWeight);
    
    Node* getOtherEnd(Node* current);
};

#endif