#ifndef LAYER_H
#define LAYER_H
#include "path.h"

class Layer
{
public:
    ~Layer();
    Layer();
    Layer(int number, double location);
    Layer(int number, double location, double length, double width);
    Layer(int number, double location, double length, double width, double extrusionMultiplier, double extrusionWidth, double infillPercentage, double resolution);
    Layer(int number, double location, double length, double width, double extrusionMultiplier, double extrusionWidth, double infillPercentage, double resolution, double height);
    Layer(int number, double location, double length, double width, double extrusionMultiplier, double extrusionWidth, double infillPercentage, double resolution, double height, double shapeHeight);
    double get_height() const;
    void   set_height(const double height);
    double get_infill_percentage() const;
    void   set_infill_percentage(const double infillPercentage);
    double get_extrusion_multiplier() const;
    void   set_extrusion_multiplier(const double extrusionMultiplier);
    double get_diameter_of_print();
    double get_volume() const;
    double get_area() const;
    double get_extrusion_width();
    void   set_extrusion_width(double extrusionWidth);
    double get_adjusted_extrusion_width();
    double get_perpendicular_to_path_distance();
    double get_modified_extrusion_width() const;
    void   set_area(const double area);
    int    get_number_of_infill_paths();
    std::vector<Path*> get_path_list();
    void   create_paths();
    double get_length() const;
    void   set_length(double const length);
    double get_width() const;
    void   set_width(double const width);
    double get_infill_width() const;
    double get_infill_length() const;
    std::vector<Point> get_points();
    int    get_number() const;
    void   set_number(int const number);
    double get_location() const;
    void   set_location(double const location);
    Path*  get_path(int pathNumber);
    void   set_auto_adjust_path(bool adjustPaths);
    double get_resolution() const;
    void   set_resolution(double const resolution);
    void   set_shape_height(double shapeHeight);
    double get_perimeter();
    Point get_intersection(Point lineStart, Point lineEnd, Point rayOrigin, double theta);
    std::vector<Point> create_angled_ray_origin_list();
    void set_infill_angle(double infillAngle);
    double get_infill_angle();
    std::vector <Point> get_intersection_points_list(Path path);
    std::vector <Point> get_perimeter_points();
    std::vector <Point> get_side_points(Path side);
    void set_corners();
    std::vector <Point> get_corners();
    void set_infill_size();


protected:

private:
    double const pi{3.14159265359};
    double mHeight{0.26};
    double mExtrusionWidth{0.26};
    double mInfillPercentage{100};
    double mExtrusionMultiplier{1.0};
    double mLength{10};
    double mWidth{10};
    double mInfillLength;
    double mInfillWidth;
    std::vector<Path*> *mPathList{nullptr};
    int    mNumber{0};
    double mLocation{0};
    bool   mAutoAdjustPaths{true};
    double mResolution{1};
    double mShapeHeight;
    double mInfillAngle{0};
    Point  mPointA;
    Point  mPointB;
    Point  mPointC;
    Point  mPointD;
};

#endif // LAYER_H
