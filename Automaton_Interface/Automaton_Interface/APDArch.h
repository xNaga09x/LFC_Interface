#pragma once

#include "Node.h"
#include<QString>
class APDArch
{
    Node* firstNode;
    Node* secondNode;
    int noLabels;
    std::vector<QString> labels;
    QStringList elements;
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
    std::vector<QString> getLabels() const;
    const QStringList& getElements() const;
    const int& getNoLabels() const;
    void addElement(const QString& element);

    void setLabels(const std::vector<QString>& newLabels);
    int setNoLabels(const int& number);
};


