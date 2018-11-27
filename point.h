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
    void   set_material(double const material);
    void   print();
    void   print_list(std::vector<Point*> pointList);
    Point  normalize();
    double get_magnitude();

protected:

private:
    double mX{0};
    double mY{0};
    double mZ{0};
    double mMaterial{0};

};

#endif // POINT_H
