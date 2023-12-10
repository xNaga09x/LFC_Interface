#ifndef NODE_H
#define NODE_H

#include <QPainter>
class Node
{
    int value;
    QPoint coordinate;
    int x;
    int y;
    bool isInitialState;
    bool isFinalState;
public:
    Node();
    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }
    void setValue(int value);
    void setCoordinate(QPoint);
    //    void setX(int x);
    //    void setY(int y);
    //    void setCoordinate(int x, int y);
    int getValue();
    QPoint getCoordinate()
    {
        return coordinate;
    }
    int getX();
    int getY();
    bool isOverlapping(const Node& other) const;
    bool operator==(const Node& other) const
    {
        return (value == other.value) && (coordinate == other.coordinate);
    }
};

#endif // NODE_H
