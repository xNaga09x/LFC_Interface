#include "Node.h"
#include <cmath>
#include <QPoint>

Node::Node()
{
    value = 0;
    x = -1;
    y = -1;
    InitialState = false;
    FinalState = false;
    coordinate.setX(0);
    coordinate.setY(0);
}

void Node::setValue(int value)
{
    this->value = value;
}

int Node::getValue()
{
    return value;
}

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

bool Node::isOverlapping(const Node& other) const
{
    int dx = coordinate.x() - other.coordinate.x();
    int dy = coordinate.y() - other.coordinate.y();
    int distance = std::sqrt(dx * dx + dy * dy);

    int radiusSum = 20;

    return distance <= radiusSum;
}

void Node::setFinalState(bool state)
{
    FinalState = state;
}

const bool Node::getFinalState()
{
    return FinalState;
}

void Node::setInitialState(bool state)
{
    InitialState = state;
}

const bool Node::getInitialState()
{
    return InitialState;
}
