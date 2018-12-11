#include "shape.h"

Shape::~Shape()
{
}

Shape::Shape():mLayerList{new std::vector<Layer*>}
{
    create_layers();
}

Shape::Shape(double height, double width, double length):mLayerList{new std::vector<Layer*>}
{
    mHeight = height;
    mWidth = width;
    mLength = length;
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

double Shape::get_adjusted_layer_height()
{
    int numberOfLayers = get_number_of_layers();
    double adjustedLayerHeight = mHeight/numberOfLayers;
    return adjustedLayerHeight;
}

void Shape::set_layer_height(double layerHeight)
{
    mLayerHeight = layerHeight;
}

double Shape::get_layer_height() const
{
    return mLayerHeight;
}

void Shape::reset_layer_height(double layerHeight)
{
    mLayerHeight = layerHeight;
    if (mAutoAdjustLayer==true)
    {
        layerHeight = get_adjusted_layer_height();
    }
    mLayerHeight = layerHeight;
    mLayerList->clear();
    create_layers();
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
    double layerHeight = get_layer_height();
    double layerLength = get_length();
    double layerWidth  = get_width();
    double extrustionMultiplier = get_extrusion_multiplier();
    double extrusionWidth = get_extrusion_width();
    double infillPercentage = get_infill_percentage();
    double resolution = get_resolution();
    bool   adjustPath = get_auto_adjust_path();
    if (mAutoAdjustLayer == true)
    {
        layerHeight = get_adjusted_layer_height();
    }
    set_layer_height(layerHeight);
    double layerLocation{0};
    for (int i{0}; i<numberOfLayers; i++)
    {
        int layerNumber{i};
        double layerHeight = get_layer_height();
        layerLocation += layerHeight;
        Layer* newLayer = new Layer(layerNumber,layerLocation,layerLength,layerWidth, extrustionMultiplier, extrusionWidth, infillPercentage, resolution, adjustPath);
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
        Layer *layer = layerList[i];
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
    mExtrusionWidth = extrusionWidth;
    mLayerList->clear();
    create_layers();
}

double Shape::get_infill_percentage()
{
    return mInfillPercentage;
}

void Shape::set_infill_percentage(double infillPercentage)
{
    mInfillPercentage = infillPercentage;
    mLayerList->clear();
    create_layers();
}

double Shape::get_extrusion_multiplier()
{
    return mExtrusionMultiplier;
}

void Shape::set_extrusion_multiplier(double extrusionMultiplier)
{
    mExtrusionMultiplier = extrusionMultiplier;
    mLayerList->clear();
    create_layers();
}

double Shape::get_width()
{
    return mWidth;
}

void Shape::set_width(double layerWidth)
{
    mWidth = layerWidth;
    mLayerList->clear();
    create_layers();
}

double Shape::get_length()
{
    return mLength;
}

void Shape::set_length(double layerLength)
{
    mLength = layerLength;
    mLayerList->clear();
    create_layers();
}

void Shape::set_auto_adjust_layer(bool adjustLayer)
{
    mAutoAdjustLayer = adjustLayer;
    mLayerList->clear();
    create_layers();
}

void Shape::set_auto_adjust_path(bool adjustPath)
{
    mAutoAdjustPath = adjustPath;
    mLayerList->clear();
    create_layers();
}

double Shape::get_resolution()
{
    return mResolution;
}

void Shape::set_resolution(double resolution)
{
    mResolution = resolution;
    mLayerList->clear();
    create_layers();
}

double Shape::get_extrusion_width()
{
    return mExtrusionWidth;
}

bool Shape::get_auto_adjust_path()
{
    return mAutoAdjustPath;
}
