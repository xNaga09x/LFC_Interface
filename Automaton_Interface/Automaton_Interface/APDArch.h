#pragma once

#include "Node.h"
#include<QString>
class APDArch
{
    Node* firstNode;
    Node* secondNode;
    int noLabels;
    std::vector<QString> labels;//adaugarea unui membru pentru eticheta(informatai de pe arc)
    QStringList elements;//adaugarea unui liste de elmente pe arc
public:
    APDArch();
    APDArch(Node* first, Node* second)
        : firstNode(first), secondNode(second)
    {
    }
    APDArch(const APDArch& other)
        : firstNode(new Node(*(other.firstNode))), secondNode(new Node(*(other.secondNode)))
    {
    }
    APDArch(Node* firstNode, Node* secondNode, const std::vector<QString>& labels);

    Node* getFirstNode() { return firstNode; }
    Node* getSecondNode() { return secondNode; }
    std::vector<QString> getLabels() const;//Metoda pentru a obtine eticheta
    const QStringList& getElements() const;//Metoda pentru a obtine o lista de elemente
    const int& getNoLabels() const;
    void addElement(const QString& element);//Metoda pentru a adauga un element

    void setLabels(const std::vector<QString>& newLabels);//Metoda pentru a seta eticheta
    int setNoLabels(const int& number);
};


