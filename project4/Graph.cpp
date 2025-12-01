/*
CITATION:
Used ChatGPT (chat.openai.com) to understand Dijkstra's algorithm for
finding highest weight paths (had to modify it since we want MAX weight
not min distance). Also asked about how to check if a string contains only
alphanumeric characters for the illegal argument checks. 
*/

#include "Graph.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <limits>
#include <tuple>

Graph::Graph() {
    mapSize = 15013; 
    nodeMap.resize(mapSize);
}

Graph::~Graph() {
    for(int i = 0; i < nodes.size(); i++) {
        delete nodes[i];
    }
    for(int i = 0; i < edges.size(); i++) {
        delete edges[i];
    }
}

unsigned int Graph::hash(const std::string& id) {
    unsigned int hashVal = 5381;
    for (char c : id) {
        hashVal = ((hashVal << 5) + hashVal) + c;
    }
    return hashVal % mapSize;
}

Node* Graph::findNodeById(const std::string& id) {
    unsigned int h = hash(id);
    for(Node* node : nodeMap[h]) {
        if(node->getId() == id) {
            return node;
        }
    }
    return nullptr;
}

bool Graph::isValidId(const std::string& id) {
    for(int i = 0; i < id.length(); i++) {
        char c = id[i];
        if(!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))) {
            return false;
        }
    }
    return true;
}

void Graph::loadEntities(std::string filename) {
    std::ifstream file(filename);
    if(!file.is_open()) return;
    
    std::string id, name, type;
    while(file >> id >> name >> type) {
        insertEntity(id, name, type);
    }
    file.close();
}

void Graph::loadRelationships(std::string filename) {
    std::ifstream file(filename);
    if(!file.is_open()) return;
    
    std::string sourceId, label, destId;
    double weight;
    while(file >> sourceId >> label >> destId >> weight) {
        insertRelationship(sourceId, label, destId, weight);
    }
    file.close();
}

bool Graph::insertEntity(std::string id, std::string name, std::string type) {
    Node* existing = findNodeById(id);
    if(existing != nullptr) {
        existing->setName(name);
        existing->setType(type);
    } else {
        Node* newNode = new Node(id, name, type);
        nodes.push_back(newNode);
        
        unsigned int h = hash(id);
        nodeMap[h].push_back(newNode);
    }
    return true;
}

Edge* Graph::findEdge(Node* n1, Node* n2) {
    std::vector<Edge*> n1Edges = n1->getEdges();
    for(int i = 0; i < n1Edges.size(); i++) {
        Node* other = n1Edges[i]->getOtherEnd(n1);
        if(other == n2) {
            return n1Edges[i];
        }
    }
    return nullptr;
}

bool Graph::insertRelationship(std::string sourceId, std::string label, 
                               std::string destId, double weight) {
    Node* source = findNodeById(sourceId);
    Node* dest = findNodeById(destId);
    
    if(source == nullptr || dest == nullptr) return false;
    
    Edge* existing = findEdge(source, dest);
    if(existing != nullptr) {
        existing->setLabel(label);
        existing->setWeight(weight);
        return true;
    }
    
    Edge* newEdge = new Edge(source, dest, label, weight);
    edges.push_back(newEdge);
    source->addEdge(newEdge);
    dest->addEdge(newEdge);
    return true;
}

bool Graph::deleteNode(std::string id) {
    if(!isValidId(id)) throw illegal_exception();
    
    Node* nodeToDelete = findNodeById(id);
    if(nodeToDelete == nullptr) return false;
    
    // Get copy of edges since we'll be modifying the node's edge list
    std::vector<Edge*> nodeEdges = nodeToDelete->getEdges();
    
    // Remove all edges connected to this node
    for(int i = 0; i < nodeEdges.size(); i++) {
        Edge* edge = nodeEdges[i];
        Node* other = edge->getOtherEnd(nodeToDelete);
        other->removeEdge(edge);
        
        // Remove from main edges vector
        for(int j = 0; j < edges.size(); j++) {
            if(edges[j] == edge) {
                edges[j] = edges.back();
                edges.pop_back();
                break;
            }
        }
        delete edge;
    }
    
    // Remove from hash table
    unsigned int h = hash(id);
    for(int i = 0; i < nodeMap[h].size(); i++) {
        if(nodeMap[h][i] == nodeToDelete) {
            nodeMap[h][i] = nodeMap[h].back();
            nodeMap[h].pop_back();
            break;
        }
    }
    
    // Remove from nodes vector
    for(int i = 0; i < nodes.size(); i++) {
        if(nodes[i] == nodeToDelete) {
            nodes[i] = nodes.back();
            nodes.pop_back();
            break;
        }
    }
    
    delete nodeToDelete;
    return true;
}

std::vector<std::string> Graph::printAdjacent(std::string id) {
    if(!isValidId(id)) throw illegal_exception();
    
    std::vector<std::string> result;
    Node* node = findNodeById(id);
    if(node == nullptr) return result;
    
    std::vector<Edge*> nodeEdges = node->getEdges();
    for(int i = 0; i < nodeEdges.size(); i++) {
        Node* neighbor = nodeEdges[i]->getOtherEnd(node);
        result.push_back(neighbor->getId());
    }
    
    return result;
}

std::vector<std::string> Graph::findPath(std::string id1, std::string id2, double& totalWeight) {
    if(!isValidId(id1) || !isValidId(id2)) throw illegal_exception();
    
    std::vector<std::string> result;
    totalWeight = 0;
    
    Node* start = findNodeById(id1);
    Node* end = findNodeById(id2);
    
    if(isEmpty() || start == nullptr || end == nullptr) return result;
    
    if(id1 == id2) {
        result.push_back(id1);
        return result;
    }

    int n = nodes.size();

    // Build a sorted mapping so we can quickly find node indices
    std::vector<std::pair<Node*, int>> nodeIndices;
    nodeIndices.reserve(n);
    for(int i = 0; i < n; i++) {
        nodeIndices.push_back(std::make_pair(nodes[i], i));
    }
    std::sort(nodeIndices.begin(), nodeIndices.end());

    // Find start and end indices with proper error checking
    auto startIt = std::lower_bound(nodeIndices.begin(), nodeIndices.end(), std::make_pair(start, -1));
    if(startIt == nodeIndices.end() || startIt->first != start) {
        return result;
    }
    int startIdx = startIt->second;
    
    auto endIt = std::lower_bound(nodeIndices.begin(), nodeIndices.end(), std::make_pair(end, -1));
    if(endIt == nodeIndices.end() || endIt->first != end) {
        return result;
    }
    int endIdx = endIt->second;

    std::vector<double> maxDist(n, -std::numeric_limits<double>::infinity());
    std::vector<int> parent(n, -1);
    std::vector<bool> visited(n, false);
    std::vector<bool> inQueue(n, false);

    maxDist[startIdx] = 0.0;
    
    // Using a heap to implement priority queue
    std::vector<std::tuple<double, int>> pq;
    pq.reserve(n);
    pq.push_back(std::make_tuple(0.0, startIdx));
    inQueue[startIdx] = true;
    
    while(!pq.empty()) {
        std::pop_heap(pq.begin(), pq.end());
        std::tuple<double, int> top = pq.back();
        pq.pop_back();
        
        double d = std::get<0>(top);
        int uIdx = std::get<1>(top);
        
        inQueue[uIdx] = false;
        
        if(visited[uIdx]) continue;
        visited[uIdx] = true;
        
        if(uIdx == endIdx) break;
        
        Node* uNode = nodes[uIdx];
        std::vector<Edge*> adj = uNode->getEdges();
        
        for(int i = 0; i < adj.size(); i++) {
            Edge* e = adj[i];
            Node* vNode = e->getOtherEnd(uNode);
            double weight = e->getWeight();
            
            auto it = std::lower_bound(nodeIndices.begin(), nodeIndices.end(), std::make_pair(vNode, -1));
            if(it == nodeIndices.end() || it->first != vNode) continue;
            int vIdx = it->second;
            
            if(!visited[vIdx]) {
                double newDist = maxDist[uIdx] + weight;
                if(newDist > maxDist[vIdx]) {
                    maxDist[vIdx] = newDist;
                    parent[vIdx] = uIdx;
                    
                    // Only add to queue if not already in queue
                    if(!inQueue[vIdx]) {
                        pq.push_back(std::make_tuple(maxDist[vIdx], vIdx));
                        std::push_heap(pq.begin(), pq.end());
                        inQueue[vIdx] = true;
                    }
                }
            }
        }
    }
    
    if(maxDist[endIdx] == -std::numeric_limits<double>::infinity()) return result;
    
    // Build the path by backtracking through parent pointers
    int curr = endIdx;
    while(curr != -1) {
        result.push_back(nodes[curr]->getId());
        curr = parent[curr];
    }
    
    std::reverse(result.begin(), result.end());
    totalWeight = maxDist[endIdx];
    
    return result;
}

std::vector<std::string> Graph::findHighest(double& totalWeight) {
    std::vector<std::string> bestPath;
    totalWeight = 0.0;
    
    if(nodes.empty() || edges.empty()) return bestPath;
    
    double globalMax = -1.0;
    
    // Check all pairs of nodes to find highest weight path
    for(int i = 0; i < nodes.size(); i++) {
        for(int j = i + 1; j < nodes.size(); j++) {
            double w = 0;
            std::vector<std::string> p = findPath(nodes[i]->getId(), nodes[j]->getId(), w);
            
            if(!p.empty() && w > globalMax) {
                globalMax = w;
                bestPath = p;
            }
        }
    }
    
    if(globalMax != -1.0) {
        totalWeight = globalMax;
        
        // Make sure output is in alphabetical order
        if (bestPath.size() > 1) {
            if (bestPath.front() > bestPath.back()) {
                std::reverse(bestPath.begin(), bestPath.end());
            }
        }
    }
    
    return bestPath;
}

std::vector<std::string> Graph::findAll(std::string fieldType, std::string fieldString) {
    std::vector<std::string> result;
    for(int i = 0; i < nodes.size(); i++) {
        if(fieldType == "name" && nodes[i]->getName() == fieldString) {
            result.push_back(nodes[i]->getId());
        } else if(fieldType == "type" && nodes[i]->getType() == fieldString) {
            result.push_back(nodes[i]->getId());
        }
    }
    return result;
}

Node* Graph::getNode(std::string id) {
    return findNodeById(id);
}

bool Graph::isEmpty() {
    return nodes.empty();
}