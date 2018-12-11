#ifndef LAYER_H
#define LAYER_H
#include "path.h"

class Layer
{
public:
    ~Layer();
    Layer();
    Layer(int layerNumber, double layerLocation);
    Layer(int layerNumber, double layerLocation, double layerLength, double layerWidth);
    Layer(int layerNumber, double layerLocation, double layerLength, double layerWidth, double extrusionMultiplier, double extrusionWidth, double infillPercentage, double resolution, bool adjustPath)    ;
    double get_height() const;
    void   set_height(const double height);
    double get_extrusion_width() const;
    void   set_extrusion_width(double extrusionWidth);
    double get_infill_percentage() const;
    void   set_infill_percentage(const double infillPercentage);
    double get_extrusion_multiplier() const;
    void   set_extrusion_multiplier(const double extrusionMultiplier);
    double get_modified_extrusion_width() const;
    double get_diameter_of_print() const;
    double get_volume() const;
    double get_area() const;
    void   set_area(const double area);
    int    get_number_of_paths();
    double get_adjusted_extrusion_width();
    std::vector<Path*> get_path_list();
    void   create_paths();
    double get_length() const;
    void   set_length(double const length);
    double get_width() const;
    void   set_width(double const width);
    std::vector<Point> get_points();
    int    get_number() const;
    void   set_number(int const number);
    double get_location() const;
    void   set_location(double const location);
    std::vector<Point>  get_turn_points(int pathNumber);
    Path*  get_path(int pathNumber);
    void   set_auto_adjust_path(bool adjustPaths);
    double get_resolution() const;
    void   set_resolution(double const resolution);

protected:

private:
    double const pi{3.14159265359};
    double mHeight{0.26};
    double mExtrusionWidth{0.26};
    double mInfillPercentage{100};
    double mExtrusionMultiplier{1.0};
    double mLength{10};
    double mWidth{10};
    std::vector<Path*> *mPathList{nullptr};
    int    mNumber{0};
    double mLocation{0};
    bool   mAutoAdjustPaths{true};
    double mResolution{1};
};

#endif // LAYER_H
