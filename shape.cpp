#include "shape.h"

Shape::~Shape()
{
}

Shape::Shape()
{
}

int Shape::get_number_of_layers()
{
    double exactNumberOfLayers = mHeight/mLayerHeight;
    int flooredNumberOfLayers = int(floor(mHeight/mLayerHeight));
    int numberOfLayers{flooredNumberOfLayers};
    if ((exactNumberOfLayers-flooredNumberOfLayers)>=0.5)
    {
        numberOfLayers = flooredNumberOfLayers+1;
    }
    return numberOfLayers;
}

void Shape::adjust_layer_height()
{
    int numberOfLayers = get_number_of_layers();
    double realLayerHeight = mHeight/numberOfLayers;
    set_layer_height(realLayerHeight);
}

void Shape::set_layer_height(const double layerHeight)
{
    mLayerHeight = layerHeight;
}

double Shape::get_layer_height() const
{
    return mLayerHeight;
}

void Shape::set_height(const double height)
{
    mHeight = height;
}

double Shape::get_height() const
{
    return mHeight;
}
