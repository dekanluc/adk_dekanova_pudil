#ifndef DRAW_H
#define DRAW_H

#include <vector>
#include <QWidget>
#include <QtGui>
#include "algorithms.h"


using namespace std;
class Draw : public QWidget
{
    Q_OBJECT
private:
    boolean draw_mode;  //true = polygon, false = point
    //std::vector <QPoint> polygon;
    QPointF q;
    vector<int> result;
    vector<QPolygonF> polygons;


public:
    explicit Draw(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);
    void changeMode(){draw_mode = !draw_mode;} 
    QPointF &getPoint(){return q;}
    void erasePoints();
    void txtFile(string &path);
    void setResult(vector<int> res){result=res;}
    vector<QPolygonF> getPolygons(){return polygons;}
};

#endif // DRAW_H
