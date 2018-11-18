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
    mX = x;
    mY = y;
    mZ = z;
    mMaterial = material;
}

double Point::get_x()
{
    return mX;
}

double Point::get_y()
{
    return mY;
}

double Point::get_z()
{
    return mZ;
}

double Point::get_material()
{
    return mMaterial;
}
