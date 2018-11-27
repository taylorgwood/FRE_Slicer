#include "point.h"

Point::~Point()
{
}

Point::Point()
{
    // set X, Y
    // set Z by checking which layer we are on, and calculating the layer height based on that.
    // set material
}

Point::Point(double x, double y, double z)
{
    set_x(x);
    set_y(y);
    set_z(z);
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

void Point::print()
{
    std::cout << "{" << get_x() << ", " << get_y() << ", " << get_z() << ", " << get_material() << "}" << std::endl;
}

void Point::print_list(std::vector<Point*> pointList)
{
    std::cout << "[" << std::endl;
    size_t numberOfPoints = pointList.size();
    for (int i{0}; i<numberOfPoints; i++)
    {
        Point* point = pointList[i];
        point->print();
    }
    std::cout << "]" << std::endl;
}
