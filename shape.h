#ifndef SHAPE_H
#define SHAPE_H
#include <cmath>
#include "layer.h"

class Shape
{
public:
    ~Shape();
    Shape();
    Shape(double height, double width, double length);
    int    get_number_of_layers();
    double get_adjusted_layer_height();
    void   set_layer_height(double layerHeight);
    double get_layer_height() const;
    void   reset_layer_height(double layerHeight);
    void   set_height(const double height);
    double get_height() const;
    void   create_layers();
    std::vector<Layer*> get_layer_list();
    std::vector<Point>  get_points_in_layer(int layerNumber);
    std::vector<Point>  get_points();
    std::vector<double> get_layer_locations();
    Layer  *get_layer(int layerNumber);
    double get_extrusion_width();
    void   set_extrusion_width(double extrusionWidth);
    double get_infill_percentage();
    void   set_infill_percentage(double infillPercentage);
    double get_extrusion_multiplier();
    void   set_extrusion_multiplier(double extrusionMultiplier);
    double get_width();
    void   set_width(double layerWidth);
    double get_length();
    void   set_length(double layerLength);
    void   set_auto_adjust_layer(bool adjustLayer);
    void   set_auto_adjust_path(bool adjustPath);
    double get_resolution();
    void   set_resolution(double resolution);
    bool   get_auto_adjust_path();
    std::vector<Path> *get_path_list();

protected:

private:
    double mHeight{10};
    double mWidth{10};
    double mLength{10};
    double mLayerHeight{0.26};
    bool   mAutoAdjustLayer{true};
    bool   mAutoAdjustPath{true};
    std::vector<Layer*> *mLayerList;
    double mExtrusionMultiplier{1.0};
    double mExtrusionWidth{0.26};
    double mInfillPercentage{100};
    double mResolution{1.0};

};

#endif // SHAPE_H
