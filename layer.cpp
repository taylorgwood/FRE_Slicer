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
    set_infill_size();
    set_corners();
    create_paths();
}

Layer::Layer(int number, double location, double length, double width, double extrusionMultiplier, double extrusionWidth, double infillPercentage, double resolution):mPathList{new std::vector<Path*>}
{
    set_number(number);
    set_location(location);
    set_length(length);
    set_width(width);
    set_infill_size();
    set_corners();
    set_extrusion_multiplier(extrusionMultiplier);
    set_extrusion_width(extrusionWidth);
    set_infill_percentage(infillPercentage);
    set_resolution(resolution);
    //    set_auto_adjust_path(adjustPath);
    create_paths();
}

Layer::Layer(int number, double location, double length, double width, double extrusionMultiplier, double extrusionWidth, double infillPercentage, double resolution, double height):mPathList{new std::vector<Path*>}
{
    set_number(number);
    set_location(location);
    set_length(length);
    set_width(width);
    set_infill_size();
    set_corners();
    set_extrusion_multiplier(extrusionMultiplier);
    set_extrusion_width(extrusionWidth);
    set_infill_percentage(infillPercentage);
    set_resolution(resolution);
    //    set_auto_adjust_path(adjustPath);
    set_height(height);
    create_paths();
}

Layer::Layer(int number, double location, double length, double width, double extrusionMultiplier, double extrusionWidth, double infillPercentage, double resolution, double height, double shapeHeight):mPathList{new std::vector<Path*>}
{
    set_number(number);
    set_location(location);
    set_length(length);
    set_width(width);
    set_infill_size();
    set_corners();
    set_extrusion_multiplier(extrusionMultiplier);
    set_extrusion_width(extrusionWidth);
    set_infill_percentage(infillPercentage);
    set_resolution(resolution);
    //    set_auto_adjust_path(adjustPath);
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
    double volume = get_volume();
    //    double infillRatio = mInfillPercentage/100;
    double modifiedExtrusionWidth = get_modified_extrusion_width();
    double area = mLength*mWidth;
    double diameterOfPrint = sqrt(volume*4*modifiedExtrusionWidth/(area*pi));
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
}

double Layer::get_adjusted_extrusion_width()
{
    double adjustedExtrusionWidth{0};
    int numberOfPaths = get_number_of_infill_paths();
    //    double diameter = get_diameter_of_print();
    double perpendicularToPathDistance = get_perpendicular_to_path_distance();
    adjustedExtrusionWidth = (perpendicularToPathDistance)/(numberOfPaths);

    return adjustedExtrusionWidth;
}

double Layer::get_perpendicular_to_path_distance()
{
    double theta = get_infill_angle()/180*pi;
    //    double zLocation = get_location();
    //    std::vector <Point> corners = get_corners();
    //    Point cornerA = corners.at(0);
    //    Point cornerB = corners.at(1);
    //    Point cornerC = corners.at(2);
    //    Point cornerD = corners.at(3);
    //    Point pointE(cos(theta+pi/2),sin(theta+pi/2),0);
    ////    double magnitudeE =
    ////    Point vectorE = (pointE).normalize();
    //    Point diagonalDistance = (cornerC-cornerA);
    //    Point diagonalDistanceNormalized = diagonalDistance.normalize();
    // = (vectorE.dot(diagonalDistanceNormalized))/std::cos(theta); //*diagonalDistance.get_magnitude();

    double perpendicularToPathDistance{0};
    if (mNumber%2 == 0)
    {
        perpendicularToPathDistance = mWidth/std::cos(theta);
    }
    else
    {
        perpendicularToPathDistance = mLength/std::cos(theta);
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
    double extrusionWidth = get_extrusion_width();
    double exactNumberOfPaths = (perpendicularToPathDistance/extrusionWidth);
    double flooredNumberOfPaths = floor(exactNumberOfPaths);
    numberOfInfillPaths = int(flooredNumberOfPaths);
    if ((exactNumberOfPaths-flooredNumberOfPaths) >= 0.5)
    {
        numberOfInfillPaths = flooredNumberOfPaths+1;
    }


    //    double modifiedEW = get_modified_extrusion_width();
    //    int numberOfPaths{0};
    //    int flooredNumberOfPaths{0};
    //    double exactNumberOfPaths{0};

    //    if (mInfill45 == true)
    //    {
    //        //        double perimeter = get_perimeter();
    //        //        double sinOf45Degrees = 0.70710678118;
    //        //        double width45 = modifiedEW/sinOf45Degrees;
    //        //        exactNumberOfPaths = (perimeter/2)/width45;
    //        //        flooredNumberOfPaths = int(floor(exactNumberOfPaths));
    //    }
    //    else
    //    {
    //        exactNumberOfPaths = (mWidth/modifiedEW);
    //        flooredNumberOfPaths = int(floor(exactNumberOfPaths));
    //        if (mNumber%2 != 0)
    //        {
    //            exactNumberOfPaths = (mLength/modifiedEW);
    //            flooredNumberOfPaths = int(floor(exactNumberOfPaths));
    //        }
    //    }

    //    numberOfPaths = flooredNumberOfPaths;

    //    if ((exactNumberOfPaths-flooredNumberOfPaths) >= 0.5)
    //    {
    //        numberOfPaths = flooredNumberOfPaths+1;
    //    }
    //    if (numberOfPaths==1)
    //    {
    //        numberOfPaths = 2;
    //    }
    return numberOfInfillPaths;
}

std::vector<Path*> Layer::get_path_list()
{
    return *mPathList;
}

void Layer::create_paths()
{
    //    int numberOfPaths = get_number_of_infill_paths();

    std::vector <Point> perimeterPointList = get_perimeter_points();
    int numberOfPaths = static_cast<int>(perimeterPointList.size())-1;

    for (int i{0}; i<numberOfPaths; i++)
    {
        int pathNumber = i;
        double diameter = get_diameter_of_print();
        double resolution = get_resolution();
        Point currentPoint = perimeterPointList.at(i);
        Point nextPoint = perimeterPointList.at(i+1);
        Path* newPath = new Path(currentPoint,nextPoint,diameter,pathNumber,resolution, mWidth, mLength, mShapeHeight);

        //        newPath->set_start(currentPoint);
        //        newPath->set_end(nextPoint);
        mPathList->push_back(newPath);
    }
    //    if (mInfill45 == true)
    //    {
    //        std::vector <Point> turnPoints = get_45_degree_turn_points();

    //        for (int i{0}; i<numberOfPaths*2; i+=2)
    //        {
    //            int pathNumber = i;
    //            Point start = turnPoints.at(i);
    //            Point end = turnPoints.at(i+1);
    //            double diameter = get_diameter_of_print();
    //            double resolution = get_resolution();
    //            Path* newPath = new Path(start,end,diameter,pathNumber,resolution, mWidth, mLength, mShapeHeight);
    //            if (pathNumber == 0)
    //            {
    //                int firstPointInPath{0};
    //                newPath->get_point_list().at(firstPointInPath)->set_diameter(0);
    //            }
    //            mPathList->push_back(newPath);
    //        }
    //    }
    //    else
    //    {
    //    for (int i{0}; i<numberOfPaths; i++)
    //    {
    //        int pathNumber = i;
    //        std::vector<Point> turnPoints = get_90_degree_turn_points(pathNumber);
    //        Point start = turnPoints[0];
    //        Point end = turnPoints[1];
    //        double diameter = get_diameter_of_print();
    //        double resolution = get_resolution();
    //        Path* newPath = new Path(start,end,diameter,pathNumber,resolution, mWidth, mLength, mShapeHeight);
    //        if (pathNumber == 0)
    //        {
    //            int firstPointInPath{0};
    //            newPath->get_point_list().at(firstPointInPath)->set_diameter(0);
    //        }
    //        mPathList->push_back(newPath);
    //    }
    //    }
}

double Layer::get_length() const
{
    return mLength;
}

void Layer::set_length(double const length)
{
    mLength = length;
    mInfillLength = length-get_diameter_of_print();
}

double Layer::get_width() const
{
    return mWidth;
}

void Layer::set_width(double const width)
{
    mWidth = width;
    mInfillWidth = width-get_diameter_of_print();
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

//std::vector<Point> Layer::get_90_degree_turn_points(int pathNumber)
//{
//    double xStart{0};
//    double yStart{0};
//    double xEnd{0};
//    double yEnd{0};
//    double zLocation = get_location();
//    //    int numberOfPaths = get_number_of_paths();
//    int layerNumber = get_number();
//    double extrusionWidth = get_extrusion_width();
//    double extrusionWidthBuffer = extrusionWidth/2;
//    double diameter = get_diameter_of_print();
//    if (mAutoAdjustPaths == true)
//    {
//        double extrusionWidth = get_adjusted_extrusion_width();
//    }
//    if (layerNumber%2 == 0)
//    {
//        if (pathNumber%2 == 0)
//        {
//            xStart = 0+diameter/2;
//            xEnd   = mLength-diameter/2;
//        }
//        else
//        {
//            xStart = mLength-diameter/2;
//            xEnd   = 0+diameter/2;
//        }
//        yStart = extrusionWidth*pathNumber+extrusionWidthBuffer;
//        yEnd   = yStart;
//    }
//    else
//    {
//        if (pathNumber%2 == 0)
//        {
//            yStart = 0+diameter/2;
//            yEnd   = mWidth-diameter/2;
//        }
//        else
//        {
//            yStart = mWidth-diameter/2;
//            yEnd   = 0+diameter/2;
//        }
//        xStart = extrusionWidth*pathNumber+extrusionWidthBuffer;
//        xEnd   = xStart;
//    }
//    Point startPoint{xStart,yStart,zLocation};
//    Point endPoint{xEnd,yEnd,zLocation};
//    std::vector <Point> turnPoints;
//    turnPoints.push_back(startPoint);
//    turnPoints.push_back(endPoint);
//    return turnPoints;
//}

//std::vector<Point> Layer::get_45_degree_turn_points()
//{
//    std::vector<Point> turnPoints;
//    std::vector<Point> perimeterPoints = get_45_degree_perimeter_points();

//    int numberOfPaths = get_number_of_paths();
//    int numberOfPoints = numberOfPaths*2;
//    for(int i{0}; i<(numberOfPaths); i++)
//    {
//        i;
//        Point startPoint = perimeterPoints.at(i);
//        int endLocation = numberOfPoints-1-i;
//        Point endPoint = perimeterPoints.at(endLocation);
//        turnPoints.push_back(startPoint);
//        turnPoints.push_back(endPoint);
//    }
//    return turnPoints;
//}

//std::vector<Point> Layer::get_perimeter_points()
//{
//    std::vector<Point> perimeterPoints;

//    if (mInfill45 == true)
//    {
//        perimeterPoints = get_45_degree_perimeter_points();
//    }
//    else
//    {
//        perimeterPoints = get_90_degree_perimeter_points();
//    }

//    return perimeterPoints;
//}

//std::vector<Point> Layer::get_45_degree_perimeter_points()
//{
//    std::vector<Point> perimeterPoints;
//    double modifiedEW = get_modified_extrusion_width();
//    double sinOf45Degrees = 0.707107;
//    double width45 = modifiedEW/sinOf45Degrees;
//    double diameter = get_diameter_of_print();
//    double xStart{diameter/2};
//    double yStart{diameter/2};
//    double xEnd = xStart + mWidth-diameter;
//    double yEnd = yStart + mLength-diameter;

//    double traversedLength = width45/2; // start half way
//    int numberOfTurnPoints = get_number_of_paths()*2;
//    for (int i{0};i<numberOfTurnPoints; i++)
//    {
//        Point point(0,0,mLocation);
//        if (mNumber%2 == 0)
//        {
//            if (traversedLength > (mWidth*2+mLength-diameter*3)) // fourth side
//            {
//                point.set_x(xStart);
//                point.set_y(yEnd - (traversedLength - (mWidth*2+mLength-diameter*3)));
//            }
//            else if (traversedLength > (mWidth+mLength-diameter*2)) // third side
//            {
//                point.set_x(xEnd - (traversedLength - (mWidth+mLength-diameter*2)));
//                point.set_y(yEnd);
//            }
//            else if (traversedLength > (mWidth-diameter)) // second side
//            {
//                point.set_x(xEnd);
//                point.set_y(yStart + traversedLength - (mWidth-diameter));
//            }
//            else // first side
//            {
//                point.set_x(xStart+traversedLength);
//                point.set_y(yStart);
//            }

//            traversedLength += width45;
//            perimeterPoints.push_back(point);
//        }
//        else
//        {
//            if (traversedLength > (mWidth*2+mLength-diameter*3)) // fourth side
//            {
//                point.set_x(xEnd);
//                point.set_y(yEnd - (traversedLength - (mWidth*2+mLength-diameter*3)));
//            }
//            else if (traversedLength > (mWidth+mLength-diameter*2)) // third side
//            {
//                point.set_x(xStart + (traversedLength - (mWidth+mLength-diameter*2)));
//                point.set_y(yEnd);
//            }
//            else if (traversedLength > (mWidth-diameter)) // second side
//            {
//                point.set_x(xStart);
//                point.set_y(yStart + traversedLength - mWidth-diameter);
//            }
//            else // first side
//            {
//                point.set_x(xEnd - traversedLength);
//                point.set_y(yStart);
//            }

//            traversedLength += width45;
//            perimeterPoints.push_back(point);
//        }
//    }
//    return perimeterPoints;
//    // add corner points!
//}

//std::vector<Point> Layer::get_90_degree_perimeter_points()
//{
//    std::vector<Point> perimeterPoints;
//    double modifiedEW = get_modified_extrusion_width();
//    double diameter = get_diameter_of_print();
//    double xStart{diameter/2};
//    double yStart{diameter/2};
//    double xEnd = xStart + mWidth-diameter;
//    double yEnd = yStart + mLength-diameter;


//    //    double traverseStart{0};
//    //    if (mNumber%2 == 0)
//    //    {
//    //        int numberOfPaths = get_number_of_paths();
//    //        traverseStart = (mWidth-modifiedEW*(numberOfPaths))/2;
//    //    }
//    //    else
//    //    {
//    //        int numberOfPaths = get_number_of_paths();
//    //        traverseStart = (mLength-modifiedEW*(numberOfPaths))/2;
//    //    }

//    double traversedLength = 0;
//    int numberOfPaths = get_number_of_paths();
//    int numberOfTurnPoints = numberOfPaths*2;
//    int mirrorLocation{1};
//    for (int i{0};i<numberOfTurnPoints; i++)
//    {
//        Point point(0,0,mLocation);
//        if (mNumber%2 == 0)
//        {
//            if (traversedLength > (mWidth-diameter)) // opposite side
//            {
//                Point mirrorPoint = perimeterPoints.at(i-mirrorLocation);
//                point.set_x(mirrorPoint.get_x());
////                point.set_x(xStart + mWidth*2 - traversedLength);
//                point.set_y(yEnd);
//                mirrorLocation += 2;
//            }
//            else // first side
//            {
//                point.set_x(xStart + traversedLength);
//                point.set_y(yStart);
//            }

//            traversedLength += modifiedEW;
//            perimeterPoints.push_back(point);
//        }
//        else
//        {
//            if (traversedLength > (mWidth-diameter)) // opposite side
//            {
//                point.set_x(xEnd);
//                Point mirrorPoint = perimeterPoints.at(i-mirrorLocation);
//                point.set_y(mirrorPoint.get_y());
//                mirrorLocation += 2;
////                point.set_y(yStart + mLength - traversedLength - diameter/2);
//            }
//            else // first side
//            {
//                point.set_x(xStart);
//                point.set_y(yStart + traversedLength);
//            }
//            traversedLength += modifiedEW;
//            perimeterPoints.push_back(point);
//        }
//    }
//    return perimeterPoints;
//}


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
    // re-create paths?
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

    //    double diameter = get_diameter_of_print();
    //    double infillWidth = mInfillWidth;
    //    double infillLength = mInfillLength;
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


    //    double xStart = diameter/2;

    int k{0};
    bool bEnd{false};
    while(bEnd == false)
    {
        //        double b{0};
        double yStart = cornerA.get_y();
        double xStart = cornerA.get_x();
        if (mNumber%2 ==0)
        {
            yStart = cornerD.get_y() - angleWidth/2 - angleWidth*k;
        }
        else
        {
            xStart = cornerA.get_y() + angleWidth/2 + angleWidth*k;
        }
        //        double yStart = b;
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
    //    if (mNumber%2 != 0)
    //    {
    //        theta += 90/180*pi;
    //    }

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
    //    double diameter = get_diameter_of_print();
    //    double width = get_width()-diameter;
    //    double length = get_length()-diameter;
    //    double zLocation = get_location();

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

    if (mNumber%2==0)
    {
        // for Even:
        perimeterPointList.push_back(cornerD); // first point is D
        int pointCount{0};
        int leftPoint{0};
        int bottomPoint{0};
        int topPoint{0};
        int rightPoint{0};
        //        int numberOfPoints = static_cast<int>(create_angled_ray_origin_list().size())*4;
        bool finish{false};
        while (finish == false)
        {
            // 1
            if (leftPoint < leftSize) // Left
            {
                perimeterPointList.push_back(leftSidePoints.at(leftPoint));
                leftPoint++;
            }
            else // Bottom
            {
                if (leftPoint > 0)
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
            // 2
            if (topPoint < topSize) // Top
            {
                perimeterPointList.push_back(topSidePoints.at(topPoint));
                topPoint++;
            }
            else // Right
            {
                if (topPoint > 0)
                {
                    if (topSize > 0)
                    {

                    }
                    else
                    {
                        if (rightPoint==0)
                        {
                            perimeterPointList.push_back(cornerC);
                        }
                    }
                }
                if (rightPoint < rightSize)
                {
                    perimeterPointList.push_back(rightSidePoints.at(rightPoint));
                    rightPoint++;
                }
            }
            // 3
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
            // 4
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
                        //                        perimeterPointList.push_back(cornerA);
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
                    //                    if ((perimeterPointList.at(i-1).get_x() - cornerB.get_x()) < 0.1)
                    //                    {
                    perimeterPointList.push_back(cornerB);
                    //                    }
                    //                    else
                    //                    {
                    //                        perimeterPointList.push_back(cornerA);
                    //                    }
                }
            }
        }
    }
    else
    {
        // for Odd:

        perimeterPointList.push_back(cornerA); // first point is A
        int pointCount{0};
        int bottomPoint{0};
        int rightPoint{0};
        int leftPoint{0};
        int topPoint{0};
        //        int numberOfPoints = static_cast<int>(create_angled_ray_origin_list().size())*4;
        bool finish{false};
        while (finish == false)
        {
            // 1
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
                    if (rightPoint==0)
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
            // 2
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
            // 3
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
            // 4
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
//                        perimeterPointList.push_back(cornerB);
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
                    //                    if ((perimeterPointList.at(i-1).get_x() == cornerC.get_x()) < 0.1)
                    //                    {
                    perimeterPointList.push_back(cornerC);
                    //                    }
                    //                    else
                    //                    {
                    //                        perimeterPointList.push_back(cornerD);
                    //                    }
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
    Point pointA(diameter/2,diameter/2,zLocation);
    Point pointB(infillWidth+diameter/2,diameter/2,zLocation);
    Point pointC(infillWidth+diameter/2,infillLength+diameter/2,zLocation);
    Point pointD(diameter/2,infillLength+diameter/2,zLocation);
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
    double diameter = get_diameter_of_print();
    mInfillLength = mLength-diameter;
    mInfillWidth = mWidth-diameter;
}
