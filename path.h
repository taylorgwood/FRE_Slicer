#ifndef PATH_H
#define PATH_H
#include "point.h"

class Path
{
public:
    ~Path();
    Path();
    Path(Point start, Point end);
    Path(Point start, Point end, double diameter);
    Path(Point start, Point end, double diameter, double layerLocation);
    Point  get_start() const;
    void   set_start(Point const startPoint);
    Point  get_end() const;
    void   set_end(Point const endPoint);
    double get_diameter() const;
    void   set_diameter(double const diameter);
    int    get_number_of_points() const;
    void   set_resolution(double const resolution);
    double get_resolution() const;
    double get_length() const;
    void   adjust_point_spacing();
    void   create_points();
    std::vector<Point*> get_point_list();
    double get_z() const;
    void   set_z(double const layerLocation);


protected:

private:
    Point mStart;
    Point mEnd;
    double mDiameter;
    std::vector<Point*> *mPointList{nullptr};
    double mLength{10};
    double mResolution{1.0};
    double mZ{0};
};

#endif // PATH_H
