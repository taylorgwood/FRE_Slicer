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

Layer::Layer(unsigned int number, double location):mPathList{new std::vector<Path*>}
{
    set_number(number);
    set_location(location);
    set_infill_size();
    set_corners();
    create_paths();
}

Layer::Layer(unsigned int number, double location, double length, double width):mPathList{new std::vector<Path*>}
{
    set_number(number);
    set_location(location);
    set_length(length);
    set_width(width);
    //        set_infill_size();
    create_paths();
}

Layer::Layer(unsigned int number, double location, double length, double width, double extrusionMultiplier, double extrusionWidth, double infillPercentage, double resolution):mPathList{new std::vector<Path*>}
{
    set_number(number);
    set_location(location);
    set_length(length);
    set_width(width);
    set_infill_percentage(infillPercentage);
    set_extrusion_multiplier(extrusionMultiplier);
    set_extrusion_width(extrusionWidth);
    //    set_infill_size();
    set_resolution(resolution);
    create_paths();
}

Layer::Layer(unsigned int number, double location, double length, double width, double extrusionMultiplier, double extrusionWidth, double infillPercentage, double resolution, double height):mPathList{new std::vector<Path*>}
{
    set_number(number);
    set_location(location);
    set_length(length);
    set_width(width);
    set_infill_percentage(infillPercentage);
    set_extrusion_multiplier(extrusionMultiplier);
    set_extrusion_width(extrusionWidth);
    set_resolution(resolution);
    //        set_infill_size();
    //        set_corners();
    set_height(height);
    create_paths();
}

Layer::Layer(unsigned int number, double location, double length, double width, double extrusionMultiplier, double extrusionWidth, double infillPercentage, double resolution, double height, double shapeHeight):mPathList{new std::vector<Path*>}
{
    set_number(number);
    set_location(location);
    set_length(length);
    set_width(width);
    set_infill_percentage(infillPercentage);
    set_extrusion_multiplier(extrusionMultiplier);
    set_extrusion_width(extrusionWidth);
    set_resolution(resolution);
    //        set_infill_size();
    //        set_corners();
    set_height(height);
    set_shape_height(shapeHeight);
    create_paths();
}

Layer::Layer(unsigned int number, double location, double length, double width, double extrusionMultiplier, double extrusionWidth, double infillPercentage, double resolution, double height, double shapeHeight, double infillAngle):mPathList{new std::vector<Path*>}
{
    set_number(number);
    set_location(location);
    set_length(length);
    set_width(width);
    set_infill_percentage(infillPercentage);
    set_infill_angle(infillAngle);
    set_extrusion_multiplier(extrusionMultiplier);
    set_extrusion_width(extrusionWidth);
    set_resolution(resolution);
    //    set_infill_size();
    //    set_corners();
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
    double extrusionWidth = get_extrusion_width();
    //    double area = mLength*mWidth;
    double height = get_height();
    double extrusionMultiplier = get_extrusion_multiplier();
    double diameterOfPrint = extrusionMultiplier*sqrt(height*4*extrusionWidth/(pi));
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
    mExtrusionWidth = extrusionWidth;
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
    unsigned int numberOfPaths = get_number_of_infill_paths();
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

double Layer::get_modified_extrusion_width()
{
    double modifiedExtrusionWidth = get_extrusion_width();
    if (mExtrusionModificationApplied == false)
    {
        double infillRatio = mInfillPercentage/100;
        modifiedExtrusionWidth = mExtrusionWidth/infillRatio;
        mExtrusionModificationApplied = true;
    }
    return modifiedExtrusionWidth;
}

unsigned int Layer::get_number_of_infill_paths()
{
    unsigned int numberOfInfillPaths{0};
    double perpendicularToPathDistance = get_perpendicular_to_path_distance();
    double modifiedExtrusionWidth = get_modified_extrusion_width();
    double exactNumberOfPaths = (perpendicularToPathDistance/modifiedExtrusionWidth);
    double flooredNumberOfPaths = floor(exactNumberOfPaths);
    numberOfInfillPaths = static_cast<unsigned int>(flooredNumberOfPaths);
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
    set_infill_size();
    std::vector <Point> perimeterPointList = get_perimeter_points();
    unsigned int numberOfPaths = static_cast<unsigned int>(perimeterPointList.size())-1;

    for (unsigned int i{0}; i<numberOfPaths; i++)
    {
        unsigned int pathNumber = i;
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
    //    mInfillLength = length-get_modified_extrusion_width();
    //    set_corners();
}

double Layer::get_width() const
{
    return mWidth;
}

void Layer::set_width(double const width)
{
    mWidth = width;
    //    mInfillWidth = width-get_modified_extrusion_width();
    //    set_corners();
}

std::vector<Point> Layer::get_points()
{
    std::vector<Point> pointList;
    unsigned int numberOfPaths = static_cast<unsigned int>(mPathList->size());
    Point* lastPoint = new Point;
    for (unsigned int i{0}; i<numberOfPaths; i++)
    {
        Path* path = get_path_list()[i];
        std::vector<Point*> points = path->get_point_list();
        unsigned int numberOfPoints = static_cast<unsigned int>(points.size());
        for (unsigned int j{0}; j<numberOfPoints; j++)
        {
            Point* point = points[j];
            bool duplicate{false};

            if (abs(lastPoint->get_x() - point->get_x()) < 0.00001)
            {
                if (abs(lastPoint->get_y() - point->get_y()) < 0.00001)
                {
                    duplicate = true;
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

unsigned int Layer::get_number() const
{
    return mNumber;
}

void Layer::set_number(unsigned int const number)
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

Path* Layer::get_path(unsigned int pathNumber)
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

    unsigned int k{0};
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
    unsigned int numberOfPaths = static_cast<unsigned int>(angledRayOriginList.size());
    for (unsigned int i{0}; i<numberOfPaths; i++)
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
    //  D---C
    //  |   |
    //  A---B
    Point cornerA = corners.at(0);
    Point cornerB = corners.at(1);
    Point cornerC = corners.at(2);
    Point cornerD = corners.at(3);

    Point cornerATravel = cornerA;
    cornerATravel.set_travel(true);
    Point cornerBTravel = cornerB;
    cornerBTravel.set_travel(true);
    Point cornerCTravel = cornerC;
    cornerCTravel.set_travel(true);
    Point cornerDTravel = cornerD;
    cornerDTravel.set_travel(true);

    Path bottom(cornerA,cornerB);
    Path right(cornerB,cornerC);
    Path top(cornerC,cornerD);
    Path left(cornerD,cornerA);
    std::vector <Point> bottomSidePoints = get_side_points(bottom);
    std::vector <Point> rightSidePoints = get_side_points(right);
    std::vector <Point> topSidePoints = get_side_points(top);
    std::vector <Point> leftSidePoints = get_side_points(left);
    unsigned int bottomSize = static_cast<unsigned int>(bottomSidePoints.size());
    unsigned int rightSize = static_cast<unsigned int>(rightSidePoints.size());
    unsigned int topSize = static_cast<unsigned int>(topSidePoints.size());
    unsigned int leftSize = static_cast<unsigned int>(leftSidePoints.size());

    if (mNumber%2==0)
    {
        // for Even:---------------------------------------------------------------------------------
        //
        //  D__2_3_7__8__C    A->D = start -> 1, 2, 3, etc.
        //   |/ /  /  / |
        //  1| /  /  /  |11
        //   |/  /  /  /|
        //  4|__/0_/__/_|     0 = theta
        //  A5 6  9  10 B,12

        //        perimeterPointList.push_back(cornerDTravel); // first point is D
        unsigned int pointCount{0};
        unsigned int leftPoint{0};
        unsigned int bottomPoint{0};
        unsigned int topPoint{0};
        unsigned int rightPoint{0};
        bool finish{false};
        while (finish == false)
        {
            // 1---------------------------------------------------------------------------------
            // Left and bottom are paired, top and right are paired.
            //
            // Along left
            if (leftPoint < leftSize) // Left
            {
                if (leftPoint == 0) // start at point D
                {
                    if (leftSize != 0)
                    {
                        perimeterPointList.push_back(cornerDTravel);
                    }
                }
                perimeterPointList.push_back(leftSidePoints.at(leftPoint));
                leftPoint++;
            }
            // Or along bottom
            else
            {
                if (bottomPoint==0) // if going from left to bottom, get corner A
                {
                    if (bottomSize != 0)
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
                if (rightPoint==0) // going from top to right, get corner C
                {
                    if (rightSize != 0)
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
                if (bottomPoint < bottomSize)
                {
                    perimeterPointList.push_back(bottomSidePoints.at(bottomPoint));
                    bottomPoint++;
                }
            }

            pointCount = static_cast<unsigned int> (perimeterPointList.size());
            if (rightPoint == rightSize)
            {
                if (bottomPoint == bottomSize)
                {
                    finish = true;
                    Point lastPoint = perimeterPointList.at(pointCount-1);
                    if ((lastPoint.get_x() - cornerA.get_x()) > 0.001)
                    {
                        perimeterPointList.push_back(cornerBTravel);
                    }
                }
            }
        }

    }
    else
    {
        // for Odd:---------------------------------------------------------------------------------
        //
        // 3,D_4__7__811_C,12    A = start -> 1, 2, 3, etc.
        //   | \  \  \ \|10
        //  2|  \  \  \ |
        //   |\  \  \  \|9
        //   |_\__\0_\__|     0 = theta > 90
        //   A  1  5  6  B

//        perimeterPointList.push_back(cornerA); // first point is A
        unsigned int pointCount{0};
        unsigned int bottomPoint{0};
        unsigned int rightPoint{0};
        unsigned int leftPoint{0};
        unsigned int topPoint{0};
        bool finish{false};
        while (finish == false)
        {
            // 1---------------------------------------------------------------------------------
            // Bottom and right are paired, left and top are paired.
            //
            // Along bottom
            if (bottomPoint < bottomSize) // Bottom
            {
                if (bottomPoint == 0) // start at corner A
                {
                    if (bottomSize != 0)
                    {
                        perimeterPointList.push_back(cornerATravel);
                    }
                }
                perimeterPointList.push_back(bottomSidePoints.at(bottomPoint));
                bottomPoint++;
            }
            // Or along right
            else
            {
                if (rightPoint == 0) // if going from bottom to right, get corner B
                {
                    if (rightSize != 0)
                    {
                        perimeterPointList.push_back(cornerB);
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
                perimeterPointList.push_back(leftSidePoints.at(leftPoint));
                leftPoint++;
            }
            else // Top
            {
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
                if (topPoint == 0) // if going from left to top, get corner D
                {
                    if (topSize != 0)
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
                if (rightPoint < rightSize)
                {
                    perimeterPointList.push_back(rightSidePoints.at(rightPoint));
                    rightPoint++;
                }
            }
            pointCount = static_cast<unsigned int> (perimeterPointList.size());
            if (topPoint == topSize)
            {
                if (rightPoint == rightSize)
                {
                    finish = true;
                    Point lastPoint = perimeterPointList.at(pointCount-1);
                    if ((lastPoint.get_y() - cornerC.get_y()) > 0.001)
                    {
                        perimeterPointList.push_back(cornerBTravel);
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
    unsigned int numberOfPoints = static_cast<unsigned int>(intersectionList.size());
    for (unsigned int i{0}; i<numberOfPoints; i++)
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
    //  D---C
    //  |   |
    //  A---B
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



std::vector <Point> Layer::get_simplified_point_list()
{
    std::vector<Point> cleanPointList;
    unsigned int numberOfPaths = static_cast<unsigned int>(mPathList->size());
    Point* lastPoint = new Point;
    for (unsigned int i{0}; i<numberOfPaths; i++)
    {
        Path* path = get_path_list()[i];
        std::vector<Point*> pointList = path->get_point_list();
        unsigned int numberOfPoints = static_cast<unsigned int>(pointList.size());
        for (unsigned int j{0}; j<numberOfPoints; j++)
        {
            Point* point = pointList[j];
            bool duplicate{false};

            if (abs(lastPoint->get_x() - point->get_x()) < 0.00001)
            {
                if (abs(lastPoint->get_y() - point->get_y()) < 0.00001)
                {
                    duplicate = true;
                }
            }
            if (duplicate == false)
            {
                if (j == 0)
                {
                    cleanPointList.push_back(*point);
                }
                else
                {
                    Point* previousPoint = pointList.at(i-1);
                    double previousMaterial = previousPoint->get_material();
                    double currentMaterial = point->get_material();
                    if (abs(previousMaterial - currentMaterial) > 0.0001)
                    {
                        cleanPointList.push_back(*point);
                    }
                }
            }
            lastPoint = point;
        }
    }
    return  cleanPointList;
}

void Layer::refresh()
{
    mExtrusionModificationApplied = false;
    set_number(mNumber);
    set_location(mLocation);
    set_length(mLength);
    set_width(mWidth);
    set_infill_percentage(mInfillPercentage);
    set_infill_angle(mInfillAngle);
    set_extrusion_multiplier(mExtrusionMultiplier);
    set_extrusion_width(mExtrusionWidth);
    set_resolution(mResolution);
    set_height(mHeight);
    set_shape_height(mShapeHeight);
    create_paths();
}
