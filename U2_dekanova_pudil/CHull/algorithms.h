#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QtGui>
#include <vector>

class Algorithms
{
public:
    Algorithms();

    double getAngle(QPoint &p1, QPoint &p2, QPoint &p3, QPoint &p4);
    int getPointLinePosition(QPoint &q, QPoint &p1, QPoint &p2);
    double getPointLineDist(QPoint &a, QPoint &p1, QPoint &p2);
    QPolygon jarvis(QPolygon &points);
    QPolygon qhull(QPolygon &points);
    void qh(int s, int e, QPolygon &points, QPolygon &ch);
    QPolygon sweepLine(QPolygon &points);
    double getLength(QPoint &p1, QPoint &p2);

};

#endif // ALGORITHMS_H
