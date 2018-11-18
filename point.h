#ifndef POINT_H
#define POINT_H


class Point
{
public:
    ~Point();
    Point();
    Point(double x, double y, double z);
    Point(double x, double y, double z, double material);
    double get_x();
    double get_y();
    double get_z();
    double get_material();

protected:

private:
    double mX{0};
    double mY{0};
    double mZ{0};
    double mMaterial{0};

};

#endif // POINT_H
