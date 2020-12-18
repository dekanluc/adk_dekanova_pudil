#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <vector>
#include <QtGui>

class Draw : public QWidget
{
    Q_OBJECT
private:
    QPolygon points;     //Input points
    QPolygon ch;                    //Convex Hull
    QPolygon polygon;

public:
    explicit Draw(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);
    QPolygon & getPoints(){return points;}
    QPolygon & getCH() {return ch;}
    void setCH(QPolygon &ch_){ch = ch_;}
    void setPoints(QPolygon Points){points = Points;}
    QPolygon & getPolygon (){return polygon;}
    QPolygon randompoints(int width, int height, int n);
    QPolygon circle(int width, int height, int n);
    QPolygon grid(int width, int height, int n);
    QPolygon ellips(int width, int height, int n);
    QPolygon square(int width, int height, int n);
    QPolygon star(int width, int height, int n);
signals:

};

#endif // DRAW_H

