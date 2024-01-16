#ifndef NODE_H
#define NODE_H

#include <QPainter>
class Node
{
    int value;
    QPoint coordinate;
    int x;
    int y;
    bool InitialState;
    bool FinalState;
public:
    Node();
    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }
    void setValue(int value);
    void setCoordinate(QPoint);
    int getValue();
    QPoint getCoordinate() const
    {
        return coordinate;
    }
    int getId() const {
        return value;
    }
    int getX();
    int getY();
    bool isOverlapping(const Node& other) const;
    bool operator==(const Node& other) const
    {
        return (value == other.value) && (coordinate == other.coordinate);
    }
    void setFinalState(bool state);
    const bool getFinalState();
    void setInitialState(bool state);
    const bool getInitialState();
    QColor color;
};

#endif // NODE_H
