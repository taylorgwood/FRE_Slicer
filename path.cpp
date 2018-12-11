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
    double material = get_material(pointNumber,pointInfo);
    pointInfo.set_material(material);
    Point* newPoint = new Point(pointInfo);
    return newPoint;
}

double Path::get_material(int pointNumber, Point pointInfo) const
{
    double material{0};
    if (pointInfo.get_x() < 5)
    {
        material = 0.25;
    }
    else
    {
        material = 0.75;
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
