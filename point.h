#ifndef POINT_H
#define POINT_H
#include <iostream>
#include <vector>

class Point
{
public:
    ~Point();
    Point();
    Point(double x, double y, double z);
    Point(double x, double y, double z, double material);
    Point(double x, double y, double z, double material, double diameter);
    Point  operator+(const Point secondPoint) const;
    Point  operator-(const Point secondPoint) const;
    Point  operator*(const double scalar) const;
    double get_x() const;
    double get_y() const;
    double get_z() const;
    double get_material() const;
    void   set_x(double const x);
    void   set_y(double const y);
    void   set_z(double const z);
    void   set_material(double material);
    void   print();
    void   print_list(std::vector<Point*> pointList);
    Point  normalize();
    double get_magnitude();
    void   set_diameter(const double diameter);
    double get_diameter() const;
    Point  cross(Point secondPoint);
//    double crossXZ(Point secondPoint);
    double dot(Point secondPoint);
    bool   is_inside_XZtrapezoid(std::vector <Point> corners);
//    double get_area_of_XZtriangle(Point corner1, Point corner2, Point corner3);
    void   set_travel(bool const isTravel);
    bool   get_travel() const;


protected:

private:
    double mX{0};
    double mY{0};
    double mZ{0};
    double mMaterial{1};
    double mDiameter{0.26};
    bool   mTravel{false};
};

#endif // POINT_H
