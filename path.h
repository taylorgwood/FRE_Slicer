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
    Path(Point start, Point end, double diameter, unsigned int pathNumber);
    Path(Point start, Point end, double diameter, unsigned int pathNumber, double resolution);
    Path(Point start, Point end, double diameter, unsigned int pathNumber, double resolution, double shapeWidth, double shapeLength, double shapeHeight);
    Path(Point start, Point end, double diameter, unsigned int pathNumber, double resolution, double shapeWidth, double shapeLength, double shapeHeight, unsigned int layerNumber);
    Point  get_start() const;
    void   set_start(Point const startPoint);
    Point  get_end() const;
    void   set_end(Point const endPoint);
    double get_diameter() const;
    void   set_diameter(double const diameter);
    unsigned int get_number_of_points() const;
    void   set_resolution(double const resolution);
    double get_resolution() const;
    double get_length() const;
    void   adjust_point_spacing();
    void   create_points();
    std::vector<Point*> get_point_list();
    double get_material(Point pointInfo) const;
    Point* create_new_point(unsigned int pointNumber, unsigned int numberOfPointsInPath) const;
    void   set_path_number(unsigned int const pathNumber);
    unsigned int get_path_number() const;
    void   set_shape_width(double shapeWidth);
    void   set_shape_length(double shapeLength);
    void   set_shape_height(double shapeHeight);
    void   set_layer_number(unsigned int const layerNumber);
    unsigned int get_layer_number() const;
protected:

private:
    Point  mStart{0,0,0};
    Point  mEnd{0,0,0};
    double mDiameter{0.26};
    std::vector<Point*> *mPointList{nullptr};
    double mResolution{1.0};
    unsigned int mPathNumber{0};
    double mShapeWidth{10};
    double mShapeLength{10};
    double mShapeHeight{10};
    unsigned int mLayerNumber{0};
};

#endif // PATH_H
