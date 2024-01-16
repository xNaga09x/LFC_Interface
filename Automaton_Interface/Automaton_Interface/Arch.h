#ifndef ARCH_H
#define ARCH_H

#include "Node.h"
#include<QString>
class Arch
{
    Node* firstNode;
    Node* secondNode;
    QString label;
    QStringList elements;
public:
    Arch();
    Arch(Node* first, Node* second)
        : firstNode(first), secondNode(second)
    {
    }
    Arch(const Arch& other)
        : firstNode(new Node(*(other.firstNode))), secondNode(new Node(*(other.secondNode)))
    {
    }
    Arch(Node* firstNode, Node* secondNode, const QString& label);

    Node* getFirstNode()const { return firstNode; }
    Node* getSecondNode()const { return secondNode; }
    QString getLabel() const;
    void setLabel(const QString& newLabel);
    const QStringList& getElements() const;
    void addElement(const QString& element);
    QColor color;
};

#endif 
