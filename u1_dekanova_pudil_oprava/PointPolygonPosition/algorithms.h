#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>
#include <QPoint>

class Algorithms
{
public:
    Algorithms();

    int getPointLinePosition(QPointF &q, QPointF &p1, QPointF &p2);
    double getAngle(QPointF &q1, QPointF &q2, QPointF &q3, QPointF &q4);
    int getPositionWinding(QPointF &q, std::vector<QPointF> &pol);
    int getPositionRay(QPointF &q, std::vector<QPointF> &pol);
};

#endif // ALGORITHMS_H
