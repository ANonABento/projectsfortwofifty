// Graph.cpp
// Implementation of Graph class

/*
CITATION:
Used ChatGPT (chat.openai.com) to understand Dijkstra's algorithm for
finding highest weight paths (had to modify it since we want MAX weight
not min distance). Also asked about how to use a priority queue with
custom comparators in C++ and how to check if a string contains only
alphanumeric characters for the illegal argument checks.
*/

#include "Graph.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include <algorithm>
#include <limits>

Graph::Graph() {
    // nothing to initialize
}

Graph::~Graph() {
    // delete all nodes
    for(auto it = nodes.begin(); it != nodes.end(); it++) {
        delete it->second;
    }
    // delete all edges
    for(int i = 0; i < edges.size(); i++) {
        delete edges[i];
    }
}

void Graph::loadEntities(std::string filename) {
    std::ifstream file(filename);
    std::string line;
    
    while(std::getline(file, line)) {
        std::istringstream stream(line);
        std::string id, name, type;
        stream >> id >> name >> type;
        insertEntity(id, name, type);
    }
    file.close();
}

void Graph::loadRelationships(std::string filename) {
    std::ifstream file(filename);
    std::string line;
    
    while(std::getline(file, line)) {
        std::istringstream stream(line);
        std::string sourceId, label, destId;
        double weight;
        stream >> sourceId >> label >> destId >> weight;
        insertRelationship(sourceId, label, destId, weight);
    }
    file.close();
}

bool Graph::insertEntity(std::string id, std::string name, std::string type) {
    if(nodes.find(id) != nodes.end()) {
        // entity exists, update it
        nodes[id]->setName(name);
        nodes[id]->setType(type);
    } else {
        // create new entity
        Node* newNode = new Node(id, name, type);
        nodes[id] = newNode;
    }
    return true;
}

Edge* Graph::findEdge(Node* n1, Node* n2) {
    // check if edge already exists between these two nodes
    std::vector<Edge*> n1Edges = n1->getEdges();
    for(int i = 0; i < n1Edges.size(); i++) {
        Node* other = n1Edges[i]->getOtherEnd(n1);
        if(other == n2) {
            return n1Edges[i];
        }
    }
    return nullptr;
}

bool Graph::insertRelationship(std::string sourceId, std::string label, std::string destId, double weight) {
    // check if both nodes exist
    if(nodes.find(sourceId) == nodes.end() || nodes.find(destId) == nodes.end()) {
        return false;
    }
    
    Node* source = nodes[sourceId];
    Node* dest = nodes[destId];
    
    // check if edge already exists
    Edge* existing = findEdge(source, dest);
    if(existing != nullptr) {
        // update existing edge
        existing->setLabel(label);
        existing->setWeight(weight);
        return true;
    }
    
    // create new edge (undirected)
    Edge* newEdge = new Edge(source, dest, label, weight);
    edges.push_back(newEdge);
    source->addEdge(newEdge);
    dest->addEdge(newEdge);
    return true;
}

bool Graph::deleteNode(std::string id) {
    // check for illegal characters
    for(int i = 0; i < id.length(); i++) {
        char c = id[i];
        if(!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))) {
            throw illegal_exception();
        }
    }
    
    if(nodes.find(id) == nodes.end()) {
        return false;
    }
    
    Node* nodeToDelete = nodes[id];
    
    // remove all edges connected to this node
    std::vector<Edge*> nodeEdges = nodeToDelete->getEdges();
    for(int i = 0; i < nodeEdges.size(); i++) {
        Edge* edge = nodeEdges[i];
        Node* other = edge->getOtherEnd(nodeToDelete);
        other->removeEdge(edge);
        
        // remove from graph edges vector
        for(int j = 0; j < edges.size(); j++) {
            if(edges[j] == edge) {
                edges.erase(edges.begin() + j);
                break;
            }
        }
        delete edge;
    }
    
    // remove node from map
    nodes.erase(id);
    delete nodeToDelete;
    return true;
}

std::vector<std::string> Graph::printAdjacent(std::string id) {
    // check for illegal characters
    for(int i = 0; i < id.length(); i++) {
        char c = id[i];
        if(!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))) {
            throw illegal_exception();
        }
    }
    
    std::vector<std::string> result;
    
    if(nodes.find(id) == nodes.end()) {
        return result; // empty vector means failure
    }
    
    Node* node = nodes[id];
    std::vector<Edge*> nodeEdges = node->getEdges();
    
    for(int i = 0; i < nodeEdges.size(); i++) {
        Node* neighbor = nodeEdges[i]->getOtherEnd(node);
        result.push_back(neighbor->getId());
    }

    // need to sort to match test case:
    std::sort(result.begin(), result.end());
    
    return result;
}
// Helper function for DFS
void findPathDFS(Node* current, Node* endNode, double currentWeight, 
                 std::vector<std::string>& currentPath, double& maxWeight, 
                 std::vector<std::string>& bestPath, std::map<std::string, bool>& visited) {
    
    std::string currId = current->getId();
    visited[currId] = true;
    currentPath.push_back(currId);

    if (current == endNode) {
        // Path found
        if (currentWeight > maxWeight) {
            maxWeight = currentWeight;
            bestPath = currentPath;
        }
    } else {
        // Continue searching
        std::vector<Edge*> edges = current->getEdges();
        for (Edge* e : edges) {
            Node* neighbor = e->getOtherEnd(current);
            if (!visited[neighbor->getId()]) {
                findPathDFS(neighbor, endNode, currentWeight + e->getWeight(), 
                            currentPath, maxWeight, bestPath, visited);
            }
        }
    }

    // acktrack: unmark visited so this node can be used in other paths
    visited[currId] = false;
    currentPath.pop_back();
}

std::vector<std::string> Graph::findPath(std::string id1, std::string id2, double& totalWeight) {
    // check for illegal characters in both IDs
    for(int i = 0; i < id1.length(); i++) {
        char c = id1[i];
        if(!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))) {
            throw illegal_exception();
        }
    }
    for(int i = 0; i < id2.length(); i++) {
        char c = id2[i];
        if(!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))) {
            throw illegal_exception();
        }
    }
    
    std::vector<std::string> result;
    totalWeight = 0;
    
    if(isEmpty() || nodes.find(id1) == nodes.end() || nodes.find(id2) == nodes.end()) {
        return result;
    }
    
    // special case: same node
    if(id1 == id2) {
        result.push_back(id1);
        return result;
    }
    
    // modified Dijkstra for MAXIMUM weight path instead of minimum
    std::map<std::string, double> maxWeight;
    std::map<std::string, std::string> previous;
    std::map<std::string, bool> visited;
    
    // initialize
    for(auto it = nodes.begin(); it != nodes.end(); it++) {
        maxWeight[it->first] = -std::numeric_limits<double>::infinity();
        visited[it->first] = false;
    }
    maxWeight[id1] = 0.0;
    
    // priority queue - use negative for max heap behavior
    std::priority_queue<std::pair<double, std::string>> pq;
    pq.push(std::make_pair(0.0, id1));
    
    while(!pq.empty()) {
        std::string current = pq.top().second;
        double currentWeight = pq.top().first;
        pq.pop();
        
        // skip if already visited with better weight
        if(visited[current]) {
            continue;
        }
        visited[current] = true;
        
        if(current == id2) {
            break;
        }
        
        Node* currentNode = nodes[current];
        std::vector<Edge*> currentEdges = currentNode->getEdges();
        
        for(int i = 0; i < currentEdges.size(); i++) {
            Node* neighbor = currentEdges[i]->getOtherEnd(currentNode);
            std::string neighborId = neighbor->getId();
            
            if(visited[neighborId]) {
                continue;
            }
            
            double edgeWeight = currentEdges[i]->getWeight();
            double newWeight = currentWeight + edgeWeight;
            
            if(newWeight > maxWeight[neighborId]) {
                maxWeight[neighborId] = newWeight;
                previous[neighborId] = current;
                pq.push(std::make_pair(newWeight, neighborId));
            }
        }
    }
    
    // check if path exists
    if(maxWeight[id2] == -std::numeric_limits<double>::infinity()) {
        return result; // no path
    }
    
    // reconstruct path backwards
    std::string current = id2;
    while(current != id1) {
        result.push_back(current);
        if(previous.find(current) == previous.end()) {
            result.clear();
            return result;
        }
        current = previous[current];
    }
    result.push_back(id1);
    
    // reverse to get correct order
    std::reverse(result.begin(), result.end());
    
    totalWeight = maxWeight[id2];
    return result;
}

std::vector<std::string> Graph::findHighest(double& totalWeight) {
    std::vector<std::string> result;
    
    if(edges.size() == 0) {
        return result;
    }
    
    double maxPathWeight = -1;
    std::string bestStart = "";
    std::string bestEnd = "";
    
    // brute force - check all pairs
    // this is slow but easier to implement correctly
    for(auto it1 = nodes.begin(); it1 != nodes.end(); it1++) {
        for(auto it2 = nodes.begin(); it2 != nodes.end(); it2++) {
            if(it1->first != it2->first) {
                double pathWeight = 0;
                std::vector<std::string> path = findPath(it1->first, it2->first, pathWeight);
                if(path.size() > 0 && pathWeight > maxPathWeight) {
                    maxPathWeight = pathWeight;
                    bestStart = it1->first;
                    bestEnd = it2->first;
                }
            }
        }
    }
    
    if(bestStart != "") {
        result = findPath(bestStart, bestEnd, totalWeight);
    }
    
    return result;
}

std::vector<std::string> Graph::findAll(std::string fieldType, std::string fieldString) {
    std::vector<std::string> result;
    
    for(auto it = nodes.begin(); it != nodes.end(); it++) {
        Node* node = it->second;
        bool match = false;
        
        if(fieldType == "name") {
            if(node->getName() == fieldString) {
                match = true;
            }
        } else if(fieldType == "type") {
            if(node->getType() == fieldString) {
                match = true;
            }
        }
        
        if(match == true) {
            result.push_back(node->getId());
        }
    }
    
    return result;
}

Node* Graph::getNode(std::string id) {
    if(nodes.find(id) != nodes.end()) {
        return nodes[id];
    }
    return nullptr;
}

bool Graph::isEmpty() {
    if(nodes.size() == 0) {
        return true;
    } else {
        return false;
    }
}