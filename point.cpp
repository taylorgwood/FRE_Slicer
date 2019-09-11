#include "point.h"

Point::~Point()
{
}

Point::Point()
{
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
    double summedM = (get_material() + secondPoint.get_material());
    Point summedPoint{summedX, summedY, summedZ, summedM};
    return summedPoint;
}

Point Point::operator-(const Point secondPoint) const
{
    double subtractedX = get_x() - secondPoint.get_x();
    double subtractedY = get_y() - secondPoint.get_y();
    double subtractedZ = get_z() - secondPoint.get_z();
    double subtractedM = (get_material() - secondPoint.get_material());
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

void Point::set_material(double material)
{
    mMaterial = material;
}

void Point::print()
{
    std::cout << "{" << get_x() << ", " << get_y() << ", " << get_z() << get_material()<< "}" << std::endl;
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
    Point normalizedPoint(x/magnitude,y/magnitude,z/magnitude);
    return normalizedPoint;
}

double Point::get_magnitude()
{
    double x = get_x();
    double y = get_y();
    double z = get_z();
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

Point  Point::cross(Point secondPoint)
{
    double X1 = this->get_x();
    double Y1 = this->get_y();
    double Z1 = this-> get_z();
    double X2 = secondPoint.get_x();
    double Y2 = secondPoint.get_y();
    double Z2 = secondPoint.get_z();
    double crossX = Y1*Z2 - Z1*Y2;
    double crossY = -X1*Z2 + Z1*X2;
    double crossZ = X1*Y2 - Y1*X2;
    Point crossProduct(crossX,crossY,crossZ);
    return crossProduct;
}

//double Point::crossXZ(Point secondPoint)
//{
//    double Ax = this->get_x();
//    double Az = this-> get_z();
//    double Bx = secondPoint.get_x();
//    double Bz = secondPoint.get_z();
//    double crossXZ = Ax*Bz-Az*Bx;
//    return crossXZ;
//}

double Point::dot(Point secondPoint)
{
    double x = this->get_x()*secondPoint.get_x();
    double y = this->get_y()*secondPoint.get_y();
    double z = this->get_z()*secondPoint.get_z();
    double dotProduct = (x+y+z);
    return dotProduct;
}

bool   Point::is_inside_XZtrapezoid(std::vector <Point> corners)
{
    Point A = corners.at(0);
    Point B = corners.at(1);
    Point C = corners.at(2);
    Point D = corners.at(3);
    Point P(get_x(),get_y(),get_z());
    A.set_y(0);
    B.set_y(0);
    C.set_y(0);
    D.set_y(0);
    P.set_y(0);
    bool isInside{false};
    //    double height = C.get_z()-B.get_z();
    //    double base = (C.get_x()-D.get_x());
    //    double top = (B.get_x()-A.get_x());
    //    double trapezoidArea = ((top+base)/2*(height));
    //    double areaABP = get_area_of_XZtriangle(A,B,P);
    //    double areaBCP = get_area_of_XZtriangle(B,C,P);
    //    double areaCDP = get_area_of_XZtriangle(C,D,P);
    //    double areaDAP = get_area_of_XZtriangle(D,A,P);
    //    double trianglesArea = areaABP+areaBCP+areaCDP+areaDAP;

    //    if ((trianglesArea-trapezoidArea)<0.0001)
    //    {
    //        isInside = true;
    //    }

    Point vectorsPAcrossBA = (P-A).cross(B-A);
    Point vectorsPDcrossCD = (P-D).cross(C-D);
    double horizontalDistanceOutside = vectorsPAcrossBA.dot(vectorsPDcrossCD);
    Point vectorsPAcrossDA = (P-A).cross(D-A);
    Point vectorsPBcrossCB = (P-B).cross(C-B);
    double verticalDistanceOutside = vectorsPAcrossDA.dot(vectorsPBcrossCB);
    if (horizontalDistanceOutside < 0)
    {
        if (verticalDistanceOutside < 0)
        {
            isInside = true;
        }
    }
    return isInside;
}

//double Point::get_area_of_XZtriangle(Point corner1, Point corner2, Point corner3)
//{
//    double x1 = corner1.get_x();
//    double x2 = corner2.get_x();
//    double x3 = corner3.get_x();
//    double z1 = corner1.get_z();
//    double z2 = corner2.get_z();
//    double z3 = corner3.get_z();
//    double area = (x1*z2 +x2*z3 +x3*z1 -z1*x2 -z2*x3 -z3*x1)/2;
//    return area;
//}

void   Point::set_travel(bool const isTravel)
{
    mTravel = isTravel;
}

bool   Point::get_travel() const
{
    return mTravel;
}

//void Point::normalize_material()
//{
//    std::vector<double> material = get_material();
//    unsigned int numberOfExtruders = static_cast<unsigned int>(material.size());
//    double magnitude{0};
//    for (unsigned int i{0}; i<numberOfExtruders; i++)
//    {
//        magnitude += material.at(i)*material.at(i);
//    }
//    for (unsigned int i{0}; i<numberOfExtruders; i++)
//    {
//        material.at(i) = material.at(i)/magnitude;
//    }
//}
