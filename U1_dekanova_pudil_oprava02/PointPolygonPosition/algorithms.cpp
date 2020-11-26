#include "algorithms.h"
#include <cmath>
Algorithms::Algorithms()
{

}

int Algorithms::getPointLinePosition(QPointF &q, QPointF &p1, QPointF &p2)
{
    //Get point and line position
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();
    double vx = q.x() - p1.x();
    double vy = q.y() - p1.y();

    //Determinant
    double t = ux * vy - uy * vx;

    //Checking the singularity
    double eps1 = 1.0e-1;//.0e-6;

    //Vectors
    double wx = q.x() - p2.x();
    double wy = q.y() - p2.y();

    //Distances
    double p2q = sqrt(wx * wx + wy * wy);
    double p1q = sqrt(vx * vx + vy * vy);

    double p1p2 = sqrt(ux * ux + uy * uy);
    double dist = p1q + p2q;

    //Checking if sum of dist p1q and dist p2q = dist p1p2
    if((dist - p1p2) <= eps1 && (dist - p1p2) >= -eps1)
        return -2;

    //Point in the left halfplane
    if ( t > eps1)
         return 1;

    //Point in the right halfplane
    if (t < eps1)
        return 0;

    //Point on the line
    else return -1;
}

double Algorithms::getAngle(QPointF &q1, QPointF &q2, QPointF &q3, QPointF &q4)
{
    // Calculate Vector betwen 2 vectors
    double ux = q2.x() - q1.x();
    double uy = q2.y() - q1.y();

    double vx = q4.x() - q3.x();
    double vy = q4.y() - q3.y();

    //Norms
    double nu = sqrt(ux * ux + uy * uy);
    double nv = sqrt(vx * vx + vy * vy);

    //Dot product
    double scalar = ux * vx + uy * vy;

    double angle = fabs(acos(scalar/(nu*nv)));
    return angle;
}

int Algorithms::getPositionWinding(QPointF &q, std::vector<QPointF> &pol)
{

    double Omega = 0.0;

    //Set tolerance
    const double eps = 1.0e-4;

    //The amount of polygon vertices
    const int n = pol.size();

    //processing of all vertices of the polygon
    for (int i = 0; i < n;i++)
        {

            //Get angle omega
            double om = getAngle(q,pol[i], q, pol[(i+1)%n]);

            //Get orientation of q and pol i , pol i + 1
            int orientation = getPointLinePosition(q, pol[i], pol[(i+1)%n]);

            //Point in the half plane
            if (orientation == 1)
                Omega +=om;

            //Point in the half plane
            else if (orientation == 0)
                Omega -= om;

            //Point on the boundary
            else
                return -1;
       }

       //Position q inside polygon
       if (fabs(fabs(Omega)-2* M_PI)< eps)
            return 1;

       //Q outside of polygon
       else
            return 0;
}


int Algorithms::getPositionRay(QPointF &q, std::vector<QPointF> &pol)
{
    // Amount of intersections
    int k = 0;

    // Amount of intersection on the left half plane
    int l = 0;

    //Amount of polygon vertices
    int n = pol.size();

    //Tolerance
    double tol=1.5;

    // Processing of all vetices of the polygon
    for (int i = 1; i < n+1 ; i++)


 {          //Compute xi, yi, xii, yii
            double xir = pol[i%n].x() - q.x();
            double yir = pol[i%n].y() - q.y();

            double xiir = pol[i-1].x() - q.x();
            double yiir = pol[i-1].y() - q.y();

            //Does segment pi-1, pi intersect ray
            if (((yir > 0) && (yiir <= 0)) || ((yiir > 0) && (yir <= 0)) )
            {
                //Computing of intersection
                double xm = (xir * yiir - xiir * yir)/(yir - yiir);

                // Point in the right half plane
                if (xm > -tol)

                    k += 1;              
                // Point in the left half plane
                 if (xm < tol)
                  l += 1;
            }

        yir = yiir;
        xir = xiir;


        }
        // Point on the edge
        if (k%2 > l%2 or l%2 > k%2)
                return -1;

        return k%2;

}
