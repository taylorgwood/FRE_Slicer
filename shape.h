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
    Shape(double height, double width, double topWidth, double length);
    unsigned int get_number_of_layers();
    double get_adjusted_layer_height();
    void   set_layer_height(double layerHeight);
    double get_layer_height() const;
    void   reset_layer_height(double layerHeight);
    void   set_height(const double height);
    void   reset_height(double height);
    double get_height() const;
    void   create_layers();
    std::vector<Layer*> get_layer_list();
    std::vector<Point>  get_points_in_layer(unsigned int layerNumber);
    std::vector<Point>  get_point_list();
    std::vector<double> get_layer_locations();
    Layer  *get_layer(unsigned int layerNumber);
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
    double get_resolution_length();
    void   set_resolution_length(double resolutionLength);
    std::vector<Path> *get_path_list();
    void   set_top_width(double topWidth);
    double get_top_width() const;
    double get_layer_width(unsigned int layerNumber);
    void   set_infill_angle(double thetaInDegrees);
    double get_infill_angle() const;
    void   refresh();
    std::vector<Point> get_simplified_point_list();

protected:

private:
    double mHeight{10};
    double mWidth{10};
    double mTopWidth{10};
    double mLength{10};
    double mLayerHeight{0.26};
    bool   mAutoAdjustLayer{true};
    std::vector<Layer*> *mLayerList;
    double mExtrusionMultiplier{1.0};
    double mExtrusionWidth{0.26};
    double mInfillPercentage{100};
    double mInfillAngle{0};
    double mResolutionLength{1.0};
};

#endif // SHAPE_H
