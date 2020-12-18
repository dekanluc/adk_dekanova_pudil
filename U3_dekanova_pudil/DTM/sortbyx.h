#ifndef SORTBYX_H
#define SORTBYX_H

#include "qpoint3d.h"

class sortByX
{
    public:
        bool operator()(QPoint3D &p1, QPoint3D &p2)
        {
            return p1.x() < p2.x();
        }
};

#endif // SORTBYX_H
