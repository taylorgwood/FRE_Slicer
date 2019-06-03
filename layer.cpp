#include "layer.h"
#include <cmath>

Layer::~Layer()
{

}

Layer::Layer():mPathList{new std::vector<Path*>}
{
    set_infill_size();
    set_corners();
    create_paths();
}

Layer::Layer(int number, double location):mPathList{new std::vector<Path*>}
{
    set_number(number);
    set_location(location);
    set_infill_size();
    set_corners();
    create_paths();
}

Layer::Layer(int number, double location, double length, double width):mPathList{new std::vector<Path*>}
{
    set_number(number);
    set_location(location);
    set_length(length);
    set_width(width);
    //    set_infill_size();
    create_paths();
}

Layer::Layer(int number, double location, double length, double width, double extrusionMultiplier, double extrusionWidth, double infillPercentage, double resolution):mPathList{new std::vector<Path*>}
{
    set_number(number);
    set_location(location);
    set_length(length);
    set_width(width);
    //    set_infill_size();
    set_extrusion_multiplier(extrusionMultiplier);
    set_extrusion_width(extrusionWidth);
    set_infill_percentage(infillPercentage);
    set_resolution(resolution);
    create_paths();
}

Layer::Layer(int number, double location, double length, double width, double extrusionMultiplier, double extrusionWidth, double infillPercentage, double resolution, double height):mPathList{new std::vector<Path*>}
{
    set_number(number);
    set_location(location);
    set_length(length);
    set_width(width);
    //    set_infill_size();
    //    set_corners();
    set_extrusion_multiplier(extrusionMultiplier);
    set_extrusion_width(extrusionWidth);
    set_infill_percentage(infillPercentage);
    set_resolution(resolution);
    set_height(height);
    create_paths();
}

Layer::Layer(int number, double location, double length, double width, double extrusionMultiplier, double extrusionWidth, double infillPercentage, double resolution, double height, double shapeHeight):mPathList{new std::vector<Path*>}
{
    set_number(number);
    set_location(location);
    set_length(length);
    set_width(width);
    //    set_infill_size();
    //    set_corners();
    set_extrusion_multiplier(extrusionMultiplier);
    set_extrusion_width(extrusionWidth);
    set_infill_percentage(infillPercentage);
    set_resolution(resolution);
    set_height(height);
    set_shape_height(shapeHeight);
    create_paths();
}

Layer::Layer(int number, double location, double length, double width, double extrusionMultiplier, double extrusionWidth, double infillPercentage, double resolution, double height, double shapeHeight, double infillAngle):mPathList{new std::vector<Path*>}
{
    set_number(number);
    set_location(location);
    set_length(length);
    set_width(width);
    //    set_infill_size();
    //    set_corners();
    set_extrusion_multiplier(extrusionMultiplier);
    set_extrusion_width(extrusionWidth);
    set_infill_percentage(infillPercentage);
    set_resolution(resolution);
    set_height(height);
    set_shape_height(shapeHeight);
    set_infill_angle(infillAngle);
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

double Layer::get_diameter_of_print()
{
    //    double volume = get_volume();
    double modifiedExtrusionWidth = get_modified_extrusion_width();
    //    double area = mLength*mWidth;
    double height = get_height();
    double diameterOfPrint = sqrt(height*4*modifiedExtrusionWidth/(pi));
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

double Layer::get_extrusion_width()
{
    return mExtrusionWidth;
}

void Layer::set_extrusion_width(double extrusionWidth)
{
    if (mAutoAdjustPaths == true)
    {
        extrusionWidth = get_adjusted_extrusion_width();
    }
    mExtrusionWidth = extrusionWidth;
    set_infill_size();
    set_corners();
}

double Layer::get_adjusted_extrusion_width()
{
    double adjustedExtrusionWidth{0};
    int numberOfPaths = get_number_of_infill_paths();
    double perpendicularToPathDistance = get_perpendicular_to_path_distance();
    adjustedExtrusionWidth = (perpendicularToPathDistance)/(numberOfPaths);

    return adjustedExtrusionWidth;
}

double Layer::get_perpendicular_to_path_distance()
{
    double theta = get_infill_angle()/180*pi;

    double perpendicularToPathDistance{0};
    if (mNumber%2 == 0)
    {
        perpendicularToPathDistance = mWidth/std::cos(theta);
    }
    else
    {
        perpendicularToPathDistance = mLength/std::sin(theta);
    }
    return perpendicularToPathDistance;
}

double Layer::get_modified_extrusion_width() const
{
    double infillRatio = mInfillPercentage/100;
    double modifiedExtrusionWidth = mExtrusionWidth/infillRatio;
    return modifiedExtrusionWidth;
}

int Layer::get_number_of_infill_paths()
{
    int numberOfInfillPaths{0};
    double perpendicularToPathDistance = get_perpendicular_to_path_distance();
    double modifiedExtrusionWidth = get_modified_extrusion_width();
    double exactNumberOfPaths = (perpendicularToPathDistance/modifiedExtrusionWidth);
    double flooredNumberOfPaths = floor(exactNumberOfPaths);
    numberOfInfillPaths = int(flooredNumberOfPaths);
    if ((exactNumberOfPaths-flooredNumberOfPaths) >= 0.5)
    {
        numberOfInfillPaths = flooredNumberOfPaths+1;
    }

    return numberOfInfillPaths;
}

std::vector<Path*> Layer::get_path_list()
{
    return *mPathList;
}

void Layer::create_paths()
{
    mPathList->clear();
    set_extrusion_width(mExtrusionWidth);
    set_infill_size();
    std::vector <Point> perimeterPointList = get_perimeter_points();
    int numberOfPaths = static_cast<int>(perimeterPointList.size())-1;

    for (int i{0}; i<numberOfPaths; i++)
    {
        int pathNumber = i;
        bool duplicate{false};
        double diameter = get_diameter_of_print();
        double resolution = get_resolution();
        Point currentPoint = perimeterPointList.at(i);
        Point nextPoint = perimeterPointList.at(i+1);
        double smallNumber{0.0001};
        if (abs(currentPoint.get_x() - nextPoint.get_x())<smallNumber)
        {
            if(abs(currentPoint.get_y() - nextPoint.get_y())<smallNumber)
            {
                duplicate = true;
            }
        }
        if (duplicate == false)
        {
            Path* newPath = new Path(currentPoint,nextPoint,diameter,pathNumber,resolution, mWidth, mLength, mShapeHeight,mNumber);
            mPathList->push_back(newPath);
        }
    }
}

double Layer::get_length() const
{
    return mLength;
}

void Layer::set_length(double const length)
{
    mLength = length;
    mInfillLength = length-get_modified_extrusion_width();
    set_corners();
}

double Layer::get_width() const
{
    return mWidth;
}

void Layer::set_width(double const width)
{
    mWidth = width;
    mInfillWidth = width-get_modified_extrusion_width();
    set_corners();
}

std::vector<Point> Layer::get_points()
{
    std::vector<Point> pointList;
    int numberOfPaths = static_cast<int>(mPathList->size());
    Point* lastPoint = new Point;
    for (int i{0}; i<numberOfPaths; i++)
    {
        Path* path = get_path_list()[i];
        std::vector<Point*> points = path->get_point_list();
        int numberOfPoints = static_cast<int>(points.size());
        for (int j{0}; j<numberOfPoints; j++)
        {
            Point* point = points[j];
            bool duplicate{false};

                if (lastPoint->get_x() == point->get_x())
                {
                    if (lastPoint->get_y() == point->get_y())
                    {
                        if (lastPoint->get_z() == point->get_z())
                        {
                            duplicate = true;
                        }
                    }
                }
            if (duplicate == false)
            {
                pointList.push_back(*point);
            }
            lastPoint = point;
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

double Layer::get_perimeter()
{
    double diameter = get_diameter_of_print();
    double perimeter = 2*(mWidth-diameter) + 2*(mLength-diameter);
    return perimeter;
}

Point Layer::get_intersection(Point lineStart, Point lineEnd, Point rayOrigin, double theta)
{
    Point intersectPoint(-1,-1,-1); // if ends like this, no intersect
    Point rayDirection(std::cos(theta),std::sin(theta),0);
    double lengthAlongRay{0};
    double ratioAlongSegment{0};
    Point v1 = rayOrigin-lineStart;
    Point v2 = lineEnd-lineStart;
    Point v3((-1*rayDirection.get_y()),rayDirection.get_x(),rayDirection.get_z());
    lengthAlongRay = ((v2.cross(v1)).get_magnitude())/(v2.dot(v3));
    ratioAlongSegment = (v1.dot(v3))/(v2.dot(v3));
    Point lineDirection = lineEnd-lineStart;

    if (ratioAlongSegment <=1 && ratioAlongSegment >=0) // If within line segment
    {
        intersectPoint = lineStart + lineDirection*ratioAlongSegment;
    }
    return intersectPoint;
}

void Layer::set_infill_angle(double infillAngle)
{
    while(infillAngle > 45)
    {
        infillAngle -= 90;
    }
    while(infillAngle < 0)
    {
        infillAngle += 90;
    }
    mInfillAngle = infillAngle;
    create_paths();
}

double Layer::get_infill_angle()
{
    double infillAngle = mInfillAngle;
    if (mNumber%2 != 0)
    {
        infillAngle += 90;
    }
    return infillAngle;
}

std::vector <Point> Layer::create_angled_ray_origin_list()
{
    std::vector <Point> angledRayOriginList;

    double zLocation = get_location();

    std::vector <Point> corners = get_corners();
    Point cornerA = corners.at(0);
    Point cornerB = corners.at(1);
    Point cornerC = corners.at(2);
    Point cornerD = corners.at(3);

    double modifiedEW = get_modified_extrusion_width();


    double theta = get_infill_angle()/180*pi;
    double angleWidth = modifiedEW/(std::cos(theta));
    if (mNumber%2 !=0)
    {
        angleWidth = modifiedEW/(std::sin(theta));
        theta -= pi;
    }

    int k{0};
    bool bEnd{false};
    while(bEnd == false)
    {
        double yStart = cornerA.get_y();
        double xStart = cornerA.get_x();
        if (mNumber%2 ==0)
        {
            yStart = cornerD.get_y() - angleWidth*k;
        }
        else
        {
            xStart = cornerA.get_y() + angleWidth*k;
        }
        Point origin(xStart,yStart,zLocation);
        Point rayOrigin = origin;

        double bX = get_intersection(cornerA,cornerB,origin,theta).get_z();
        double rX = get_intersection(cornerB,cornerC,origin,theta).get_z();
        double tX = get_intersection(cornerC,cornerD,origin,theta).get_z();
        double lX = get_intersection(cornerD,cornerA,origin,theta).get_z();
        double noX = -1;

        if (bX == noX && rX == noX && tX == noX && lX == noX)
        {
            bEnd = true;
        }
        if (bEnd == false)
        {
            angledRayOriginList.push_back(rayOrigin);
        }
        k++;
    }
    return angledRayOriginList;
}

std::vector <Point> Layer::get_intersection_points_list(Path path)
{
    std::vector <Point> intersectionPoints;

    Point start = path.get_start();
    Point end = path.get_end();
    double theta = get_infill_angle()/180*pi;

    std::vector <Point> angledRayOriginList = create_angled_ray_origin_list();
    int numberOfPaths = angledRayOriginList.size();
    int k{numberOfPaths};
    for (int i{0}; i<numberOfPaths; i++)
    {
        Point intersect = get_intersection(start,end,angledRayOriginList.at(i),theta);
        intersectionPoints.push_back(intersect);
    }

    return intersectionPoints;
}

std::vector <Point> Layer::get_perimeter_points()
{
    std::vector <Point> perimeterPointList;

    std::vector <Point> corners = get_corners();
    Point cornerA = corners.at(0);
    Point cornerB = corners.at(1);
    Point cornerC = corners.at(2);
    Point cornerD = corners.at(3);
    Path bottom(cornerA,cornerB);
    Path right(cornerB,cornerC);
    Path top(cornerC,cornerD);
    Path left(cornerD,cornerA);
    std::vector <Point> bottomSidePoints = get_side_points(bottom);
    std::vector <Point> rightSidePoints = get_side_points(right);
    std::vector <Point> topSidePoints = get_side_points(top);
    std::vector <Point> leftSidePoints = get_side_points(left);
    int bottomSize = static_cast<int>(bottomSidePoints.size());
    int rightSize = static_cast<int>(rightSidePoints.size());
    int topSize = static_cast<int>(topSidePoints.size());
    int leftSize = static_cast<int>(leftSidePoints.size());
    double theta = get_infill_angle()/180*pi;

    if (mNumber%2==0)
    {
        // for Even:---------------------------------------------------------------------------------

        perimeterPointList.push_back(cornerD); // first point is D
        int pointCount{0};
        int leftPoint{0};
        int bottomPoint{0};
        int topPoint{0};
        int rightPoint{0};
        bool finish{false};
        while (finish == false)
        {
            // 1---------------------------------------------------------------------------------
            if (leftPoint < leftSize) // Left
            {
                perimeterPointList.push_back(leftSidePoints.at(leftPoint));
                leftPoint++;
                if (rightSize > 0)
                {
                }
                else
                {
                    if (rightPoint == 0)
                    {
                        perimeterPointList.push_back(cornerB);
                    }
                }
            }
            else // Bottom
            {
                if (bottomSize > 0)
                {
                }
                else
                {
                    if (bottomPoint==0)
                    {
                        perimeterPointList.push_back(cornerA);
                    }
                }
                if (bottomPoint < bottomSize)
                {
                    perimeterPointList.push_back(bottomSidePoints.at(bottomPoint));
                    bottomPoint++;
                }
            }
            // 2---------------------------------------------------------------------------------
            if (topPoint < topSize) // Top
            {
                perimeterPointList.push_back(topSidePoints.at(topPoint));
                topPoint++;
            }
            else // Right
            {
                if (rightSize > 0)
                {
                }
                else
                {
                    if (rightPoint==0)
                    {
                        perimeterPointList.push_back(cornerC);
                    }
                }

                if (rightPoint < rightSize)
                {
                    perimeterPointList.push_back(rightSidePoints.at(rightPoint));
                    rightPoint++;
                }
            }
            // 3---------------------------------------------------------------------------------
            if (topPoint < topSize) // Top
            {
                perimeterPointList.push_back(topSidePoints.at(topPoint));
                topPoint++;
            }
            else // Right
            {
                if (rightSize > 0)
                {
                }
                else
                {
                    if (rightPoint==0)
                    {
                        perimeterPointList.push_back(cornerC);
                    }
                }
                if (rightPoint < rightSize)
                {
                    perimeterPointList.push_back(rightSidePoints.at(rightPoint));
                    rightPoint++;
                }
            }
            // 4---------------------------------------------------------------------------------
            if (leftPoint < leftSize) // Left
            {
                perimeterPointList.push_back(leftSidePoints.at(leftPoint));
                leftPoint++;
            }
            else // Bottom
            {
                if (bottomSize > 0)
                {
                }
                else
                {
                    if (bottomPoint == 0)
                    {
                    }
                    else
                    {
                        perimeterPointList.push_back(cornerB);
                    }
                }
                if (bottomPoint < bottomSize)
                {
                    perimeterPointList.push_back(bottomSidePoints.at(bottomPoint));
                    bottomPoint++;
                }
            }

            pointCount = static_cast<int> (perimeterPointList.size());
            if (rightPoint == rightSize)
            {
                if (bottomPoint == bottomSize)
                {
                    finish = true;
                    if ((theta) < 0.001)
                    {
                        if (get_number_of_infill_paths()%2 == 0)
                        {
                            perimeterPointList.push_back(cornerA);
                        }
                        else
                        {
                            //                            perimeterPointList.push_back(cornerB);
                        }
                    }
                }
            }
        }

    }
    else
    {
        // for Odd:---------------------------------------------------------------------------------

        perimeterPointList.push_back(cornerA); // first point is A
        int pointCount{0};
        int bottomPoint{0};
        int rightPoint{0};
        int leftPoint{0};
        int topPoint{0};
        bool finish{false};
        while (finish == false)
        {
            // 1---------------------------------------------------------------------------------
            if (bottomPoint < bottomSize) // Bottom
            {
                perimeterPointList.push_back(bottomSidePoints.at(bottomPoint));
                bottomPoint++;
                if (topSize > 0)
                {
                }
                else
                {
                    if (topPoint==0)
                    {
                        perimeterPointList.push_back(cornerD);
                    }
                }
            }
            else // Right
            {
                if (rightSize > 0)
                {
                }
                else
                {
                    if (rightPoint == 0)
                    {
                        perimeterPointList.push_back(cornerD); // not sure about this
                    }
                }
                if (rightPoint < rightSize)
                {
                    perimeterPointList.push_back(rightSidePoints.at(rightPoint));
                    rightPoint++;
                }
            }
            // 2---------------------------------------------------------------------------------
            if (leftPoint < leftSize) // Left
            {
                if (leftSize == 1)
                {
                }
                else
                {
                    perimeterPointList.push_back(leftSidePoints.at(leftPoint));
                }
                leftPoint++;
            }
            else // Top
            {
                if (topSize > 0)
                {
                }
                else
                {
                    if (topPoint==0)
                    {
                        perimeterPointList.push_back(cornerD);
                    }
                }

                if (topPoint < topSize)
                {
                    perimeterPointList.push_back(topSidePoints.at(topPoint));
                    topPoint++;
                }
            }
            // 3---------------------------------------------------------------------------------
            if (leftPoint < leftSize) // Left
            {
                perimeterPointList.push_back(leftSidePoints.at(leftPoint));
                leftPoint++;
            }
            else // Top
            {
                if (topSize > 0)
                {

                }
                else
                {
                    if (topPoint==0)
                    {
                        perimeterPointList.push_back(cornerD);
                    }
                }
                if (topPoint < topSize)
                {
                    perimeterPointList.push_back(topSidePoints.at(topPoint));
                    topPoint++;
                }
            }
            // 4---------------------------------------------------------------------------------
            if (bottomPoint < bottomSize) // Bottom
            {
                perimeterPointList.push_back(bottomSidePoints.at(bottomPoint));
                bottomPoint++;
            }
            else // Right
            {
                if (rightSize > 0)
                {

                }
                else
                {
                    if (rightPoint ==0 )
                    {
                    }
                    else
                    {
                        perimeterPointList.push_back(cornerC);
                    }
                }
                if (rightPoint < rightSize)
                {
                    perimeterPointList.push_back(rightSidePoints.at(rightPoint));
                    rightPoint++;
                }
            }
            pointCount = static_cast<int> (perimeterPointList.size());
            if (topPoint == topSize)
            {
                if (rightPoint == rightSize)
                {
                    finish = true;
                    if ((theta - pi) < 0.001)
                    {
                        perimeterPointList.at(1) = cornerD;
                        int num = get_number_of_infill_paths();
                        if (num%2 == 0)
                        {
                            perimeterPointList.push_back(cornerB);
                        }
                        else
                        {
                            //                            perimeterPointList.push_back(cornerC);
                        }
                    }

                }
            }
        }
    }
    return perimeterPointList;
}


std::vector <Point> Layer::get_side_points(Path side)
{
    std::vector <Point> perimeterPointList;
    std::vector <Point> intersectionList = get_intersection_points_list(side);
    Point noIntersection(-1,-1,-1);
    Point intersection;
    int numberOfPoints = static_cast<int>(intersectionList.size());
    for (int i{0}; i<numberOfPoints; i++)
    {
        intersection = intersectionList.at(i);
        if (intersection.get_z() == noIntersection.get_z())
        {
        }
        else
        {
            perimeterPointList.push_back(intersection);
        }
    }

    return perimeterPointList;
}

double Layer::get_infill_width() const
{
    return mInfillWidth;
}

double Layer::get_infill_length() const
{
    return mInfillLength;
}

void Layer::set_corners()
{
    double diameter = get_diameter_of_print();
    double zLocation = get_location();
    double infillWidth = get_infill_width();
    double infillLength = get_infill_length();
    double mEW = get_modified_extrusion_width();
    Point pointA(mEW/2,mEW/2,zLocation);
    Point pointB(infillWidth+mEW/2,mEW/2,zLocation);
    Point pointC(infillWidth+mEW/2,infillLength+mEW/2,zLocation);
    Point pointD(mEW/2,infillLength+mEW/2,zLocation);
    mPointA = pointA;
    mPointB = pointB;
    mPointC = pointC;
    mPointD = pointD;
}

std::vector <Point> Layer::get_corners()
{
    std::vector <Point> corners;
    corners.push_back(mPointA);
    corners.push_back(mPointB);
    corners.push_back(mPointC);
    corners.push_back(mPointD);
    return corners;
}

void Layer::set_infill_size()
{
    //    double diameter = get_diameter_of_print();
    double modifiedExtrusionWidth = get_modified_extrusion_width();
    mInfillLength = mLength-modifiedExtrusionWidth;
    mInfillWidth = mWidth-modifiedExtrusionWidth;
}
