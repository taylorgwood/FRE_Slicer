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
    set_height(height);
    set_width(width);
    set_length(length);
    create_layers();
}

Shape::Shape(double height, double width, double topWidth, double length):mLayerList{new std::vector<Layer*>}
{
    set_height(height);
    set_width(width);
    set_top_width(topWidth);
    set_length(length);
    create_layers();
}

unsigned int Shape::get_number_of_layers()
{
    double exactNumberOfLayers = mHeight/mLayerHeight;
    unsigned int flooredNumberOfLayers = static_cast<unsigned int>((floor(mHeight/mLayerHeight)));
    unsigned int numberOfLayers{flooredNumberOfLayers};
    if ((exactNumberOfLayers-flooredNumberOfLayers)>=0.5)
    {
        numberOfLayers = flooredNumberOfLayers+1;
    }
    return numberOfLayers;
}

double Shape::get_adjusted_layer_height()
{
    unsigned int numberOfLayers = get_number_of_layers();
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
    create_layers();
}

void Shape::set_height(const double height)
{
    mHeight = height;
}

void Shape::reset_height(double height)
{
    mHeight = height;
}

double Shape::get_height() const
{
    return mHeight;
}

void Shape::create_layers()
{
    mLayerList->clear();
    unsigned int numberOfLayers = get_number_of_layers();
    double layerHeight = get_layer_height();
    double layerLength = get_length();
    double extrustionMultiplier = get_extrusion_multiplier();
    double extrusionWidth = get_extrusion_width();
    double infillPercentage = get_infill_percentage();
    double resolution = get_resolution();
    double shapeHeight = get_height();
    double infillAngle = get_infill_angle();
    if (mAutoAdjustLayer == true)
    {
        layerHeight = get_adjusted_layer_height();
    }
    set_layer_height(layerHeight);
    double layerLocation = -layerHeight/2;
    for (unsigned int i{0}; i<numberOfLayers; i++)
    {
        unsigned int layerNumber{i};
        double layerWidth = get_layer_width(layerNumber);
        layerLocation += layerHeight;
        Layer* newLayer = new Layer(layerNumber,layerLocation,layerLength,layerWidth, extrustionMultiplier, extrusionWidth, infillPercentage, resolution, layerHeight, shapeHeight, infillAngle);
        mLayerList->push_back(newLayer);
    }
}

std::vector<Layer*> Shape::get_layer_list()
{
    return *mLayerList;
}

std::vector<Point> Shape::get_points_in_layer(unsigned int layerNumber)
{
    std::vector<Layer*> layerList = get_layer_list();
    Layer* layer = layerList[layerNumber];
    std::vector<Point> pointsInLayer = layer->get_point_list();
    return pointsInLayer;
}

std::vector<Point> Shape::get_point_list()
{
    std::vector<Point> pointList;
    std::vector<Layer*> layerList = get_layer_list();
    unsigned int numberOfLayers = static_cast<unsigned int>(layerList.size());
    for (unsigned int i{0}; i<numberOfLayers; i++)
    {
        Layer *layer = layerList[i];
        std::vector<Point> pointsInLayer = layer->get_point_list();
        unsigned int numberOfPointsInLayer = static_cast<unsigned int>(pointsInLayer.size());
        for (unsigned int j{0}; j<numberOfPointsInLayer; j++)
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
    unsigned int numberOfLayers = get_number_of_layers();
    for (unsigned int i{0}; i<numberOfLayers; i++)
    {
        Layer *layer = layerList[i];
        double layerHeight = layer->get_height();
        layerLocation += layerHeight;
        layerLocationVector.push_back(layerLocation);
    }
    return layerLocationVector;
}

Layer* Shape::get_layer(unsigned int layerNumber)
{
    std::vector<Layer*> layerList = get_layer_list();
    Layer* layer = layerList[layerNumber];
    return layer;
}

void Shape::set_extrusion_width(double extrusionWidth)
{
    mExtrusionWidth = extrusionWidth;
}

double Shape::get_infill_percentage()
{
    return mInfillPercentage;
}

void Shape::set_infill_percentage(double infillPercentage)
{
    mInfillPercentage = infillPercentage;
}

double Shape::get_extrusion_multiplier()
{
    return mExtrusionMultiplier;
}

void Shape::set_extrusion_multiplier(double extrusionMultiplier)
{
    mExtrusionMultiplier = extrusionMultiplier;
}

double Shape::get_width()
{
    return mWidth;
}

void Shape::set_width(double layerWidth)
{
    mWidth = layerWidth;
}

double Shape::get_length()
{
    return mLength;
}

void Shape::set_length(double layerLength)
{
    mLength = layerLength;
}

void Shape::set_auto_adjust_layer(bool adjustLayer)
{
    mAutoAdjustLayer = adjustLayer;
}

double Shape::get_resolution()
{
    return mResolution;
}

void Shape::set_resolution(double resolution)
{
    mResolution = resolution;
}

double Shape::get_extrusion_width()
{
    return mExtrusionWidth;
}

std::vector<Path>* Shape::get_path_list()
{
    std::vector<Path>* pathList = new std::vector<Path>;
    unsigned int numberOfLayers = static_cast<unsigned int>(get_layer_list().size());
    for (unsigned int i{0}; i<numberOfLayers; i++)
    {
        Layer* layer = get_layer(i);
        std::vector<Path*> layerPathList = layer->get_path_list();
        unsigned int numberOfPaths = static_cast<unsigned int>(layerPathList.size());
        for (unsigned int j{0}; j<numberOfPaths; j++)
        {
            Path* path = layerPathList.at(j);
            pathList->push_back(*path);
        }
    }
    return pathList;
}

void Shape::set_top_width(double topWidth)
{
    if (topWidth < 0)
    {
        double minimumWidth{0.26};
        topWidth = minimumWidth;
    }
    mTopWidth = topWidth;
}

double Shape::get_top_width() const
{
    return mTopWidth;
}

double Shape::get_layer_width(unsigned int layerNumber)
{
    double shapeWidth = get_width();
    double shapeHeight = get_height();
    double topWidth = get_top_width();
    double widthDifference = shapeWidth-topWidth;
    double theta = std::atan2(shapeHeight,widthDifference);
    double layerHeight = (layerNumber+0.5)*get_layer_height();
    double heightDifference = shapeHeight-layerHeight;
    double layerWidth = topWidth + heightDifference/std::tan(theta);
    return layerWidth;
}

void Shape::set_infill_angle(double thetaInDegrees)
{
    mInfillAngle = thetaInDegrees;
}

double Shape::get_infill_angle() const
{
    return mInfillAngle;
}

void  Shape::refresh()
{
    set_height(mHeight);
    set_width(mWidth);
    set_top_width(mTopWidth);
    set_length(mLength);
    reset_layer_height(mLayerHeight);
    set_extrusion_width(mExtrusionWidth);
    set_extrusion_multiplier(mExtrusionMultiplier);
    set_infill_angle(mInfillAngle);
    set_infill_percentage(mInfillPercentage);
    set_auto_adjust_layer(mAutoAdjustLayer);
    set_resolution(mResolution);
    create_layers();
}

std::vector<Point> Shape::get_simplified_point_list()
{
    std::vector<Point> pointList;
    std::vector<Layer*> layerList = get_layer_list();
    unsigned int numberOfLayers = static_cast<unsigned int>(layerList.size());
    for (unsigned int i{0}; i<numberOfLayers; i++)
    {
        Layer *layer = layerList[i];
        std::vector<Point> pointsInLayer = layer->get_simplified_point_list();
        unsigned int numberOfPointsInLayer = static_cast<unsigned int>(pointsInLayer.size());
        for (unsigned int j{0}; j<numberOfPointsInLayer; j++)
        {
            Point point = pointsInLayer[j];
            pointList.push_back(point);
        }
    }
    return pointList;
}
