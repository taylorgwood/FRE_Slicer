#include "shape.h"

Shape::~Shape()
{
}

Shape::Shape():mLayerList{new std::vector<Layer*>}
{
    create_layers();
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
    double layerLocation{0};
    for (int i{0}; i<numberOfLayers; i++)
    {
        int layerNumber{i};
        double layerHeight = get_layer_height();
        layerLocation += layerHeight;
        Layer* newLayer = new Layer(layerNumber,layerLocation);
        mLayerList->push_back(newLayer);
    }
}

std::vector<Layer*> Shape::get_layer_list()
{
    return *mLayerList;
}

std::vector<Point> Shape::get_points_in_layer(int layerNumber)
{
    std::vector<Layer*> layerList = get_layer_list();
    Layer* layer = layerList[layerNumber];
    std::vector<Point> pointsInLayer = layer->get_points();
    return pointsInLayer;
}

std::vector<Point> Shape::get_points()
{
    std::vector<Point> pointList;
    int numberOfLayers = get_number_of_layers();
    for (int i{0}; i<numberOfLayers; i++)
    {
        std::vector<Layer*> layerList = get_layer_list();
        Layer *layer = layerList[1];
        std::vector<Point> pointsInLayer = layer->get_points();
        size_t numberOfPointsInLayer = pointsInLayer.size();
        for (int j{0}; j<numberOfPointsInLayer; j++)
        {
            Point point = pointsInLayer[j];
            pointList.push_back(point);
        }
    }
    return pointList;
}

std::vector<double> Shape::get_layer_locations()
{
    std::vector<double> layerLocationVector;
    double layerLocation{0};
    std::vector<Layer*> layerList = get_layer_list();
    size_t numberOfLayers = get_number_of_layers();
    for (int i{0}; i<numberOfLayers; i++)
    {
        Layer *layer = layerList[i];
        double layerHeight = layer->get_height();
        layerLocation += layerHeight;
        layerLocationVector.push_back(layerLocation);
    }
    return layerLocationVector;
}

Layer* Shape::get_layer(int layerNumber)
{
    std::vector<Layer*> layerList = get_layer_list();
    Layer* layer = layerList[layerNumber];
    return layer;
}

void Shape::set_extrusion_width(double extrusionWidth)
{
    size_t numberOfLayers = get_number_of_layers();
    for (int i{0}; i<numberOfLayers; i++)
    {
        Layer* layer = get_layer(i);
        layer->set_extrusion_width(extrusionWidth);
    }
}

void Shape::set_infill_percentage(double infillPercentage)
{
    size_t numberOfLayers = get_number_of_layers();
    for (int i{0}; i<numberOfLayers; i++)
    {
        Layer* layer = get_layer(i);
        layer->set_infill_percentage(infillPercentage);
    }
}

void Shape::set_extrusion_multiplier(double extrusionMultiplier)
{
    size_t numberOfLayers = get_number_of_layers();
    for (int i{0}; i<numberOfLayers; i++)
    {
        Layer* layer = get_layer(i);
        layer->set_extrusion_multiplier(extrusionMultiplier);
    }
}

void Shape::set_width(double layerWidth)
{
    size_t numberOfLayers = get_number_of_layers();
    for (int i{0}; i<numberOfLayers; i++)
    {
        Layer* layer = get_layer(i);
        layer->set_width(layerWidth);
    }
}

void Shape::set_length(double layerLength)
{
    size_t numberOfLayers = get_number_of_layers();
    for (int i{0}; i<numberOfLayers; i++)
    {
        Layer* layer = get_layer(i);
        layer->set_length(layerLength);
    }
}
