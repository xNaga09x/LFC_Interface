#include "Node.h"
#include <cmath>
#include <QPoint>
//setarea default a nodului
//constructorul de initializare
Node::Node()
{
    value = 0;
    x = -1;
    y = -1;
    isInitialState = false;
    isFinalState = false;
}

void Node::setValue(int value)
{
    this->value = value;
}

int Node::getValue()
{
    return value;
}
//setter-ul pt coordonate,pt un punct x si y
void Node::setCoordinate(QPoint p)
{
    coordinate = p;
}

int Node::getX()
{
    return coordinate.x();
}
int Node::getY()
{
    return coordinate.y();
}
//Pt punctul 2
bool Node::isOverlapping(const Node& other) const
{
    int dx = coordinate.x() - other.coordinate.x();
    int dy = coordinate.y() - other.coordinate.y();
    int distance = std::sqrt(dx * dx + dy * dy); // Calcularea distantei euclidiana

    int radiusSum = 20; // Suma razelor nodurilor (in acest exemplu, 20 de unitati)

    return distance <= radiusSum;
}
