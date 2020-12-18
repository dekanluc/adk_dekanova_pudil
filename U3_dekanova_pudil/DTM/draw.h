#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <vector>
#include <QtGui>

#include "qpoint3d.h"
#include "edge.h"
#include "triangle.h"
#include <fstream>

class Draw : public QWidget
{
    Q_OBJECT

private:
    std::vector <QPoint3D> points;
    std::vector <Edge> dt;
    std::vector <Edge> contours;
    std::vector <Triangle> dtm;
    std::vector<Edge> label_contours,label_contours2, main_contours;
    bool slope, aspect;
    bool panchromatic, colorful;

public:
    explicit Draw(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void setPoints(std::vector<QPoint3D> &points_){points=points_;}
    std::vector<QPoint3D> & getPoints(){return points;}
    void setDT(std::vector<Edge> &dt_){dt = dt_;}
    std::vector<Edge> & getDT(){return dt;}
    void setContours(std::vector<Edge> &contours_){contours = contours_;}
    std::vector<Edge> & getContours(){return contours;}
    void setDTM(std::vector<Triangle> &dtm_){dtm = dtm_;}
    std::vector<Triangle> & getDTM(){return dtm;}
    void loadFile(std::string &path,std::vector<QPoint3D> &points, int height, int width, double &min_z, double &max_z);
    void setSlope(bool &slope_){slope = slope_;}
    void setAspect(bool &aspect_){aspect = aspect_;}
    void setPanchromatic(bool &panchromatic_){panchromatic = panchromatic_;}//
    void setColorful(bool &colorful_){colorful = colorful_;}//
    void setLabelContours(std::vector<Edge> &label_contours_){label_contours = label_contours_;}
    std::vector<Edge>& getLabelContours() {return label_contours;}
    void setMainContours(std::vector<Edge> &main_contours_){main_contours = main_contours_;}
    std::vector<Edge>& getMainContours() {return main_contours;}

signals:

public slots:
};

#endif // DRAW_H
