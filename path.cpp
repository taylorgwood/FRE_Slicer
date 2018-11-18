#include "path.h"

Path::~Path()
{

}

Path::Path()
{

}

Path::Path(Point startPoint, Point endPoint)
{
    set_start(startPoint);
    set_end(endPoint);
}

Path::Path(Point startPoint, Point endPoint, double diameter)
{
    set_start(startPoint);
    set_end(endPoint);
    set_diameter(diameter);
}

Point Path::get_start() const
{
    return mStart;
}

Point Path::get_end() const
{
    return mEnd;
}

double Path::get_diameter() const
{
    return mDiameter;
}

void Path::set_start(Point const startPoint)
{
    mStart = startPoint;
}

void Path::set_end(Point const endPoint)
{
    mEnd = endPoint;
}

void Path::set_diameter(double const diameter)
{
    mDiameter = diameter;
}
