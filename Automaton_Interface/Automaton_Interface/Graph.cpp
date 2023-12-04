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
std::vector<Node*>& Graph::getNodes()
{
    return nodes;
}

std::vector<Arch*>& Graph::getArches()
{
    return arches;
}

void Graph::addNode(QPoint p)
{
    Node* n = new Node;
    n->setCoordinate(p);
    n->setValue(nodes.size());

    // Ad?uga?i noul nod în lista de noduri
    nodes.push_back(n);

    // Extinde?i matricea de adiacen?? cu o nou? linie ?i o nou? coloan? cu zero-uri pentru noul nod
    size_t numNodes = nodes.size();
}
//Pentru punctul 3
bool Graph::isArcUnique(Node* source, Node* target)
{
    // Verifica?i dac? arcul nu exist? deja
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
            // Verifica?i dac? exist? un arc de la node1 la node2
            return true;
        }
    }
    return false;
}
void Graph::addArch(Node* firstNode, Node* secondNode)
{
    // Verifica?i dac? arcul este unic ?i ad?uga?i-l
    if (isArcUnique(firstNode, secondNode))
    {
        Arch* arc = new Arch(firstNode, secondNode);
        arc->addElement("A");
        /*arc->addElement("B");
        arc->addElement("C");*/
        arches.push_back(arc);
    }
}
void Graph::removeNode(Node* node)
{
    // Stergerea nodului din vector
    auto it = std::remove(nodes.begin(), nodes.end(), node);
    nodes.erase(it, nodes.end());

    // Stergerea arcelor conectate de nod
    std::vector<Arch*>::iterator it1;
    it1 = arches.begin();
    while (it1 != arches.end())
    {
        if ((*it1)->getFirstNode() == node || (*it1)->getSecondNode() == node)
        {
            //delete* it1;
            it1 = arches.erase(it1);
        }
        else
        {
            ++it1;
        }
    }
}

