#ifndef SHAPE_H
#define SHAPE_H
#include <cmath>
#include "layer.h"

class Shape
{
public:
    ~Shape();
    Shape();
    int    get_number_of_layers();
    double get_adjusted_layer_height();
    void   set_layer_height(const double layerHeight);
    double get_layer_height() const;
    void   set_height(const double height);
    double get_height() const;
    void   create_layers();
    std::vector<Layer*> get_layer_list();
    std::vector<Point>  get_points_in_layer(int layerNumber);
    std::vector<Point>  get_points();
    std::vector<double> get_layer_locations();
    Layer  *get_layer(int layerNumber);
    void   set_extrusion_width(double extrusionWidth);
    void   set_infill_percentage(double infillPercentage);
    void   set_extrusion_multiplier(double extrusionMultiplier);
    void   set_width(double layerWidth);
    void   set_length(double layerLength);
    void   set_auto_adjust_layer(bool adjustLayer);
    void   set_auto_adjust_path(bool adjustPath);
    void   set_resolution(double resolution);
    double get_extrusion_width();

protected:

private:
    double mHeight{10};
    double mLayerHeight{0.26};
    bool   mAutoAdjustLayer{true};
    std::vector<Layer*> *mLayerList;


};

#endif // SHAPE_H
