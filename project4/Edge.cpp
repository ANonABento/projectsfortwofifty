#include "Edge.h"
#include "Node.h"

Edge::Edge() {
    source = nullptr;
    destination = nullptr;
    label = "";
    weight = 0.0;
}

Edge::Edge(Node* src, Node* dest, std::string lbl, double wt) {
    source = src;
    destination = dest;
    label = lbl;
    weight = wt;
}

Node* Edge::getSource() {
    return source;
}

Node* Edge::getDestination() {
    return destination;
}

std::string Edge::getLabel() {
    return label;
}

double Edge::getWeight() {
    return weight;
}

void Edge::setLabel(std::string newLabel) {
    label = newLabel;
}

void Edge::setWeight(double newWeight) {
    weight = newWeight;
}

Node* Edge::getOtherEnd(Node* current) {
    // for undirected edges, return the other endpoint
    if(current == source) {
        return destination;
    } else {
        return source;
    }
}