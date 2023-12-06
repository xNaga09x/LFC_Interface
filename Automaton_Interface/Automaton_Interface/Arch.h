#ifndef ARCH_H
#define ARCH_H

#include "Node.h"
#include<QString>
class Arch
{
    Node* firstNode;
    Node* secondNode;
    QString label;//adaugarea unui membru pentru eticheta(informatai de pe arc)
    QStringList elements;//adaugarea unui liste de elmente pe arc
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

    Node* getFirstNode() { return firstNode; }
    Node* getSecondNode() { return secondNode; }
    QString getLabel() const;//Metoda pentru a obtine eticheta
    void setLabel(const QString& newLabel);//Metoda pentru a seta eticheta
    const QStringList& getElements() const;//Metoda pentru a obtine o lista de elemente
    void addElement(const QString& element);//Metoda pentru a adauga un element
};

#endif // ARCH_H
