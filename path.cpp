#include "path.h"

Path::~Path()
{

}

Path::Path():mPointList{new std::vector<Point*>}
{
    create_points();
}

Path::Path(Point start, Point end):mPointList{new std::vector<Point*>}
{
    set_start(start);
    set_end(end);
    create_points();
}

Path::Path(Point start, Point end, double diameter):mPointList{new std::vector<Point*>}
{
    set_start(start);
    set_end(end);
    set_diameter(diameter);
    create_points();
}

Path::Path(Point start, Point end, double diameter, int pathNumber):mPointList{new std::vector<Point*>}
{
    set_start(start);
    set_end(end);
    set_diameter(diameter);
    set_path_number(pathNumber);
    create_points();
}

Path::Path(Point start, Point end, double diameter, int pathNumber, double resolution):mPointList{new std::vector<Point*>}
{
    set_start(start);
    set_end(end);
    set_diameter(diameter);
    set_path_number(pathNumber);
    set_resolution(resolution);
    create_points();
}

Path::Path(Point start, Point end, double diameter, int pathNumber, double resolution, double shapeWidth, double shapeLength, double shapeHeight):mPointList{new std::vector<Point*>}
{
    set_start(start);
    set_end(end);
    set_diameter(diameter);
    set_path_number(pathNumber);
    set_resolution(resolution);
    set_shape_width(shapeWidth);
    set_shape_length(shapeLength);
    set_shape_height(shapeHeight);
    create_points();
}

Path::Path(Point start, Point end, double diameter, int pathNumber, double resolution, double shapeWidth, double shapeLength, double shapeHeight, int layerNumber):mPointList{new std::vector<Point*>}
{
    set_start(start);
    set_end(end);
    set_diameter(diameter);
    set_path_number(pathNumber);
    set_resolution(resolution);
    set_shape_width(shapeWidth);
    set_shape_length(shapeLength);
    set_shape_height(shapeHeight);
    set_layer_number(layerNumber);
    create_points();
}

Point Path::get_start() const
{
    return mStart;
}

void Path::set_start(Point const startPoint)
{
    mStart = startPoint;
}

Point Path::get_end() const
{
    return mEnd;
}

void Path::set_end(Point const endPoint)
{
    mEnd = endPoint;
}

double Path::get_diameter() const
{
    return mDiameter;
}

void Path::set_diameter(double const diameter)
{
    mDiameter = diameter;
}

int Path::get_number_of_points() const
{
    double length = get_length();
    double exactNumberOfSegments = length/mResolution;
    int flooredNumberOfSegments = int(floor(exactNumberOfSegments));
    int numberOfPoints{flooredNumberOfSegments+1};
    return numberOfPoints;
}

void Path::set_resolution(double const resolution)
{
    mResolution = resolution;
}

double Path::get_resolution() const
{
    return mResolution;
}

double Path::get_length() const
{
    Point pathVector = mStart-mEnd;
    double length = pathVector.get_magnitude();
    return length;
}

void Path::adjust_point_spacing()
{
    double length = get_length();
    double exactNumberOfSegments = length/mResolution;
    int flooredNumberOfSegments = int(floor(exactNumberOfSegments));
    if (flooredNumberOfSegments < 1)
    {
        flooredNumberOfSegments = 1;
    }

    double realResolution = length/flooredNumberOfSegments;
    set_resolution(realResolution);
}

void Path::create_points()
{
    adjust_point_spacing();
    int numberOfPoints = get_number_of_points();
    for (int i{0}; i<numberOfPoints; i++)
    {
        Point* newPoint = create_new_point(i,numberOfPoints);
        mPointList->push_back(newPoint);
    }
}

std::vector<Point*> Path::get_point_list()
{
    return *mPointList;
}

Point* Path::create_new_point(int pointNumber, int numberOfPointsInPath) const
{
    Point pathVector = mEnd-mStart;
    double pathLength = pathVector.get_magnitude();
    double stepLength = pathLength/(numberOfPointsInPath-1);
    Point stepDirection = pathVector.normalize();
    Point step = stepDirection*stepLength;
    Point pointInfo = (mStart + step*pointNumber); // set material here
    double material = get_material(pointInfo);
    pointInfo.set_material(material);
    double diameter = get_diameter();
    pointInfo.set_diameter(diameter);
    Point* newPoint = new Point(pointInfo);
    return newPoint;
}

double Path::get_material(Point pointInfo) const
{
    double material{0};
    // Note: Change this code to define the material throughout the print.
    //    double xLocation = pointInfo.get_x()-mShapeWidth/2;
    //    double yLocation = pointInfo.get_y()-mShapeLength/2;
    //    double zLocation = pointInfo.get_z()-mShapeHeight/2;
    //    double magnitude = pointInfo.get_magnitude();
    //    double locationMagnitude = sqrt(xLocation*xLocation + yLocation*yLocation + zLocation*zLocation);
    //    double shapeMagnitude = sqrt(mShapeHeight*mShapeHeight + mShapeLength*mShapeLength + mShapeWidth*mShapeWidth)/2;
    //    material = locationMagnitude/shapeMagnitude;
    double xLocation = pointInfo.get_x();
    double yLocation = pointInfo.get_y();
    double zLocation = pointInfo.get_z();
    int layerNumber = get_layer_number();
    //    if (layerNumber%2 == 0)
    //    {
    //        material = 1;
    //    }
    if (layerNumber < 16)
    {
        if (xLocation > 5)
        {
            material = 1;
        }
    }
    else
    {
        if (xLocation < 5)
        {
            material = 1;
        }
    }
    return material;
}

void Path::set_path_number(int const pathNumber)
{
    mPathNumber = pathNumber;
}

int Path::get_path_number() const
{
    return mPathNumber;
}

void Path::set_shape_width(double const shapeWidth)
{
    mShapeWidth = shapeWidth;
}

void Path::set_shape_length(double const shapeLength)
{
    mShapeLength = shapeLength;
}

void Path::set_shape_height(double const shapeHeight)
{
    mShapeHeight = shapeHeight;
}

void Path::set_layer_number(int const layerNumber)
{
    mLayerNumber = layerNumber;
}

int Path::get_layer_number() const
{
    return mLayerNumber;
}
