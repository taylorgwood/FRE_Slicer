#include "point.h"

Point::~Point()
{
}

Point::Point()
{
    // set X, Y
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

Point::Point(double x, double y, double z, double material, double diameter)
{
    set_x(x);
    set_y(y);
    set_z(z);
    set_material(material);
    set_diameter(diameter);
}


Point Point::operator+(const Point secondPoint) const
{
    double summedX = get_x() + secondPoint.get_x();
    double summedY = get_y() + secondPoint.get_y();
    double summedZ = get_z() + secondPoint.get_z();
    double summedM = (get_material() + secondPoint.get_material())/2;
    Point summedPoint{summedX, summedY, summedZ, summedM};
    return summedPoint;
}

Point Point::operator-(const Point secondPoint) const
{
    double subtractedX = get_x() - secondPoint.get_x();
    double subtractedY = get_y() - secondPoint.get_y();
    double subtractedZ = get_z() - secondPoint.get_z();
    double subtractedM = (get_material() + secondPoint.get_material())/2;
    Point subtractedPoint{subtractedX, subtractedY, subtractedZ, subtractedM};
    return subtractedPoint;
}

Point Point::operator*(const double scalar) const
{
    double scaledX = get_x()*scalar;
    double scaledY = get_y()*scalar;
    double scaledZ = get_z()*scalar;
    double scaledM = get_material();
    Point scaledPoint{scaledX, scaledY, scaledZ, scaledM};
    return scaledPoint;
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

Point Point::normalize()
{
    double x = get_x();
    double y = get_y();
    double z = get_z();
    double magnitude = get_magnitude();
    Point normalizedPoint(x/magnitude,y/magnitude,z/magnitude,1);
    return normalizedPoint;
}

double Point::get_magnitude()
{
    double x = get_x();
    double y = get_y();
    double z = get_z();
    double material = get_material();
    double magnitude = sqrt(x*x + y*y + z*z);
    return magnitude;
}

void Point::set_diameter(const double diameter)
{
    mDiameter = diameter;
}

double Point::get_diameter() const
{
    return mDiameter;
}
