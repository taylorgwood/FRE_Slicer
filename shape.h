#ifndef SHAPE_H
#define SHAPE_H
#include <cmath>

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

protected:

private:
    double mHeight{10};
    double mLayerHeight{0.26};
//    double mInfillAngle{90};


};

#endif // SHAPE_H
