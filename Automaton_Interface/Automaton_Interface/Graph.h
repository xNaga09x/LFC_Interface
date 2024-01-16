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
    Graph(const std::vector<Node*>& initialNodes);
    std::vector<Node*>& getNodes();
    std::vector<Arch*>& getArches();
    std::vector<APDArch*>& getAPDArches();
    void addNode(QPoint p);

    void addArch(Node* firstNode, Node* secondNode, QString& label);
    void addAPDArch(Node* firstNode, Node* secondNode, std::vector<QString>& labels);
    void addArch(Node* firstNode, Node* secondNode);
    bool isArcUnique(Node* source, Node* target);
    bool arcExists(Node* node1, Node* node2) const;
    void removeNode(Node* node);
    Node* getNodeByCoordinates(const QPoint& coordinates) const {
        auto it = std::find_if(nodes.begin(), nodes.end(),
            [coordinates](const Node* node) { return node->getCoordinate() == coordinates; });
        return (it != nodes.end()) ? *it : nullptr;
    }

    Node* getNodeById(int nodeId)  {
        auto it = std::find_if(nodes.begin(), nodes.end(),
            [nodeId](const Node* node) { return node->getId() == nodeId; });
        return (it != nodes.end()) ? *it : nullptr;
    }

 
    Arch* getArcByNodes(const Node* sourceNode, const Node* targetNode) const {
        auto it = std::find_if(arches.begin(), arches.end(),
            [sourceNode, targetNode](const Arch* arc) {
                return (arc->getFirstNode() == sourceNode && arc->getSecondNode() == targetNode);
            });
        return (it != arches.end()) ? *it : nullptr;
    }
};

#endif