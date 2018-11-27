#ifndef SHAPE_H
#define SHAPE_H
#include <cmath>
#include "layer.h"

class Shape
{
public:
    ~Shape();
    Shape();
    int get_number_of_layers();
    void adjust_layer_height();
    void set_layer_height(const double layerHeight);
    double get_layer_height() const;
    void set_height(const double height);
    double get_height() const;
    void create_layers();
    std::vector<Layer*> get_layer_list();
    std::vector<Point> get_points_in_layer(int layerNumber);
    std::vector<Point> get_points();
    std::vector<double> get_layer_locations();

protected:

private:



    double mHeight{10};
    double mLayerHeight{0.26};
    //    double mInfillAngle{90};
    bool mAutoAdjustLayers{true};
    std::vector<Layer*> *mLayerList;


};

#endif // SHAPE_H
