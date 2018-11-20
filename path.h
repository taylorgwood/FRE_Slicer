#ifndef PATH_H
#define PATH_H
#include "point.h"
#include <vector>

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
    int    get_number_of_points() const;
    void   set_resolution(double const resolution);
    double get_resolution() const;


protected:

private:
    Point mStart;
    Point mEnd;
    double mDiameter;
    std::vector<Point*> *mPointList{nullptr};
    double mLength{10};
    double mResolution{1.0};
};

#endif // PATH_H
