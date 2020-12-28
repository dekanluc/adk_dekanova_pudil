#ifndef EDGE_H
#define EDGE_H

#include "qpoint3d.h"

class Edge
{
    private:
        QPoint3D s, e;

    public:
        Edge(): s(0, 0, 0), e(0, 0, 0){}
        Edge(QPoint3D &s_, QPoint3D &e_):s(s_), e(e_){}

        void setStart(QPoint3D &s_){s = s_;}
        void setEnd(QPoint3D &e_){e = e_;}
        QPoint3D & getStart(){return s;}
        QPoint3D & getEnd(){return e;}
        void changeOrientation()
        {
            QPoint3D a = s;
            s = e;
            e = a;
        }

        bool operator == (const Edge &h) const
        {
            return (h.s == s) && (h.e == e);
        }

};

#endif // EDGE_H
