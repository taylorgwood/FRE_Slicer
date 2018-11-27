#include "layer.h"
#include <cmath>

Layer::~Layer()
{

}

Layer::Layer():mPathList{new std::vector<Path*>}
{
    create_paths();
}

Layer::Layer(int layerNumber, double layerLocation):mPathList{new std::vector<Path*>}
{
    set_number(layerNumber);
    set_location(layerLocation);
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

void Layer::set_extrusion_width(const double extrusionWidth)
{
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
    return mHeight;
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

void Layer::adjust_extrusion_width()
{
    int numberOfPaths = get_number_of_paths();
    double realExtrusionWidth = mWidth/numberOfPaths;
    set_extrusion_width(realExtrusionWidth);
}

std::vector<Path*> Layer::get_path_list()
{
    return *mPathList;
}

void Layer::create_paths()
{
    int numberOfPaths = get_number_of_paths();
    adjust_extrusion_width();
    for (int i{0}; i<numberOfPaths; i++)
    {
        Point start = get_path_start(i);
        Point end   = get_path_end(i);
        double diameter = get_diameter_of_print();
        Path* newPath = new Path(start,end,diameter);
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
        size_t numberOfPoints = path->get_number_of_points();
        std::vector<Point*> points = path->get_point_list();
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

Point Layer::get_path_start(int pathNumber)
{
    double xLocation{0};
    double yLocation{0};
    double zLocation = get_location();
    int numberOfPaths = get_number_of_paths();
    int layerNumber = get_number();
    if (layerNumber%2 == 0)
    {
        yLocation = mWidth/numberOfPaths*pathNumber;
    }
    else
    {
        xLocation = mLength/numberOfPaths*pathNumber;
    }

    Point start{xLocation,yLocation,zLocation};
    return start;
}

Point Layer::get_path_end(int pathNumber)
{
    double xLocation{mLength};
    double yLocation{mWidth};
    double zLocation = get_location();
    int numberOfPaths = get_number_of_paths();
    int layerNumber = get_number();
    if (layerNumber%2 == 0)
    {
        yLocation = mWidth/numberOfPaths*pathNumber;
    }
    else
    {
        xLocation = mLength/numberOfPaths*pathNumber;
    }

    Point end{xLocation,yLocation,zLocation};
    return end;
}
