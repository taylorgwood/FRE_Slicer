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

Path::Path(Point start, Point end, double diameter, double layerLocation):mPointList{new std::vector<Point*>}
{
    set_start(start);
    set_end(end);
    set_diameter(diameter);
    set_z(layerLocation);
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
    double exactNumberOfPoints = mLength/mResolution;
    int flooredNumberOfPoints = int(floor(mLength/mResolution));
    int numberOfPoints{flooredNumberOfPoints};
    if ((exactNumberOfPoints-flooredNumberOfPoints)>=0.5)
    {
        numberOfPoints = flooredNumberOfPoints+1;
    }
    if (numberOfPoints==1)
    {
        numberOfPoints = 2;
    }
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
    return mLength;
}

void Path::adjust_point_spacing()
{
    int numberOfPoints = get_number_of_points();
    double realResolution = mLength/numberOfPoints;
    set_resolution(realResolution);
}

void Path::create_points()
{
    int numberOfPoints = get_number_of_points();
    adjust_point_spacing();
    for (int i{0}; i<numberOfPoints; i++)
    {
        Point* newPoint = new Point();
        double xLocation{1};
        double yLocation{2};
        double zLocation = mZ;
        double material{3};
        Point* newPoint2 = new Point(xLocation,yLocation,zLocation,material);
        mPointList->push_back(newPoint2);
    }
}

std::vector<Point*> Path::get_point_list()
{
    return *mPointList;
}

void Path::set_z(double const layerLocation)
{
    mZ = layerLocation;
}

double Path::get_z() const
{
    return mZ;
}
