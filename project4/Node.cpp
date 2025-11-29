#include "Node.h"
#include "Edge.h"
#include <algorithm>

Node::Node() {
    id = "";
    name = "";
    type = "";
}

Node::Node(std::string nodeId, std::string nodeName, std::string nodeType) {
    id = nodeId;
    name = nodeName;
    type = nodeType;
}

std::string Node::getId() {
    return id;
}

std::string Node::getName() {
    return name;
}

std::string Node::getType() {
    return type;
}

void Node::setName(std::string newName) {
    name = newName;
}

void Node::setType(std::string newType) {
    type = newType;
}

void Node::addEdge(Edge* edge) {
    edges.push_back(edge);
}

void Node::removeEdge(Edge* edge) {
    // find and remove the edge from the vector
    for(int i = 0; i < edges.size(); i++) {
        if(edges[i] == edge) {
            edges.erase(edges.begin() + i);
            break;
        }
    }
}

std::vector<Edge*> Node::getEdges() {
    return edges;
}