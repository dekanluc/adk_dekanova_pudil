#include "draw.h"

Draw::Draw(QWidget *parent) : QWidget(parent)
{

}

void Draw::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.begin(this);

    //Draw points
    for (int i = 0; i < points.size(); i++)
    {
        painter.drawEllipse(points[i].x() - 5, points[i].y() - 5, 10, 10);
    }

    //Draw Delaunay edges
    QPen p(Qt::green, 1);
    painter.setPen(p);

    for (int i = 0; i < dt.size(); i++)
    {
        painter.drawLine(dt[i].getStart(), dt[i].getEnd());
    }

    //Draw slope

    if(slope == TRUE)
    {
    for (Triangle t : dtm)
    {
        //Get triangle vertices
        QPoint3D p1 = t.getP1();
        QPoint3D p2 = t.getP2();
        QPoint3D p3 = t.getP3();
    if (panchromatic == TRUE)
        {
        //Get slope
        double k = 255/90.0;
        int colorSlope = 255 - t.getSlope()*k;

        //Create color and set brush
        QColor c(colorSlope,colorSlope,colorSlope);
        painter.setBrush(c);
        }

    if (colorful == TRUE)
        {
        int colorSlope = t.getSlope();

        //colorful values
        if((colorSlope>=0) && (colorSlope<6)){
           painter.setBrush(QColor(245, 245, 220));
        }

        else if((colorSlope>=6) && (colorSlope<13)){
           painter.setBrush(QColor(245, 222, 179));
        }

        else if((colorSlope>=13) && (colorSlope<19)){
           painter.setBrush(QColor(210, 180, 140));
        }

        else if((colorSlope>=19) && (colorSlope<27)){
           painter.setBrush(QColor(205, 133, 63));
        }

        else if((colorSlope>=27) && (colorSlope<34)){
           painter.setBrush(QColor(210, 105, 30));
        }

        else if((colorSlope>=34) && (colorSlope<41)){
           painter.setBrush(QColor(244, 164, 96));
        }

        else if((colorSlope>=41) && (colorSlope<49)){
           painter.setBrush(QColor(160, 82, 45));
        }
        else
           painter.setBrush(QColor(139, 69, 19));

        }
        //Create triangle, add vertices
        QPolygonF triangle;
        triangle.append(QPointF(p1.x(), p1.y()));
        triangle.append(QPointF(p2.x(), p2.y()));
        triangle.append(QPointF(p3.x(), p3.y()));

        //Draw triangle
        painter.drawPolygon(triangle);
    }
}

    //Draw aspect

    if(aspect == TRUE)
    {
    for (Triangle t : dtm)
    {
        //Get triangle vertices
        QPoint3D p1 = t.getP1();
        QPoint3D p2 = t.getP2();
        QPoint3D p3 = t.getP3();

        if (panchromatic == TRUE)
        {
        double k = 255/360.0;
        //Get aspect
        int colorAspect = 255 - t.getAspect()*k;

        //Create color and set brush
        QColor c(colorAspect,colorAspect,colorAspect);
        painter.setBrush(c);
        }

        if(colorful == TRUE)
        {
            //intervals taken from arcgis aspect function
            int colorAspect = t.getAspect();
            if(((colorAspect>=0.0) && (colorAspect<22.5)) || ((colorAspect>=337.5) && (colorAspect<=360))){
                painter.setBrush(QColor(240, 255, 240));
            }

            else if((colorAspect>=22.5) && (colorAspect<67.5)){
                painter.setBrush(QColor(102, 205, 170));
            }

            else if((colorAspect>=67.5) && (colorAspect<112.5)){
                painter.setBrush(QColor(0, 191, 255));
            }

            else if((colorAspect>=112.5) && (colorAspect<157.5)){
                painter.setBrush(QColor(132, 112, 155));
            }
            else if((colorAspect>=157.5) && (colorAspect<202.5)){
                painter.setBrush(QColor(176, 48, 96));
            }

            else if((colorAspect>=202.5) && (colorAspect<247.5)){
                painter.setBrush(QColor(255, 240, 245));
            }

            else if((colorAspect>=247.5) && (colorAspect<292.5)){
                painter.setBrush(QColor(255, 222, 173));
            }

            else if((colorAspect>=292.5) && (colorAspect<337.5)){
                painter.setBrush(QColor(255, 250, 205));
            }
        }

        //Create triangle, add vertices
        QPolygonF triangle;
        triangle.append(QPointF(p1.x(), p1.y()));
        triangle.append(QPointF(p2.x(), p2.y()));
        triangle.append(QPointF(p3.x(), p3.y()));

        //Draw triangle
        painter.drawPolygon(triangle);
    }
}



    //Draw contour lines
    QPen q(Qt::gray, 1);
    painter.setPen(q);

    for (int i = 0; i < contours.size(); i++)
    {
        painter.drawLine(contours[i].getStart(), contours[i].getEnd());
    }

    //Draw main contour lines
    QPen q2(Qt::gray, 2);
    painter.setPen(q2);

    for (int i = 0; i < main_contours.size(); i++)
    {
        painter.drawLine(main_contours[i].getStart(), main_contours[i].getEnd());
    }

   //Draw contour labels
    QPen r(Qt::gray,1);
    painter.setPen(r);

    for (int i = 0; i < label_contours.size(); i++)
       { Edge one_contour = label_contours[i];
        QPoint3D label_point1 = one_contour.getStart();
        QPoint3D label_point2 = one_contour.getEnd();
        QPointF label_point;
        label_point.setX((label_point1.x() + label_point2.x()) / 2);
        label_point.setY((label_point1.y() + label_point2.y()) / 2);
        double z = label_point1.getZ();
        QString z_string = QString::number(z);
        painter.drawText(label_point, z_string);
       }

    painter.end();
}


void Draw::mousePressEvent(QMouseEvent *event)
{
    //Get coordinates of cursor
    QPoint3D p;
    p.setX(event ->x());
    p.setY(event ->y());
    double random = std::rand() * 200.0 / RAND_MAX;
    p.setZ(random);

    //Add point to the list
    points.push_back(p);

    repaint();
};


void Draw::loadFile(std::string &path,std::vector<QPoint3D> &points, int height, int width, double &min_z, double &max_z)
{
    points.clear();
    repaint();
double x;
double y;
double z;

QPoint3D point;

//Storing all polygons
double min_x = std::numeric_limits<double>::max();
double min_y = std::numeric_limits<double>::max();
double max_x = std::numeric_limits<double>::min();
double max_y = std::numeric_limits<double>::min();
min_z = std::numeric_limits<double>::max();
max_z = std::numeric_limits<double>::min();

//Load data from .txt file
std::ifstream data(path);
if(data.is_open())
    {
    //Fill the vectors
    while(data >> x >> y >> z)
        {

        point.setX(x);
        point.setY(y);
        point.setZ(z);
        points.push_back(point);
        if(x < min_x) min_x = x;
        if(x > max_x) max_x = x;
        if(y < min_y) min_y = y;
        if(y > max_y) max_y = y;
        if(z < min_z) min_z = z;
        if(z > max_z) max_z = z;
        }
    data.close();
    }
double x_coef = (width-40)/(max_x-min_x);
double y_coef = (height-40)/(max_y-min_y);

for(unsigned int i = 0; i < points.size(); i++)
{
    points[i].setX((points[i].x()-min_x)*x_coef);
    points[i].setY((points[i].y()-min_y)*y_coef);
}
    repaint();
};
