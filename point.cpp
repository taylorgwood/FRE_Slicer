#include "point.h"

Point::~Point()
{
}

Point::Point()
{
}

Point::Point(double x, double y, double z)
{
    mX = x;
    mY = y;
    mZ = z;
}

Point::Point(double x, double y, double z, double material)
{
    set_x(x);
    set_y(y);
    set_z(z);
    set_material(material);
}

Point Point::operator+(const Point secondPoint)
{
    double summedX = get_x() + secondPoint.get_x();
    double summedY = get_y() + secondPoint.get_y();
    double summedZ = get_z() + secondPoint.get_z();
    double summedM = (get_material() + secondPoint.get_material())/2;
    Point summedPoint{summedX, summedY, summedZ, summedM};
    return summedPoint;
}

double Point::get_x() const
{
    return mX;
}

double Point::get_y() const
{
    return mY;
}

double Point::get_z() const
{
    return mZ;
}

double Point::get_material() const
{
    return mMaterial;
}

void Point::set_x(double const x)
{
    mX = x;
}

void Point::set_y(double const y)
{
    mY = y;
}

void Point::set_z(double const z)
{
    mZ = z;
}

void Point::set_material(double const material)
{
    mMaterial = material;
}
