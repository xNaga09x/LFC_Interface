#ifndef GRAPH_H
#define GRAPH_H

#include "Arch.h"
#include "APDArch.h"
#include <fstream>
#include <algorithm>
#include<vector>
class Graph
{
    std::vector<Node*> nodes;
    std::vector<Arch*> arches;
    std::vector<APDArch*> apd_arches;
public:
    Graph();
    Graph(const Graph& other);
    std::vector<Node*>& getNodes();
    std::vector<Arch*>& getArches();
    std::vector<APDArch*>& getAPDArches();
    void addNode(QPoint p);
    //void addArch(Arch *arc);
    void addArch(Node* firstNode, Node* secondNode, QString& label);
    void addAPDArch(Node* firstNode, Node* secondNode, std::vector<QString>& labels);
    void addArch(Node* firstNode, Node* secondNode);
    bool isArcUnique(Node* source, Node* target);
    bool arcExists(Node* node1, Node* node2) const;
    void removeNode(Node* node);
};

#endif // GRAPH_H