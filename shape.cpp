#include "shape.h"

Shape::~Shape()
{
    //    layerArray = nullptr;
}

Shape::Shape():mLayerList{new std::vector<Layer*>}
{
    create_layers();
    //    create_layer_array();
    //    fill_layer_array();
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

void Shape::create_layers()
{
    int numberOfLayers = get_number_of_layers();
    adjust_layer_height();
    for (int i{0}; i<numberOfLayers; i++)
    {
        Layer* newLayer = new Layer();
        mLayerList->push_back(newLayer);
    }
}

std::vector<Layer*> Shape::get_layer_list()
{
    return *mLayerList;
}

std::vector<Point*> Shape::get_points_in_layer(int layerNumber)
{
    std::vector<Layer*> layerList = get_layer_list();
    Layer* layer = layerList[layerNumber];
    std::vector<Point*> pointsInLayer = layer->get_points();
    return pointsInLayer;
}

std::vector<Point*> Shape::get_points()
{
    std::vector<Point*> pointList;
    int numberOfLayers = get_number_of_layers();
    for (int i{0}; i<numberOfLayers; i++)
    {
        std::vector<Layer*> layerList = get_layer_list();
        Layer *layer = layerList[1];
        std::vector<Point*> pointsInLayer = layer->get_points();
        size_t numberOfPointsInLayer = pointsInLayer.size();
        for (int j{0}; j<numberOfPointsInLayer; j++)
        {
            Point *point = pointsInLayer[j];
            pointList.push_back(point);
        }
    }
    return pointList;
}
