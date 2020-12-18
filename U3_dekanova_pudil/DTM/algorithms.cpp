#include "algorithms.h"
#include "sortbyx.h"
#include "sortbyy.h"

int Algorithms::getPointLinePosition(QPoint3D &q,QPoint3D &p1,QPoint3D &p2)
{
    //Analyze point and line position
    //1 point in the left half plane
    //0 point in the right half plane
    //-1 point on the line
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();

    double vx = q.x() - p1.x();
    double vy = q.y() - p1.y();

    //Test criteron
    double t = ux * vy - uy * vx;

    //Point in the left half plane
    if (t>0)
        return 1;

    //Point in the right halfplane
    if (t<0)
        return 0;

    //Colinear point
    return -1;
}


void Algorithms::circleCenterAndRadius(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3, double &r, QPoint3D &s)
{
    //Center and radius of circle given by 3 points p1, p2, p3
    double k1 = p1.x() * p1.x() + p1.y() * p1.y();
    double k2 = p2.x() * p2.x() + p2.y() * p2.y();
    double k3 = p3.x() * p3.x() + p3.y() * p3.y();
    double k4 = p1.y() - p2.y();
    double k5 = p1.y() - p3.y();
    double k6 = p2.y() - p3.y();
    double k7 = p1.x() - p2.x();
    double k8 = p1.x() - p3.x();
    double k9 = p2.x() - p3.x();
    double k10 = p1.x() * p1.x();
    double k11 = p2.x() * p2.x();
    double k12 = p3.x() * p3.x();

    //Compute m: x-coordinate of circle center
    double m_numerator = -k12 * k4 + k11 * k5 - (k10 + k4 * k5) * k6;
    double m_denominator = -p3.x() * k4 + p2.x() * k5 - p1.x() * k6;
    double m = 0.5 * m_numerator / m_denominator;

    //Compute n: y-coordinate of circle center
    double n_numerator = -k1 * k9  + k2 * k8 - k3 * k7;
    double n_denominator = -p1.y() * k9  + p2.y()  * k8 - p3.y()  * k7;
    double n = 0.5 * n_numerator / n_denominator;

    //Compute circle radius
    r = sqrt((p1.x() - m) * (p1.x() - m) + (p1.y() - n) * (p1.y() - n));

    //Set m,n to s
    s.setX(m);
    s.setY(n);
}


int Algorithms::findDelaunayPoint(QPoint3D &pi, QPoint3D &pj, std::vector<QPoint3D> &points)
{
    //Find optimal Delaunay point (third vertex of of triangle)
    int i_min = -1;
    double r_min = INFINITY;

    //Browse all input points
    for (int i = 0; i < points.size(); i++)
    {
        //Check, whether points[i] different from pi, pj
        if((points[i]!=pi) && (points[i]!=pj))
        {
            //Point in the left halfplane
            if(getPointLinePosition(points[i], pi, pj)==1)
            {
                //Compute circle center and radius
                QPoint3D s;
                double r;
                circleCenterAndRadius(pi, pj, points[i], r, s);

                //Circle center in the right halplane
                if(getPointLinePosition(s, pi, pj)==0)
                    r = -r;

                //Actualize minimum
                if(r<r_min)
                {
                    r_min = r;
                    i_min = i;
                }

            }
        }
    }

    return i_min;
}


double Algorithms::dist(QPoint3D &p1, QPoint3D &p2)
{
     //Get Euclidean distance between two points
     double dx = (p1.x() - p2.x());
     double dy = (p1.y() - p2.y());

     return sqrt(dx*dx + dy*dy);
}


int Algorithms::getNearestpoint(QPoint3D &p, std::vector<QPoint3D> &points)
{
    //Find nearest point to p
    int i_min = 1;
    double d_min = dist(p, points[1]);

    //Browses all points
    for (unsigned int i = 2; i < points.size(); i++)
    {
        //Compute distance
        double d = dist(p, points[i]);

        //Actualize minimum i and distance
        if (d < d_min)
        {
            d_min=d;
            i_min=i;
        }
    }

    return i_min;
}


std::vector<Edge> Algorithms:: DT(std::vector<QPoint3D> &points)
{
    //Delaunay triangulation
    std::vector<Edge> dt;
    std::list<Edge> ael;

    //Sort by X
    sort(points.begin(), points.end(), sortByX());

    //Pivot
    QPoint3D q = points[0];

    //Nearest point
    int index = getNearestpoint(q,points);
    QPoint3D p_nearest = points[index];

    //Find optimal Delaunay point
    int i_o = findDelaunayPoint(q, p_nearest,points);

    //Create new edge
    Edge e(q, p_nearest);

    //No suitable point found in the left halfplane
    if (i_o == -1)
    {
        //Change orientation of the edge
        e.changeOrientation();

        //Find optimal Delauanay points once more
        i_o = findDelaunayPoint(e.getStart(), e.getEnd(), points);
    }

    //3rd vertex of triangle
    QPoint3D p_3 = points[i_o];

    //Create inicial triangle
    Edge e2(e.getEnd(), p_3);
    Edge e3(p_3, e.getStart());

    //Add triangle to DT
    dt.push_back(e);
    dt.push_back(e2);
    dt.push_back(e3);

    //Add edges to AEL
    ael.push_back(e);
    ael.push_back(e2);
    ael.push_back(e3);

    //Until ael is empty process candidate edges
    while (!ael.empty())
    {
        //Get last edge
        Edge edge1 = ael.back();
        ael.pop_back();

        //Change orientation
        edge1.changeOrientation();

        //Find optimal Delauanay point
        i_o = findDelaunayPoint(edge1.getStart(), edge1.getEnd(), points);

        //Optimal point found
        if (i_o != -1)
        {
            //3rd vertex of triangle
            QPoint3D p3 = points[i_o];

            //Create inicial triangle
            Edge edge2(edge1.getEnd(), p3);
            Edge edge3(p3, edge1.getStart());

            //Add triangle to DT
            dt.push_back(edge1);
            dt.push_back(edge2);
            dt.push_back(edge3);

            //Change orientation of edges
            edge2.changeOrientation();
            edge3.changeOrientation();

            //Update AEL
            updateAEL(edge2,ael);
            updateAEL(edge3,ael);
        }
    }

    return dt;
}


void Algorithms::updateAEL(Edge &e, std::list<Edge> &ael)
{
    //Update AEL
    std::list<Edge>::iterator ie = find(ael.begin(),ael.end(), e);

    //Edge is not in AEL
    if(ie == ael.end())
    {
        //Change orientation
        e.changeOrientation();

        //Add edge to AEL
        ael.push_back(e);
    }

    //Edge is already in list, erase
    else ael.erase(ie);
}


QPoint3D Algorithms::getContourPoint(QPoint3D &p1, QPoint3D &p2, double z)
{
    //Create point on contour
    double xa = (p2.x()-p1.x())/(p2.getZ()-p1.getZ())*(z-p1.getZ())+p1.x();
    double ya = (p2.y()-p1.y())/(p2.getZ()-p1.getZ())*(z-p1.getZ())+p1.y();

    QPoint3D A(xa, ya, z);
    return A;
}


std::vector<Edge> Algorithms::contourLines(std::vector<Edge> &dt, double z_min, double z_max, double dz)
{
    //Create contour lines using linear interpolation
    std::vector<Edge> contours;

    //Browse all triangles one by one
    for(int i=0; i<dt.size(); i+=3)
    {
        //Get triangle vertices
        QPoint3D p1 = dt[i].getStart();
        QPoint3D p2 = dt[i].getEnd();
        QPoint3D p3 = dt[i+1].getEnd();

        //Compute all contour lines for a triangle
        for (double z = z_min; z <= z_max; z += dz)
        {
            //Get height differences
            double dz1 = z - p1.getZ();
            double dz2 = z - p2.getZ();
            double dz3 = z - p3.getZ();

            //Compute test criteria
            double t12 = dz1*dz2;
            double t23 = dz2*dz3;
            double t31 = dz3*dz1;

            //Triangle in the plane
            if ((dz1 == 0) && (dz2 == 0) && (dz3 == 0))
                continue;

            //Edge 1,2 of triangle in plane
            else if ((dz1 == 0) && (dz2 == 0))
                contours.push_back(dt[i]);

            //Edge 2,3 of triangle in plane
            else if ((dz2 == 0) && (dz3 == 0))
                contours.push_back(dt[i+1]);

            //Edge 3,1 of triangle in plane
            else if ((dz3 == 0) && (dz1 == 0))
                contours.push_back(dt[i+2]);

            //Edges 1,2 and 2,3 are intersected by plane
            else if ((t12 < 0) && (t23 <= 0) || (t12 <= 0) && (t23 < 0))
            {
                //Compute points of intersection
                QPoint3D A = getContourPoint(p1, p2, z);
                QPoint3D B = getContourPoint(p2, p3, z);

                //Create contour line and add to the list
                Edge e(A, B);
                contours.push_back(e);
            }

            //Edges 2,3 and 3,1 are intersected by plane
            else if ((t23 < 0) && (t31 <= 0) || (t23 <= 0) && (t31 < 0))
            {
                //Compute points of intersection
                QPoint3D A = getContourPoint(p2, p3, z);
                QPoint3D B = getContourPoint(p3, p1, z);

                //Create contour line and add to the list
                Edge e(A, B);
                contours.push_back(e);
            }

            //Edges 1,2 and 3,1 are intersected by plane
            else if ((t12 < 0) && (t31 <= 0) || (t12 <= 0) && (t31 < 0))
            {
                //Compute points of intersection
                QPoint3D A = getContourPoint(p1, p2, z);
                QPoint3D B = getContourPoint(p3, p1, z);

                //Create contour line and add to the list
                Edge e(A, B);
                contours.push_back(e);
            }
        }
    }

    return contours;
}


double Algorithms::getSlope(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3)
{
    //Compute slope of triangle in DT
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();
    double uz = p2.getZ() - p1.getZ();

    double vx = p3.x() - p1.x();
    double vy = p3.y() - p1.y();
    double vz = p3.getZ() - p1.getZ();

    //Normal vector
    double nx = uy*vz - uz*vy;
    double ny = -(ux*vz - uz*vx);
    double nz = ux*vy - uy*vx;

    //Norm
    double nt = sqrt(nx*nx + ny*ny + nz*nz);

    return acos(nz/nt)*180/M_PI;
}


double Algorithms::getAspect(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3)
{
    //Compute aspect of triangle in DT
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();
    double uz = p2.getZ() - p1.getZ();

    double vx = p3.x() - p1.x();
    double vy = p3.y() - p1.y();
    double vz = p3.getZ() - p1.getZ();

    //Calculate normal vector and its norm
    double nx = uy*vz - uz*vy;
    double ny = -(ux*vz - uz*vx);

    return atan2(nx, ny)/ M_PI * 180;
}


std::vector<Triangle> Algorithms::analyzeDTM(std::vector<Edge> &dt)
{
    //Analyze slope and aspect of DTM
    std::vector<Triangle> triangles;

    //Process each triangle
    for (int i = 0; i<dt.size() ;i += 3)
    {
        //Get vertices
        QPoint3D p1 = dt[i].getStart();
        QPoint3D p2 = dt[i].getEnd();
        QPoint3D p3 = dt[i+1].getEnd();

        //Compute slope and aspect
        double slope = getSlope(p1, p2, p3);

        //Compute aspect
        double aspect = getAspect(p1, p2, p3);

        //Create triangle
        Triangle triangle (p1, p2, p3, slope, aspect);

        //Add triangle to the list
        triangles.push_back(triangle);
    }

    return triangles;
}

double Algorithms::getPointLineDistance(QPoint3D &q, QPoint3D &p1, QPoint3D &p2)
{
    //Calculate point and line distance
    double numerator = q.x() * (p1.y() - p2.y()) + p1.x() * (p2.y() - q.y()) + p2.x() * (q.y() - p1.y());
    double dx = p2.x() - p1.x();
    double dy = p2.y() - p1.y();
    double denumenator = sqrt(dx * dx + dy * dy);

    double dist = fabs(numerator)/denumenator;
    return dist;
}

std::vector<QPoint3D> Algorithms::generateRandomPoints(int n, int height, int width)
{
    std::vector<QPoint3D> points;

     for (int i =0; i < n; i++)
     {
         QPoint3D point;
         double x = QRandomGenerator::global()->bounded(10, width - 10);
         double y = QRandomGenerator::global()->bounded(10, height - 10);
         point.setX(x);
         point.setY(y);
         double random = std::rand() * 200.0 / RAND_MAX;
         point.setZ(random);
         points.push_back(point);
     }

     return points;
}

std::vector<QPoint3D> Algorithms::generateStack(int n, int height, int width)
{
    //Generate random points
    std::vector<QPoint3D> points = generateRandomPoints(n, height, width);

    //Initialize center
    double sum_x = 0;
    double sum_y = 0;

    //Count center of points
    for (int i = 0; i < points.size(); i++)
    {
        sum_x = sum_x + points[i].x();
        sum_y = sum_y + points[i].y();
    }

    QPoint3D point;
    point.setX(sum_x/points.size());
    point.setY(sum_y/points.size());
    point.setZ(1000);
    points.push_back(point);

    //Giving z to points by distance
    for (int i = 0; i < (points.size()-1); i++)
    {
        double d = dist(points[n], points[i]);
        points[i].setZ(1000 - d + (std::rand() * 10 / RAND_MAX));
    }

    return points;
}


std::vector<QPoint3D> Algorithms::generateRidge(int n, int height, int width)
{
    std::vector<QPoint3D> points = generateRandomPoints(n, height, width);
    //Make vertical or horizontal ridge
    int w = QRandomGenerator::global()->bounded(2);

    qDebug() << w;

    if (w == 0)
        std::sort(points.begin(), points.end(), sortByX());
    else
        std::sort(points.begin(), points.end(), sortByY());

    //Set z of ridge
    points[0].setZ(1000);
    points.back().setZ(950);

    //Initialize center
    double sum_x = 0;
    double sum_y = 0;

    //Count center of points
    for (int i = 0; i < points.size(); i++)
    {
        sum_x = sum_x + points[i].x();
        sum_y = sum_y + points[i].y();
    }

    QPoint3D point;
    point.setX(sum_x/points.size());
    point.setY(sum_y/points.size());
    point.setZ(1000);
    points.push_back(point);

    for (int i = 1; i < points.size() - 1; i++)
    {
        double d1 = dist(points[i], points[0]);
        double d2 = dist(points[i], points[points.size()-2]);

        if (d1 < d2)
            points[i].setZ(1000 - getPointLineDistance(points[i], points[0], points.back()) + (std::rand() * 10 / RAND_MAX));
        else
            points[i].setZ(1000 - getPointLineDistance(points[i], points[points.size()-2], points.back()) + (std::rand() * 10 / RAND_MAX));

    }
    return points;
}

std::vector<QPoint3D> Algorithms::generateValley(int n, int height, int width)
{
    std::vector<QPoint3D> points = generateRandomPoints(n, height, width);
    //Make vertical or horizontal ridge
    int w = QRandomGenerator::global()->bounded(2);

    qDebug() << w;

    if (w == 0)
        std::sort(points.begin(), points.end(), sortByX());
    else
        std::sort(points.begin(), points.end(), sortByY());

    //Set z of ridge
    points[0].setZ(100);
    points.back().setZ(150);

    //Initialize center
    double sum_x = 0;
    double sum_y = 0;

    //Count center of points
    for (int i = 0; i < points.size(); i++)
    {
        sum_x = sum_x + points[i].x();
        sum_y = sum_y + points[i].y();
    }

    QPoint3D point;
    point.setX(sum_x/points.size());
    point.setY(sum_y/points.size());
    point.setZ(120);
    points.push_back(point);

    for (int i = 1; i < points.size() - 1; i++)
    {
        double d1 = dist(points[i], points[0]);
        double d2 = dist(points[i], points[points.size()-2]);

        if (d1 < d2)
            points[i].setZ(150 + getPointLineDistance(points[i], points[0], points.back()) + (std::rand() * 10 / RAND_MAX));
        else
            points[i].setZ(150 + getPointLineDistance(points[i], points[points.size()-2], points.back()) + (std::rand() * 10 / RAND_MAX));

    }
    return points;

}

std::vector<QPoint3D> Algorithms::generateSettling(int n, int height, int width)
{
    std::vector<QPoint3D> points = generateRandomPoints(n, height, width);
    //Make vertical or horizontal ridge
    int w = QRandomGenerator::global()->bounded(2);

    if (w == 0)
        std::sort(points.begin(), points.end(), sortByX());
    else
        std::sort(points.begin(), points.end(), sortByY());

    //Set z of ridge
    points[0].setZ(1000);
    points[1].setZ(1000);

    for (int i = 2; i < points.size(); i++)
    {
        points[i].setZ(1000 - getPointLineDistance(points[i], points[0], points[1]) + (std::rand() * 10 / RAND_MAX));
    }

    double z1 = points[points.size()/2].getZ();
    double z2 = points[points.size()/2 + n/4].getZ();
    double dz = z1 - z2;

    for (int i = points.size()/2; i < (points.size()/2 + n/4); i++)
    {
        points[i].setZ(z1);
    }

    for (int i = (points.size()/2 + n/4); i < points.size(); i++)
    {
        double z3 = points[i].getZ();
        points[i].setZ(z3 + dz);
    }
    return points;
}
