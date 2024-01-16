#include "Graph.h"
#include <fstream>
#include <QFile>
#include <QDebug>
Graph::Graph()
{
}
Graph::Graph(const Graph& other)
{

    for (Node* node : other.nodes)
    {
        nodes.push_back(new Node(*node));
    }

    for (Arch* arch : other.arches)
    {
        Arch* newArch = new Arch(arch->getFirstNode(), arch->getSecondNode());
        arches.push_back(newArch);
    }
}
Graph::Graph(const std::vector<Node*>& initialNodes) : nodes(initialNodes)
{
}
std::vector<Node*>& Graph::getNodes()
{
    return nodes;
}

std::vector<Arch*>& Graph::getArches()
{
    return arches;
}

std::vector<APDArch*>& Graph::getAPDArches()
{
    return apd_arches;
}

void Graph::addNode(QPoint p)
{
    Node* n = new Node;
    n->setCoordinate(p);
    n->setValue(nodes.size());

    nodes.push_back(n);

    size_t numNodes = nodes.size();
}

bool Graph::isArcUnique(Node* source, Node* target)
{
    for (Arch* arc : arches)
    {
        if (arc->getFirstNode() == source && arc->getSecondNode() == target)
        {
            return false;
        }
    }
    return true;
}
bool Graph::arcExists(Node* node1, Node* node2) const
{
    for (Arch* a : arches)
    {
        if (a->getFirstNode() == node1 && a->getSecondNode() == node2)
        {
            return true;
        }
    }
    return false;
}
void Graph::addArch(Node* firstNode, Node* secondNode)
{
    if (isArcUnique(firstNode, secondNode))
    {
        Arch* arc = new Arch(firstNode, secondNode);
        arc->addElement("A");
        arches.push_back(arc);
    }
}

void Graph::addArch(Node* firstNode, Node* secondNode, QString& label)
{
    if (firstNode && secondNode)
    {
        Arch* arch = new Arch(firstNode, secondNode, label);
        arches.push_back(arch);
    }
}

void Graph::addAPDArch(Node* firstNode, Node* secondNode, std::vector<QString>& labels)
{
    if (firstNode && secondNode)
    {
        APDArch* arch = new APDArch(firstNode, secondNode, labels);
        apd_arches.push_back(arch);
    }
}

void Graph::removeNode(Node* node)
{

    auto it = std::remove(nodes.begin(), nodes.end(), node);
    nodes.erase(it, nodes.end());


    std::vector<Arch*>::iterator it1;
    it1 = arches.begin();
    while (it1 != arches.end())
    {
        if ((*it1)->getFirstNode() == node || (*it1)->getSecondNode() == node)
        {
            it1 = arches.erase(it1);
        }
        else
        {
            ++it1;
        }
    }
}

