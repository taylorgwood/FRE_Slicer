#include <iostream>
#include "gtest/gtest.h"
#include "point.h"
#include "path.h"
#include "layer.h"
#include "shape.h"
#include "gcode.h"

double const pi{3.14159265359};

void EXPECT_POINT_EQ(Point firstPoint, Point secondPoint)
{
    EXPECT_NEAR(firstPoint.get_x(),secondPoint.get_x(), 0.00001);
    EXPECT_NEAR(firstPoint.get_y(),secondPoint.get_y(), 0.00001);
    EXPECT_NEAR(firstPoint.get_z(),secondPoint.get_z(), 0.00001);
}

void EXPECT_POINT_EQ(Point* firstPoint, Point* secondPoint)
{
    EXPECT_NEAR(firstPoint->get_x(),secondPoint->get_x(), 0.00001);
    EXPECT_NEAR(firstPoint->get_y(),secondPoint->get_y(), 0.00001);
    EXPECT_NEAR(firstPoint->get_z(),secondPoint->get_z(), 0.00001);
}

void EXPECT_POINT_LIST_EQ(std::vector<Point*> firstVector, std::vector<Point> secondVector)
{
    size_t numberOfPoints = firstVector.size();
    for (int i{0}; i<numberOfPoints; i++)
    {
        EXPECT_POINT_EQ(*firstVector[i],secondVector[i]);
    }
}

void EXPECT_VECTOR_EQ(std::vector<double> firstVector, std::vector<double> secondVector)
{
    size_t vectorSize = firstVector.size();
    for (int i{0}; i<vectorSize; i++)
    {
        EXPECT_NEAR(firstVector[i],secondVector[i],0.00001);
    }
}

TEST(PointConstructor,givenNoValues_getZeroXYZ)
{
    Point point;
    double x_value = point.get_x();
    double y_value = point.get_y();
    double z_value = point.get_z();
    EXPECT_DOUBLE_EQ(x_value,0);
    EXPECT_DOUBLE_EQ(y_value,0);
    EXPECT_DOUBLE_EQ(z_value,0);
}

TEST(PointAddition,givenValues_getCorrectPoint)
{
    Point firstPoint(1,2,3,1);
    Point secondPoint(4,5,6,2);
    Point addedPoint = firstPoint + secondPoint;
    Point expectedPoint(5,7,9,1.5);
    EXPECT_POINT_EQ(addedPoint,expectedPoint);
}

TEST(PathConstructor,givenTwoPoints_getPath)
{
    Point start(1,2,3);
    Point end(2,4,6);
    double diameter{1};
    Path path(start,end,diameter);
    EXPECT_POINT_EQ(start,path.get_start());
    EXPECT_POINT_EQ(end,path.get_end());
    EXPECT_DOUBLE_EQ(diameter,path.get_diameter());
}

TEST(LayerExtrusionWidth,givenDefaultParameters_getExtrusionWidth)
{
    Layer layer;
    layer.set_auto_adjust_path(0);
    double calculatedExtrusionWidth = layer.get_modified_extrusion_width();
    double expectedExtrusionWidth = layer.get_extrusion_width();
    EXPECT_DOUBLE_EQ(calculatedExtrusionWidth,expectedExtrusionWidth);
}

TEST(LayerExtrusionWidth,givenChangedParameters_getCorrectExtrusionWidth)
{
    Layer layer;
    layer.set_infill_percentage(50);
    layer.set_auto_adjust_path(0);
    double calculatedExtrusionWidth = layer.get_modified_extrusion_width();
    double expectedExtrusionWidth = layer.get_extrusion_width()*2;
    EXPECT_DOUBLE_EQ(calculatedExtrusionWidth,expectedExtrusionWidth);
}

TEST(LayerVolume,givenLayerAreaAndHeight_getVolume)
{
    Layer layer;
    layer.set_width(10);
    layer.set_length(10);
    layer.set_height(0.26);
    double expectedVolume{26};
    double calculatedVolume = layer.get_volume();
    EXPECT_DOUBLE_EQ(expectedVolume,calculatedVolume);
}

TEST(ShapeFormation,givenShapeHeight_getNumberOfLayers)
{
    Shape shape;
    double initialLayerHeight{0.26};
    shape.set_layer_height(initialLayerHeight);
    shape.set_height(10);
    int expectedNumberOfLayers{38};
    EXPECT_EQ(shape.get_number_of_layers(),expectedNumberOfLayers);
}

TEST(ShapeFormation,givenShapeHeight_getCorrectNumberOfLayers)
{
    Shape shape;
    double initialLayerHeight{0.49};
    shape.set_layer_height(initialLayerHeight);
    shape.set_height(10);
    double adjustedLayerHeight = shape.get_adjusted_layer_height();
    shape.set_layer_height(adjustedLayerHeight);
    int adjustedNumberOfLayers{shape.get_number_of_layers()};
    int expectedNumberOfLayers{20};
    double expectedLayerHeight{0.5};
    EXPECT_EQ(adjustedNumberOfLayers,expectedNumberOfLayers);
    EXPECT_DOUBLE_EQ(shape.get_layer_height(),expectedLayerHeight);
}

TEST(PathNumber,givenLayerWidthAndPathWidth_getNumberOfPaths)
{
    Layer layer;
    int calculatedNumberOfPaths = layer.get_number_of_infill_paths();
    int expectedNumberOfPaths{38};
    EXPECT_EQ(calculatedNumberOfPaths,expectedNumberOfPaths);
}

TEST(PathNumber,givenLayerWidthAndPathWidth_getAdjustedNumberOfPaths)
{
    Layer layer;
    double adjustedExtrusionWidth = layer.get_adjusted_extrusion_width();
    layer.set_extrusion_width(adjustedExtrusionWidth);
    double expectedExtrusionWidth = 0.263157895;
    int expectedNumberOfPaths{38};
    EXPECT_NEAR(adjustedExtrusionWidth,expectedExtrusionWidth,0.00001);
    EXPECT_EQ(layer.get_number_of_infill_paths(),expectedNumberOfPaths);
}

TEST(LayerList,givenShape_getCorrectlySizedLayerList)
{
    Shape shape;
    shape.set_height(10);
    shape.set_layer_height(0.26);
    int numberOfLayers{38};
    std::vector<Layer*> layerList = shape.get_layer_list();
    size_t numberOfLayersInList = layerList.size();
    EXPECT_EQ(numberOfLayers,numberOfLayersInList);
}

TEST(LayerList,givenShape_getCorrectNumberOfPathsInFirstLayer)
{
    Shape shape;
    shape.set_height(10);
    shape.set_layer_height(0.26);
    std::vector<Layer*> layerList = shape.get_layer_list();
    Layer* firstLayer = layerList[0];
    int numberOfPaths = firstLayer->get_number_of_infill_paths();
    int expectedNumberOfPaths{38};
    EXPECT_EQ(numberOfPaths,expectedNumberOfPaths);
}

TEST(PathList,givenShape_getCorrectlySizedPathList)
{
    Shape shape;
    shape.set_height(10);
    shape.set_layer_height(0.26);
    std::vector<Layer*> layerList = shape.get_layer_list();
    Layer* firstLayer = layerList[0];
    std::vector<Path*> pathList = firstLayer->get_path_list();
    size_t numberOfPathsInList = pathList.size();
    int expectedNumberOfPaths{75};
    EXPECT_EQ(numberOfPathsInList,expectedNumberOfPaths);
}

TEST(PathList,givenShape_getCorrectNumberOfPointsInSecondPath)
{
    Shape shape;
    shape.set_height(10);
    shape.set_layer_height(0.26);
    std::vector<Layer*> layerList = shape.get_layer_list();
    Layer* firstLayer = layerList[0];
    std::vector<Path*> pathList = firstLayer->get_path_list();
    Path* secondPath = pathList[0];
    int numberOfPoints = secondPath->get_number_of_points();
    int expectedNumberOfPoints{10};
    EXPECT_EQ(numberOfPoints,expectedNumberOfPoints);
}

TEST(PathList,givenShape_getCorrectNumberOfPointsInSecondPathList)
{
    Shape shape;
    shape.set_height(10);
    shape.set_layer_height(0.26);
    std::vector<Layer*> layerList = shape.get_layer_list();
    Layer* firstLayer = layerList[0];
    std::vector<Path*> pathList = firstLayer->get_path_list();
    Path* firstPath = pathList[0];
    std::vector<Point*> pointList = firstPath->get_point_list();
    size_t numberOfPoints = pointList.size();
    int expectedNumberOfPoints{10};
    EXPECT_EQ(numberOfPoints,expectedNumberOfPoints);
}

TEST(LayerList,whenAskedForPointsInFirstLayer_getCorrectNumberOfPointsInFirstLayer)
{
    Shape shape;
    int firstLayer{0};
    std::vector<Point> pointList = shape.get_points_in_layer(firstLayer);
    size_t numberOfPointsInLayer = pointList.size();
    int expectedNumberOfPointsInLayer{(38*10)+(38-2)/2};
    EXPECT_EQ(numberOfPointsInLayer,expectedNumberOfPointsInLayer);
}

TEST(LayerList,whenAskedForPointsInSecondLayer_getCorrectNumberOfPointsInSecondLayer)
{
    Shape shape;
    int secondLayer{1};
    std::vector<Point> pointList = shape.get_points_in_layer(secondLayer);
    size_t numberOfPointsInLayer = pointList.size();
    int expectedNumberOfPointsInLayer{(38*10)+(38-2)/2};
    EXPECT_EQ(numberOfPointsInLayer,expectedNumberOfPointsInLayer);
}

TEST(ShapeList,whenAskedForPointsInShape_getCorrectNumberOfPointsInShape)
{
    Shape shape;
    std::vector<Point> pointList = shape.get_points();
    size_t numberOfPoints = pointList.size();
    int expectedNumberOfPoints{(38*((38*10)+(38-2)/2))};
    EXPECT_EQ(numberOfPoints,expectedNumberOfPoints);
}

TEST(PrintOut,whenGivenPoint_printPointToConsole)
{
    Point point{1,2,3};
//        point.print();
}

TEST(PrintOut,whenGivenPointList_printPointsToConsole)
{
    Point firstPoint{1,2,3};
    Point secondPoint{4,5,6};
    Point thirdPoint{7,8,9};
    std::vector<Point> pointList = {firstPoint,secondPoint,thirdPoint};
    //    firstPoint.print_list(pointList);
}

TEST(LayerLocations,whenAskedForLayerLocations_getLayerLocations)
{
    Shape shape;
    int firstLayer{0};
    std::vector<double> layerLocationVector = shape.get_layer_locations();
    size_t vectorLength = layerLocationVector.size();
    double layerHeight{0.26315789473};
    double height{0};
    std::vector<double> expectedLayerHeights;
    for (int i{0}; i<vectorLength; i++)
    {
        height += layerHeight;
        expectedLayerHeights.push_back(height);
    }
    EXPECT_VECTOR_EQ(layerLocationVector,expectedLayerHeights);
}

TEST(LayerLocations,whenConstructingShape_layerLocationsSet)
{
    Shape shape;
    std::vector<Layer*> layerList = shape.get_layer_list();
    std::vector<double> layerLocationVector;
    size_t vectorLength = layerLocationVector.size();
    double layerHeight{0.26};
    double expectedLocation{0};
    std::vector<double> expectedLayerHeights;
    for (int i{0}; i<vectorLength; i++)
    {
        expectedLocation += layerHeight;
        expectedLayerHeights.push_back(expectedLocation);
        Layer* layer = layerList[i];
        double location = layer->get_location();
        layerLocationVector.push_back(location);
    }
    EXPECT_VECTOR_EQ(layerLocationVector,expectedLayerHeights);
}

TEST(LayerNumber,whenConstructingShape_layerNumbersSet)
{
    Shape shape;
    std::vector<Layer*> layerList = shape.get_layer_list();
    std::vector<double> layerNumberVector;
    size_t vectorLength = layerNumberVector.size();
    std::vector<double> expectedLayerNumbers;
    for (int i{0}; i<vectorLength; i++)
    {
        expectedLayerNumbers.push_back(i);
        Layer* layer = layerList[i];
        double number = layer->get_number();
        layerNumberVector.push_back(number);
    }
    EXPECT_VECTOR_EQ(layerNumberVector,expectedLayerNumbers);
}

TEST(PointLocations,whenConstructingShape_pointLocationsConstructed)
{
//        Shape shape;
//        std::vector<Layer*> layerList = shape.get_layer_list();
//        Layer* firstLayer = layerList[0];
//        std::vector<Path*> pathList = firstLayer->get_path_list();
//        Path* secondPath = pathList[1];
//        std::vector<Point*> pointList = secondPath->get_point_list();
//        std::vector<Point> expectedPointList;
//        double modifiedExtrusionWidth = shape.get_layer(0)->get_modified_extrusion_width();
//        double layerHeight = shape.get_layer_height();
//        double diameter = secondPath->get_diameter();
//        double pointLength = (10-modifiedExtrusionWidth)/((secondPath->get_number_of_points())-1);
//        for (int i{0}; i<11; i++)
//        {
//            double x = 10-(modifiedExtrusionWidth/2)-i*pointLength;
//            double y = modifiedExtrusionWidth*3/2;
//            double z = layerHeight/2;
//            Point point;
//            point.set_x(x);
//            point.set_y(y);
//            point.set_z(z);
//            expectedPointList.push_back(point);
//        }
//        EXPECT_POINT_LIST_EQ(pointList,expectedPointList);
}

TEST(PointMath,whenAskedForNormalizedPoint_getNormalizedPoint)
{
    Point point{1,2,3,0.5};
    Point normalizedPoint = point.normalize();
    Point expectedPoint{1/sqrt(14),2/sqrt(14),3/sqrt(14),0.5};
    EXPECT_POINT_EQ(normalizedPoint,expectedPoint);
}

TEST(PointLocations,whenConstructingShape_pointLocationsCorrect)
{
    //    Shape shape;
    //    std::vector<Layer*> layerList = shape.get_layer_list();
    //    Layer* firstLayer = layerList[0];
    //    std::vector<Path*> pathList = firstLayer->get_path_list();
    //    Path* thirdPath = pathList[2];
    //    std::vector<Point*> pointList = thirdPath->get_point_list();
    //    std::vector<Point> expectedPointList;
    //    double modifiedExtrusionWidth = shape.get_layer(0)->get_modified_extrusion_width();
    //    double layerHeight = shape.get_layer_height();
    //    double diameter = thirdPath->get_diameter();
    //    double pointLength = (10-diameter)/((thirdPath->get_number_of_points())-1);
    //    for (int i{0}; i<11; i++)
    //    {
    //        double x = (diameter/2) + i*pointLength;
    //        double y = modifiedExtrusionWidth*5/2;
    //        double z = layerHeight/2;
    //        Point point;
    //        point.set_x(x);
    //        point.set_y(y);
    //        point.set_z(z);
    //        expectedPointList.push_back(point);
    //    }
    //    EXPECT_POINT_LIST_EQ(pointList,expectedPointList);
}

TEST(PointLocations,whenConstructingShape_pointLocationsFollowSwitchbackPattern)
{
    //    Shape shape;
    //    std::vector<Layer*> layerList = shape.get_layer_list();
    //    Layer* firstLayer = layerList[0];
    //    std::vector<Path*> pathList = firstLayer->get_path_list();
    //    Path* thirdPath = pathList[2];
    //    std::vector<Point*> pointList3 = thirdPath->get_point_list();
    //    Path* fourthPath = pathList[3];
    //    std::vector<Point*> pointList4 = fourthPath->get_point_list();
    //    std::vector<Point> expectedThirdPointList;
    //    std::vector<Point> expectedFourthPointList;
    //    double modifiedExtrusionWidth = shape.get_layer(0)->get_modified_extrusion_width();
    //    double layerHeight = shape.get_layer_height();
    //    double diameter = thirdPath->get_diameter();
    //    double pointLength = (10-diameter)/((fourthPath->get_number_of_points())-1);
    //    for (int i{0}; i<11; i++)
    //    {
    //        double x3 = (diameter/2) + i*pointLength;
    //        double x4 = (10-diameter/2) - i*pointLength;
    //        double y3 = (modifiedExtrusionWidth*5/2);
    //        double y4 = (modifiedExtrusionWidth*7/2);
    //        double z = layerHeight/2;
    //        Point point3;
    //        Point point4;
    //        point3.set_x(x3);
    //        point3.set_y(y3);
    //        point3.set_z(z);
    //        point4.set_x(x4);
    //        point4.set_y(y4);
    //        point4.set_z(z);
    //        expectedThirdPointList.push_back(point3);
    //        expectedFourthPointList.push_back(point4);
    //    }
    //    EXPECT_POINT_LIST_EQ(pointList3,expectedThirdPointList);
    //    EXPECT_POINT_LIST_EQ(pointList4,expectedFourthPointList);
}

TEST(Gcode,whenAskedToCreateEmptyFile_newEmptyFileCreated)
{
//    Gcode gcode;
//    gcode.create_empty_file();
//    std::string suffix = ".txt";
//    std::string fileName = gcode.get_file_name();
//    EXPECT_TRUE(gcode.does_file_exist(fileName + suffix));
//    gcode.delete_file();
}

TEST(Gcode,whenAskedToCreateFileWithName_namedFileCreated)
{
//    Gcode gcode;
//    Shape newShape;
//    std::string fileName = "testFileName";
//    gcode.generate_file(newShape,fileName);
//    std::string suffix = ".txt";
//    EXPECT_TRUE(gcode.does_file_exist(fileName + suffix));
//    gcode.delete_file();
}

TEST(Gcode,whenAskedToDeleteFile_fileDeleted)
{
//    Gcode gcode;
//    Shape shape;
//    std::string fileName = "deleteThisFile";
//    gcode.generate_file(shape, fileName);
//    std::string updatedFileName = gcode.get_file_name();
//    std::string suffix = ".txt";
//    EXPECT_TRUE(gcode.does_file_exist(fileName + suffix));
//    gcode.delete_file();
//    EXPECT_FALSE(gcode.does_file_exist(updatedFileName + suffix));
}

TEST(Gcode,whenAskedToGenerateGCodeFile_LayersPrintedInFile)
{
//    Gcode newGcode;
//    Shape newShape;
//    std::string fileName = "testForLayers";
//    newGcode.generate_file(newShape, fileName);
//    // read some line from file
//    newGcode.delete_file();
}

TEST(Gcode,whenAskedToGenerateGCodeFile_PointsPrintedInFile)
{
    Gcode newGcode;
    Shape newShape;
    std::string fileName = "testForPoints";
    newGcode.generate_file(newShape, fileName);
    // read some lines from file
//    newGcode.delete_file();
}

TEST(Gcode,whenAskedForInitialExtrusionAmount_getZeroExtrusionAmount)
{
    Gcode newGcode;
    Shape newShape;
    double displacementA = newGcode.get_extruder_displacement()[0];
    double displacementB = newGcode.get_extruder_displacement()[1];
    double totalDisplacement = displacementA + displacementB;
    double expectedDisplacement = 0;
    EXPECT_DOUBLE_EQ(totalDisplacement,expectedDisplacement);
}

//TEST(Extrusion,whenAskedForShapeExtrusionAmount_getCorrectShapeExtrusionAmount)
//{
//    Gcode newGcode;
//    Shape newShape;
//    std::string fileName = "deleteThisFile";
//    newGcode.generate_file(newShape,fileName);
//    newGcode.delete_file();
//    double displacementA = newGcode.get_extruder_displacement()[0];
//    double displacementB = newGcode.get_extruder_displacement()[1];
//    double totalDisplacement = displacementA + displacementB;
//    double syringeDiameter = newGcode.get_syringe_diameter();
//    double pi = 3.14159265359;
//    double printCrossSectionalArea = pi*(syringeDiameter*syringeDiameter)/4;
//    double printVolume = 10*10*10;
//    double expectedDisplacement = printVolume/printCrossSectionalArea;
//    EXPECT_DOUBLE_EQ(totalDisplacement,expectedDisplacement);
//}

TEST(Extrusion,whenCreatingShape_getCorrectLayerVolume)
{
    Shape newShape;
    Layer* firstLayer = newShape.get_layer(0);
    double extrusionWidth = firstLayer->get_extrusion_width();
    double infillPercentage = firstLayer->get_infill_percentage();
    double extrusionMultiplier = firstLayer->get_extrusion_multiplier();
    double layerHeight = newShape.get_layer_height();
    double print_diameter = firstLayer->get_diameter_of_print();
    double layerVolume = extrusionMultiplier*extrusionWidth*infillPercentage/100;
    int numberOfLayers = newShape.get_number_of_layers();
}

TEST(ShapeSize,whenResettingLayerSize_getNewShapeSize)
{
    Shape shape;
    double newShapeWidth = 8;
    shape.set_width(newShapeWidth);
    double actualShapeWidth = shape.get_width();
    EXPECT_DOUBLE_EQ(newShapeWidth,actualShapeWidth);
}

TEST(ShapeSize,whenResettingLayerSize_getNewLayerSize)
{
    Shape shape;
    double newShapeLength = 5;
    shape.set_length(newShapeLength);
    int firstLayer{0};
    double actualLayerLength = shape.get_layer(firstLayer)->get_length();
    EXPECT_DOUBLE_EQ(newShapeLength,actualLayerLength);
}

TEST(PathNumber,whenChangingShapeSize_getDifferentNumberOfPathsDependingOnOrientation)
{
    double shapeHeight{10};
    double shapeWidth{5};
    double shapeLength{10};
    Shape shape(shapeHeight,shapeWidth,shapeLength);
    Layer* firstLayer = shape.get_layer(0);
    int numberOfPathsInFirstLayer = firstLayer->get_number_of_infill_paths();
    Layer* secondLayer = shape.get_layer(1);
    int numberOfPathsInSecondLayer = secondLayer->get_number_of_infill_paths();
    EXPECT_EQ(numberOfPathsInFirstLayer,19);
    EXPECT_EQ(numberOfPathsInSecondLayer,38);
}

TEST(PointNumber,whenChangingShapeSize_getDifferentNumberOfPointsDependingOnPathOrientation)
{
    double shapeHeight{10};
    double shapeWidth{5};
    double shapeLength{10};
    Shape shape(shapeHeight,shapeWidth,shapeLength);
    Layer* firstLayer = shape.get_layer(0);
    int numberOfPointsInFirstLayerPath = firstLayer->get_path(0)->get_number_of_points();
    Layer* secondLayer = shape.get_layer(1);
    int numberOfPointsInSecondLayerPath = secondLayer->get_path(0)->get_number_of_points();
    EXPECT_EQ(numberOfPointsInFirstLayerPath,shapeWidth);
    EXPECT_EQ(numberOfPointsInSecondLayerPath,shapeLength);
}

TEST(Infill,whenChangingInfill_getCorrectlySizedLayer)
{
    Shape shape;
    double newInfillPercentage{50};
    shape.set_infill_percentage(newInfillPercentage);
    Layer* firstLayer = shape.get_layer(0);
    double layerWidth = firstLayer->get_width();
    double layerLength = firstLayer->get_length();
    double expectedLayerWidth{10};
    double expectedLayerLength{10};
    EXPECT_DOUBLE_EQ(layerWidth, expectedLayerWidth);
    EXPECT_DOUBLE_EQ(layerLength,expectedLayerLength);
}

TEST(PathList,whenRequestingPathList_getCorrectlySizedPathList)
{
    Shape shape;
    std::vector<Path>* pathList = shape.get_path_list();
    size_t numberOfPaths = pathList->size();
    int numberOfLayers = shape.get_number_of_layers();
    std::vector <Path*> layerPathList = shape.get_layer(0)->get_path_list();
    int numberOfPathsInFirstLayer = static_cast<int>(layerPathList.size());
    int expectedNumberOfPaths = numberOfLayers*numberOfPathsInFirstLayer;
    EXPECT_EQ(numberOfPaths,expectedNumberOfPaths);
}

TEST(PathList,whenRequestingPathList_getCorrectLengthPathsInPathList)
{
    Shape shape;
    std::vector<Path>* pathList = shape.get_path_list();
    Path firstPath = pathList->at(0);
    double firstPathLength = firstPath.get_length();
    double firstLayerExtrusionWidth = shape.get_layer(0)->get_modified_extrusion_width();
    double expectedFirstPathLength = shape.get_length()-firstLayerExtrusionWidth;
    int numberOfPathsInFirstLayer = 75;
    EXPECT_EQ(firstPathLength,expectedFirstPathLength);
    Path secondLayerPath = pathList->at(numberOfPathsInFirstLayer);
    double secondLayerPathLength = secondLayerPath.get_length();
    double secondLayerExtrusionWidth = shape.get_layer(0)->get_modified_extrusion_width();
    double expectedSecondLayerPathLength = shape.get_width()-secondLayerExtrusionWidth;
    EXPECT_EQ(secondLayerPathLength,expectedSecondLayerPathLength);
}

TEST(LayerHeight,whenChangingLayerHeight_getCorrectPathDiameter)
{
    Shape shape;
    double newLayerHeight{0.5};
    shape.reset_layer_height(newLayerHeight);
    Layer* firstLayer = shape.get_layer(0);
    double pathDiameter = firstLayer->get_diameter_of_print();
    double expectedPathDiameter = 0.409306143423;
    EXPECT_NEAR(pathDiameter, expectedPathDiameter,0.00001);
}

TEST(PathList,whenRequestingPathList_getCorrectFirstPathLocation)
{
    Shape shape;
    std::vector<Path>* pathList = shape.get_path_list();
    Path firstPath = pathList->at(0);
    Point firstPathLocation = firstPath.get_start();
    double firstLayerLocation = shape.get_layer(0)->get_location();
    double modifiedExtrusionWidth = shape.get_layer(0)->get_modified_extrusion_width();
    double diameter = firstPath.get_diameter();
    std::vector <Point> corners = shape.get_layer(0)->get_corners();
    Point expectedFirstLocation = corners.at(3);
    EXPECT_POINT_EQ(firstPathLocation,expectedFirstLocation);
}

TEST(PathList,whenRequestingPathList_getCorrectLastPathLocation)
{
    Shape shape;
    Layer* firstLayer = shape.get_layer(0);
    std::vector<Path*> pathList = firstLayer->get_path_list();
    int lastPathNumber = static_cast<int>(pathList.size())-1;
    Path* lastPath = pathList.at(lastPathNumber);
    Point lastPathLocation = lastPath->get_start();
    double firstLayerZLocation = firstLayer->get_location();
    double expectedXLocation = shape.get_layer(0)->get_corners().at(1).get_x();
    double expectedYLocation = shape.get_layer(0)->get_corners().at(1).get_y();
    Point expectedLocation{expectedXLocation,expectedYLocation,firstLayerZLocation};
    EXPECT_POINT_EQ(lastPathLocation,expectedLocation);
}


TEST(Infill,whenChangingInfill_getCorrectNumberOfPaths)
{
    Shape shape;
    double newInfillPercentage{100};
    shape.set_infill_percentage(newInfillPercentage);
    Layer* firstLayer = shape.get_layer(0);
    int numberOfPathsInList = firstLayer->get_number_of_infill_paths();
    double modifiedExtrusionWidth = firstLayer->get_modified_extrusion_width();
    int expectedNumberOfPaths = firstLayer->get_width()/modifiedExtrusionWidth;
    EXPECT_EQ(numberOfPathsInList,expectedNumberOfPaths);
}

TEST(Infill,whenRequestingPathListWithChangedInfill_getCorrectLastPathLocation)
{
    //    Shape shape;
    //    shape.set_infill_percentage(50);
    //    Layer* firstLayer = shape.get_layer(0);
    //    std::vector<Path*> pathList = firstLayer->get_path_list();
    //    int lastPathNumber = static_cast<int>(pathList.size())-1;
    //    Path* lastPath = pathList.at(lastPathNumber);
    //    Point lastPathLocation = lastPath->get_start();
    //    double firstLayerZLocation = firstLayer->get_location();
    //    Point expectedLocation{0.14847094303887,4.8684210526,firstLayerZLocation};
    //    EXPECT_POINT_EQ(lastPathLocation,expectedLocation);
}

TEST(DotProduct,whenGivenTwoPoints_getDotProduct)
{
    Point firstPoint(1,2,3);
    Point secondPoint(5,4,3);
    double dotProduct = firstPoint.dot(secondPoint);
    double expectedDotProduct = 1*5+2*4+3*3;
    EXPECT_DOUBLE_EQ(expectedDotProduct,dotProduct);
}

TEST(CrossProduct,whenGivenTwoPointsAsVectors_getCrossProduct)
{
    Point firstPoint(1,2,3);
    Point secondPoint(5,4,3);
    Point crossProduct = firstPoint.cross(secondPoint);
    Point expectedCrossProduct(-6,12,-6);
    EXPECT_POINT_EQ(expectedCrossProduct,crossProduct);
}

TEST(IntersectPoint,whenGivenCrossingRayAndLine_getIntersectionPoint)
{
    Point start(10,0,0);
    Point end(10,10,0);
    Point origin(0,5,0);
    double theta = 0;
    Layer layer;
    Point intersectPoint = layer.get_intersection(start,end,origin,theta);
    Point expectedIntersection(10,5,0);
    EXPECT_POINT_EQ(intersectPoint,expectedIntersection);
}

TEST(IntersectPoint,whenGivenNonCrossingRay_getNoIntersectionPoint)
{
    Point start(10,0,0);
    Point end(10,10,0);
    Point origin(0,5,0);
    double theta = pi/4;
    Layer layer;
    Point intersectPoint = layer.get_intersection(start,end,origin,theta);
    Point noIntersection(-1,-1,-1);
    EXPECT_POINT_EQ(intersectPoint,noIntersection);
}

TEST(IntersectPoint,whenGivenRayAtStart_getIntersectionPoint)
{
    Point start(0,0,0);
    Point end(0,10,0);
    Point origin(0,0,0);
    double theta = pi/4;
    Layer layer;
    Point intersectPoint = layer.get_intersection(start,end,origin,theta);
    Point expectedIntersection(0,0,0);
    EXPECT_POINT_EQ(intersectPoint,expectedIntersection);
}

TEST(IntersectPoint,whenGivenRayThatCrossesLineWhenExtended_getIntersectionPoint)
{
    Point start(0,0,1);
    Point end(10,0,1);
    Point origin(0,-2,1);
    double theta = pi/4;
    Layer layer;
    Point intersectPoint = layer.get_intersection(start,end,origin,theta);
    Point expectedIntersection(2,0,1);
    EXPECT_POINT_EQ(intersectPoint,expectedIntersection);
}

TEST(IntersectPoint,whenGivenRayThatCrossLineWhenExtendedBackwards_getIntersectionPoint)
{
    Point start(0,0,0);
    Point end(10,0,0);
    Point origin(2,1,0);
    double theta = pi/4;
    Layer layer;
    Point intersectPoint = layer.get_intersection(start,end,origin,theta);
    Point expectedIntersection(1,0,0);
    EXPECT_POINT_EQ(intersectPoint,expectedIntersection);
}

TEST(IntersectPoint,whenGivenRayAndReversedLine_getIntersectionPoint)
{
    Point start(10,0,0);
    Point end(0,0,0);
    Point origin(2,1,0);
    double theta = pi/4;
    Layer layer;
    Point intersectPoint = layer.get_intersection(start,end,origin,theta);
    Point expectedIntersection(1,0,0);
    EXPECT_POINT_EQ(intersectPoint,expectedIntersection);
}

TEST(AngledPaths,whenGivenThetaOf45Degrees_getCorrectNumberOfPaths)
{
    Layer layer;
    layer.set_width(4);
    layer.set_length(4);
    layer.set_infill_angle(45);
    std::vector <Point> angledRayOriginList = layer.create_angled_ray_origin_list();
    int numberOfPaths = angledRayOriginList.size();
    int expectedNumberOfPaths{20};
    EXPECT_EQ(numberOfPaths,expectedNumberOfPaths);
}

TEST(SetAngle,givenAngleGreaterThan45_getAdjustedAngle)
{
    Layer layer;
    double largeAngle{182};
    double expectedAngle = largeAngle-90*2;
    layer.set_infill_angle(largeAngle);
    double adjustedAngle = layer.get_infill_angle();
    EXPECT_DOUBLE_EQ(adjustedAngle,expectedAngle);
}

TEST(SetAngle,givenAngleLessThanZero_getAdjustedAngle)
{
    Layer layer;
    double negativeAngle{-247};
    double expectedAngle = negativeAngle+90*3;
    layer.set_infill_angle(negativeAngle);
    double adjustedAngle = layer.get_infill_angle();
    EXPECT_DOUBLE_EQ(adjustedAngle,expectedAngle);
}

TEST(LayerSize,givenTopSize_getSameBottomLayer)
{
    Shape shape;
    double newTopWidth{4};
    shape.set_top_width(newTopWidth);
    Layer* firstLayer = shape.get_layer(0);
    double firstLayerWidth = firstLayer->get_width();
    EXPECT_DOUBLE_EQ(firstLayerWidth,shape.get_width());
}

TEST(LayerSize,givenTopSize_getCorrectlySizedTopLayer)
{
    Shape shape;
    double newTopWidth{4};
    shape.set_top_width(newTopWidth);
    int numberOfLayers = shape.get_number_of_layers();
    Layer* topLayer = shape.get_layer(numberOfLayers-1);
    double topLayerWidth = topLayer->get_width();
    double extrusionWidth = topLayer->get_modified_extrusion_width();
    double expectedTopLayerWidth = shape.get_top_width(); //+extrusionWidth/2;
    EXPECT_DOUBLE_EQ(topLayerWidth,expectedTopLayerWidth);
//    Gcode gcode;
//    gcode.generate_file(shape,"trapezoid_test");
}

TEST(InsideTrapezoid,givenPointInsideTrapezoidXYZ_getTrue)
{
    Point point(1,0,1);
    Point cornerA(0,0,0);
    Point cornerB(2,0,0);
    Point cornerC(2,0,2);
    Point cornerD(0,0,2);
    std::vector <Point> trapezoid{4};
    trapezoid.at(0) = cornerA;
    trapezoid.at(1) = cornerB;
    trapezoid.at(2) = cornerC;
    trapezoid.at(3) = cornerD;
    bool isInside = point.is_inside_XZtrapezoid(trapezoid);
    EXPECT_TRUE(isInside);
}

TEST(InsideTrapezoid,givenPointInsideTrapezoidXZ_getTrue)
{
    Point point(1,1,1);
    Point cornerA(0,0,0);
    Point cornerB(2,0,0);
    Point cornerC(2,0,2);
    Point cornerD(0,0,2);
    std::vector <Point> trapezoid{4};
    trapezoid.at(0) = cornerA;
    trapezoid.at(1) = cornerB;
    trapezoid.at(2) = cornerC;
    trapezoid.at(3) = cornerD;
    bool isInside = point.is_inside_XZtrapezoid(trapezoid);
    EXPECT_TRUE(isInside);
}

TEST(InsideTrapezoid,givenPointOutsideTrapezoidXY_getFalse)
{
    Point point(3,0,1);
    Point cornerA(0,0,0);
    Point cornerB(2,0,0);
    Point cornerC(2,0,2);
    Point cornerD(0,0,2);
    std::vector <Point> trapezoid{4};
    trapezoid.at(0) = cornerA;
    trapezoid.at(1) = cornerB;
    trapezoid.at(2) = cornerC;
    trapezoid.at(3) = cornerD;
    bool isInside = point.is_inside_XZtrapezoid(trapezoid);
    EXPECT_FALSE(isInside);
}

TEST(A,B_c)
{
    Gcode gcode;
    Shape shape;
    shape.set_width(10.75);
    shape.set_length(14);
    shape.set_top_width(3);
    double infillAngle{45};
    shape.set_infill_angle(infillAngle);
    gcode.generate_file(shape,"InfillTest45Degrees");

}

TEST(PrintOut,PrintOutPerimeterPoints)
{
//        Layer layer;
//        layer.set_number(0);
//    //    layer.set_width(10);
//        layer.set_infill_angle(0);
//        layer.set_infill_percentage(100);
//        std::vector <Point> perimeterPoints = layer.get_perimeter_points();
//        EXPECT_EQ(1,1);
//        int numberOfPoints = static_cast<int>(perimeterPoints.size());

//            for (int i{0}; i<numberOfPoints; i++)
//            {
//                Point point = perimeterPoints.at(i);
//                std::cout << i << " X " << point.get_x() << std::endl;
//                std::cout << "  Y " << point.get_y() << std::endl;
//                std::cout << std::endl;
//            }

//            std::cout << " X " << std::endl;
//            std::cout << std::endl;
//            for (int i{0}; i<numberOfPoints; i++)
//            {
//                Point point = perimeterPoints.at(i);
//                std::cout << point.get_x() << std::endl;
//            }
//            std::cout << std::endl;
//            std::cout << std::endl;
//            std::cout << " Y " << std::endl;
//            std::cout << std::endl;

//            for (int i{0}; i<numberOfPoints; i++)
//            {
//                Point point = perimeterPoints.at(i);
//                std::cout << point.get_y() << std::endl;
//            }
}


