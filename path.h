#ifndef PATH_H
#define PATH_H
#include "point.h"

class Path
{
public:
    ~Path();
    Path();
    Path(Point startPoint, Point endPoint);
    Path(Point startPoint, Point endPoint, double diameter);
    Point  get_start() const;
    Point  get_end() const;
    double get_diameter() const;
    void   set_start(Point const startPoint);
    void   set_end(Point const endPoint);
    void   set_diameter(double const diameter);


protected:

private:
    Point mStart;
    Point mEnd;
    double mDiameter;

};

#endif // PATH_H
