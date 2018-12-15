#include "layer.h"
#include <cmath>

Layer::~Layer()
{

}

Layer::Layer():mPathList{new std::vector<Path*>}
{
    create_paths();
}

Layer::Layer(int number, double location):mPathList{new std::vector<Path*>}
{
    set_number(number);
    set_location(location);
    create_paths();
}

Layer::Layer(int number, double location, double length, double width):mPathList{new std::vector<Path*>}
{
    set_number(number);
    set_location(location);
    set_length(length);
    set_width(width);
    create_paths();
}

Layer::Layer(int number, double location, double length, double width, double extrusionMultiplier, double extrusionWidth, double infillPercentage, double resolution, bool adjustPath):mPathList{new std::vector<Path*>}
{
    set_number(number);
    set_location(location);
    set_length(length);
    set_width(width);
    set_extrusion_multiplier(extrusionMultiplier);
    set_extrusion_width(extrusionWidth);
    set_infill_percentage(infillPercentage);
    set_resolution(resolution);
    set_auto_adjust_path(adjustPath);
    create_paths();
}

Layer::Layer(int number, double location, double length, double width, double extrusionMultiplier, double extrusionWidth, double infillPercentage, double resolution, bool adjustPath, double height):mPathList{new std::vector<Path*>}
{
    set_number(number);
    set_location(location);
    set_length(length);
    set_width(width);
    set_extrusion_multiplier(extrusionMultiplier);
    set_extrusion_width(extrusionWidth);
    set_infill_percentage(infillPercentage);
    set_resolution(resolution);
    set_auto_adjust_path(adjustPath);
    set_height(height);
    create_paths();
}

Layer::Layer(int number, double location, double length, double width, double extrusionMultiplier, double extrusionWidth, double infillPercentage, double resolution, bool adjustPath, double height, double shapeHeight):mPathList{new std::vector<Path*>}
{
    set_number(number);
    set_location(location);
    set_length(length);
    set_width(width);
    set_extrusion_multiplier(extrusionMultiplier);
    set_extrusion_width(extrusionWidth);
    set_infill_percentage(infillPercentage);
    set_resolution(resolution);
    set_auto_adjust_path(adjustPath);
    set_height(height);
    set_shape_height(shapeHeight);
    create_paths();
}

double Layer::get_height() const
{
    return mHeight;
}

void Layer::set_height(const double height)
{
    mHeight = height;
}

double Layer::get_extrusion_width() const
{
    return mExtrusionWidth;
}

void Layer::set_extrusion_width(double extrusionWidth)
{
    mExtrusionWidth = extrusionWidth;
    if (mAutoAdjustPaths==true)
    {
        extrusionWidth = get_adjusted_extrusion_width();
    }
    mExtrusionWidth = extrusionWidth;
}

double Layer::get_infill_percentage() const
{
    return mInfillPercentage;
}

void Layer::set_infill_percentage(const double infillPercentage)
{
    mInfillPercentage = infillPercentage;
}

double Layer::get_extrusion_multiplier() const
{
    return mExtrusionMultiplier;
}

void Layer::set_extrusion_multiplier(const double extrusionMultiplier)
{
    mExtrusionMultiplier = extrusionMultiplier;
}

double Layer::get_modified_extrusion_width() const
{
    double infillRatio = mInfillPercentage/100;
    double modifiedExtrusionWidth = mExtrusionWidth/infillRatio;
    return modifiedExtrusionWidth;
}

double Layer::get_diameter_of_print() const
{
    double volume = get_volume();
    double realExtrusionWidth = get_modified_extrusion_width();
    double area = mLength*mWidth;
    double diameterOfPrint = sqrt(volume*4*realExtrusionWidth/(area*pi));
    return diameterOfPrint;
}

double Layer::get_volume() const
{
    double infillRatio = mInfillPercentage/100;
    double area = mLength*mWidth;
    double volume = area*mHeight*infillRatio*mExtrusionMultiplier;
    return volume;
}

double Layer::get_area() const
{
    return mWidth*mLength;
}

int Layer::get_number_of_paths()
{
    double exactNumberOfPaths = mWidth/get_modified_extrusion_width();
    int flooredNumberOfPaths = int(floor(mWidth/get_modified_extrusion_width()));
    if (mNumber%2 != 0)
    {
        exactNumberOfPaths = mLength/get_modified_extrusion_width();
        flooredNumberOfPaths = int(floor(mLength/get_modified_extrusion_width()));
    }
    int numberOfPaths{flooredNumberOfPaths};
    if ((exactNumberOfPaths-flooredNumberOfPaths)>=0.5)
    {
        numberOfPaths = flooredNumberOfPaths+1;
    }
    if (numberOfPaths==1)
    {
        numberOfPaths = 2;
    }
    return numberOfPaths;
}

double Layer::get_adjusted_extrusion_width()
{
    int numberOfPaths = get_number_of_paths();
    double adjustedExtrusionWidth = mWidth/numberOfPaths;
    if (mNumber%2 != 0)
    {
        adjustedExtrusionWidth = mLength/numberOfPaths;
    }
    return adjustedExtrusionWidth;
}

std::vector<Path*> Layer::get_path_list()
{
    return *mPathList;
}

void Layer::create_paths()
{
    int numberOfPaths = get_number_of_paths();
    double extrusionWidth = get_extrusion_width();
    if (mAutoAdjustPaths == true)
    {
        double extrusionWidth = get_adjusted_extrusion_width();
    }
    for (int i{0}; i<numberOfPaths; i++)
    {
        int pathNumber = i;
        std::vector<Point> turnPoints = get_turn_points(pathNumber);
        Point start = turnPoints[0];
        Point end = turnPoints[1];
        double diameter = get_diameter_of_print();
        double resolution = get_resolution();
        Path* newPath = new Path(start,end,diameter,pathNumber,resolution, mWidth, mLength, mShapeHeight);
        if (pathNumber == 0)
        {
            int firstPointInPath{0};
            newPath->get_point_list().at(firstPointInPath)->set_diameter(0);
        }
        mPathList->push_back(newPath);
    }
}

double Layer::get_length() const
{
    return mLength;
}

void Layer::set_length(double const length)
{
    mLength = length;
}

double Layer::get_width() const
{
    return mWidth;
}

void Layer::set_width(double const width)
{
    mWidth = width;
}

std::vector<Point> Layer::get_points()
{
    std::vector<Point> pointList;
    size_t numberOfPaths = mPathList->size();
    for (int i{0}; i<numberOfPaths; i++)
    {
        Path* path = get_path_list()[i];
        std::vector<Point*> points = path->get_point_list();
        size_t numberOfPoints = points.size();
        for (int j{0}; j<numberOfPoints; j++)
        {
            Point* point = points[j];
            pointList.push_back(*point);
        }
    }
    return pointList;
}

int Layer::get_number() const
{
    return mNumber;
}

void Layer::set_number(int const number)
{
    mNumber = number;
}

double Layer::get_location() const
{
    return mLocation;
}

void Layer::set_location(double const location)
{
    mLocation = location;
}

std::vector<Point> Layer::get_turn_points(int pathNumber)
{
    double xStart{0};
    double yStart{0};
    double xEnd{0};
    double yEnd{0};
    double zLocation = get_location();
    int numberOfPaths = get_number_of_paths();
    int layerNumber = get_number();
    double extrusionWidthBuffer = get_adjusted_extrusion_width()/2;
    if (layerNumber%2 == 0)
    {
        if (pathNumber%2 == 0)
        {
            xStart = 0;
            xEnd   = mLength;
        }
        else
        {
            xStart = mLength;
            xEnd   = 0;
        }
        yStart = mWidth/numberOfPaths*pathNumber+extrusionWidthBuffer;
        yEnd   = yStart;
    }
    else
    {
        if (pathNumber%2 == 0)
        {
            yStart = 0;
            yEnd   = mWidth;
        }
        else
        {
            yStart = mWidth;
            yEnd   = 0;
        }
        xStart = mLength/numberOfPaths*pathNumber+extrusionWidthBuffer;
        xEnd   = xStart;
    }
    Point startPoint{xStart,yStart,zLocation};
    Point endPoint{xEnd,yEnd,zLocation};
    std::vector <Point> turnPoints;
    turnPoints.push_back(startPoint);
    turnPoints.push_back(endPoint);
    return turnPoints;
}

Path* Layer::get_path(int pathNumber)
{
    std::vector<Path*> pathList = get_path_list();
    Path* path = pathList[pathNumber];
    return path;
}

void Layer::set_auto_adjust_path(bool adjustPaths)
{
    mAutoAdjustPaths = adjustPaths;
}

double Layer::get_resolution() const
{
    return mResolution;
}

void Layer::set_resolution(double const resolution)
{
    mResolution = resolution;
}

void Layer::set_shape_height(double shapeHeight)
{
    mShapeHeight = shapeHeight;
}
