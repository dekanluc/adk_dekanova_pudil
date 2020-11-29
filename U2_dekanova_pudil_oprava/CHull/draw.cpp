#include "draw.h"
#include <cmath>

Draw::Draw(QWidget *parent) : QWidget(parent)
{

}
void Draw::mousePressEvent(QMouseEvent *e)
{
    //Get coordinates
    int x = e->x();
    int y = e->y();

    //Add point to the list
    QPoint p(x,y);
    points.push_back(p);

    //Repaint
    repaint();

}


void Draw::paintEvent(QPaintEvent *e)
{
    //Start draw
    QPainter qp(this);
    qp.begin(this);
    //Draw points
    int r = 2;
    for(int i = 0; i < points.size(); i++)
    {
        qp.drawEllipse(points[i].x() - r,points[i].y() - r, 2 * r, 2 * r);
    }

    //Draw polygon
    QPen paintpen(Qt::red);
    paintpen.setWidth(2);
    qp.setPen(paintpen);
    qp.drawPolygon(ch);

    //End draw
    qp.end();
}


QPolygon Draw::randompoints(int width, int height, int n)
{
    QPolygon generatedpoints;
    QPoint a;
    // generating random points with defined height and width
    for(int i = 0; i<n; i++)
        {
        a.setX(rand()%width);
        a.setY(rand()%height);
        generatedpoints.push_back(a);
        }

    return generatedpoints;
}

QPolygon Draw::circle(int width, int height, int n)
{
    QPolygon generatedpoints;
    //creating center of circle
    QPoint center;
           center.setX(rand()%width);
           center.setY(rand()%height);
    //Generate radius of circle
    double range = rand()%100;
    double om = (2*M_PI)/n;

    //creating polygon with points
    QPoint a;

    for(int i = 0; i<n; i++)
        {
        a.setX(center.x()+ range*cos(i*om));
        a.setY(center.y()+ range*sin(i*om));
        generatedpoints.push_back(a);
        }

    return generatedpoints;
}

QPolygon Draw::grid(int width, int height, int n)
{
    QPolygon generatedpoints;

    // creating length between next point
    double lx = rand()%50;
    double ly = rand()%50;

    //size of the grid
    int grid = sqrt(n);

    //creating first point of the grid
    generatedpoints.push_back(QPoint(20,20));
    QPoint a;

    // Creating points in the row and then comming to next
    for(int i = 0; i < grid; i++)
    {
        for(int j = 0; j < grid; j++)
        {
            a.setX(20+i*lx);
            a.setY(20+j*ly);
            generatedpoints.push_back(a);
        }
    }

    return generatedpoints;
}

QPolygon Draw::ellips(int width, int height, int n)
{
    QPolygon generatedpoints;

    //Creating center
    QPoint center;
           center.setX(rand()%width);
           center.setY(rand()%height);

    //Create axis a,b and angle om
    double a = rand()%100;
    double b = rand()%100;
    double om = (2*M_PI)/n;
    QPoint c;

    for (int i = 0; i < n; i++)
        {
        c.setX(center.x() + a * cos(i*om));
        c.setY(center.y() + b * sin(i*om));
        generatedpoints.push_back(c);
        }

    return generatedpoints;
}


QPolygon Draw::square(int width, int height, int n)
{
    QPolygon generatedpoints;

    //Create top left corner of square

    QPoint  lt;
            double x(rand()%100);
            double y(rand()%100);
            lt.setX(x);
            lt.setY(y);
    generatedpoints.push_back(lt);
    //Generate how long side is
    double a=rand()%400;

    //Creating other corners
    QPoint  rt(x+a,y);
    QPoint  lb(x,y+a);
    QPoint  rb(x+a,y+a);
    generatedpoints.push_back(rt);
    generatedpoints.push_back(lb);
    generatedpoints.push_back(rb);

    //divide a in little pieces
    double dp=a/(n/4);

    for (int i = 0; i < n/4; i++)
        {
        QPoint p1(x+dp*i    , y);
        QPoint p2(x         , y+dp*i);
        QPoint p3(x+a-(dp*i), y+a);
        QPoint p4(x+a       , y+a-(dp*i));
        generatedpoints.push_back(p1);
        generatedpoints.push_back(p2);
        generatedpoints.push_back(p3);
        generatedpoints.push_back(p4);
        }

    return generatedpoints;
}

QPolygon Draw::star(int width, int height, int n)
{
    QPolygon generatedpoints;

    //Creating center with length
    QPoint center;
           center.setX(rand()%300);
           center.setY(rand()%300);

    //Create radius
    double range  = rand()%200;
    double range2 = rand()%100;
    double om = (2*M_PI)/n;
    QPoint a;


    for(int i = 0;i<n;i++)
    {
        if (i%2==0)
        {a.setX(center.x() + (range*cos(i*om)));
        a.setY(center.y() + (range*sin(i*om)));
        generatedpoints.push_back(a);}
        else {
        a.setX(center.x() + (range2*cos(i*om)));
        a.setY(center.y() + (range2*sin(i*om)));
        generatedpoints.push_back(a);}
    }

    return generatedpoints;
}
