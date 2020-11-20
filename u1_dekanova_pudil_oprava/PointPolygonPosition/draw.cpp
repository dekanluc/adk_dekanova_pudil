#include "draw.h"
#include <fstream>
#include <iostream>
#include <QtGui>
#include <QWidget>
#include <vector>
#include "algorithms.h"
#include "widget.h"
#include <QWidget>
#include <cmath>
#include <QVector>

using namespace std;

void Draw::txtFile(std::string &path)
{
    polygons.clear();
    result.clear();
    repaint();
    int id;
    int i = 0;
    double x;
    double y;

    QPolygonF polygon;
    QPointF point;

    //Load data from .txt file
    ifstream file(path);


   if (file.is_open())
      {
        while (file >> id >> x >> y )
        {

            if (i == id)
            {
                // pushing back the current polygon
                point.setX(x);
                point.setY(y);
                polygon.push_back(point);
            } else {
                // creating of a new polygon
                polygons.push_back(polygon);
                polygon.clear();
                // adding of a new point to the new polygon
                point.setX(x);
                point.setY(y);
                polygon.push_back(point);
                i = 1;
            }
            i++;
          }
             //saving
             polygons.push_back(polygon);

            //erasing of all the polygons
            polygon.clear();

            //closing file
             file.close();
}

    else cout << "Unable to open file";
      }

Draw::Draw(QWidget *parent) : QWidget(parent)
{
        //Set default draw mode
        draw_mode = true;

        //Draw point outside Canvas
        q.setX(-50);
        q.setY(-50);
}

void Draw::mousePressEvent(QMouseEvent *e)
{
        //Cursor position
        double x = e->pos().x();
        double y = e->pos().y();

    //Mode = polygon
        if (draw_mode)
    {
        //Create new point
        QPoint p(x, y);

    }

        //Mode = q
        else
    {
        //Change coordinates of q
        q.setX(x);
        q.setY(y);

    repaint();
}
}
void Draw::paintEvent(QPaintEvent *e)
{
    //Start drawing

    QPainter painter(this);
    painter.begin(this);

        //setting of brush parameters
        QBrush brush;

        brush.setStyle(Qt::DiagCrossPattern);
        brush.setColor(Qt::magenta);

        //setting path
        QPainterPath path;
        //declaration of the filled polygon
        QPolygonF colored_polygon;

        //point inside the polygon -> result =1
        for(unsigned int i = 0; i < result.size(); i++)
        {
            if(result[i] == 1|| result[i] == -1)
            {
                colored_polygon << polygons[i];
                path.addPolygon(colored_polygon);
                painter.fillPath(path, brush);
                painter.drawPolygon(colored_polygon);
                colored_polygon.clear();
            }

        }
        //Draw loaded polygons
            for(unsigned int i = 0; i < polygons.size(); i++)
            {
                QPolygonF poly = polygons[i];
                painter.drawPolygon(poly);
            }

        //Draw q point
        int r = 5;

        painter.drawEllipse(q.x()-r, q.y()-r, 2 * r, 2 *r);


        painter.end();
}

// Clear all points
void Draw::erasePoints()
{

      polygons.clear();
      q.setX(-100);
      q.setY(-100);
      repaint();

}
