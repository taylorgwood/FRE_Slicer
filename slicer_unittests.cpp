#include <iostream>
#include "gtest/gtest.h"
#include "point.h"
#include "path.h"
#include "layer.h"
#include "shape.h"
#include "gcode.h"

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
        EXPECT_EQ(firstVector[i],secondVector[i]);
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
    double calculatedExtrusionWidth = layer.get_modified_extrusion_width();
    double expectedExtrusionWidth = layer.get_extrusion_width();
    EXPECT_DOUBLE_EQ(calculatedExtrusionWidth,expectedExtrusionWidth);
}

TEST(LayerExtrusionWidth,givenChangedParameters_getCorrectExtrusionWidth)
{
    Layer layer;
    layer.set_infill_percentage(50);
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

TEST(ShapeFormation,givenShapeHeihgt_getAdjustedNumberOfLayers)
{
    Shape shape;
    double initialLayerHeight{0.49};
    shape.set_layer_height(initialLayerHeight);
    shape.set_height(10);
    shape.adjust_layer_height();
    int adjustedNumberOfLayers{shape.get_number_of_layers()};
    int expectedNumberOfLayers{20};
    double expectedLayerHeight{0.5};
    EXPECT_EQ(adjustedNumberOfLayers,expectedNumberOfLayers);
    EXPECT_DOUBLE_EQ(shape.get_layer_height(),expectedLayerHeight);
}

TEST(PathNumber,givenLayerWidthAndPathWidth_getNumberOfPaths)
{
    Layer layer;
    int calculatedNumberOfPaths = layer.get_number_of_paths();
    int expectedNumberOfPaths{38};
    EXPECT_EQ(calculatedNumberOfPaths,expectedNumberOfPaths);
}

TEST(PathNumber,givenLayerWidthAndPathWidth_getAdjustedNumberOfPaths)
{
    Layer layer;
    layer.adjust_extrusion_width();
    double adjustedExtrusionWidth{layer.get_extrusion_width()};
    double expectedExtrusionWidth{0.26315789473};
    EXPECT_NEAR(adjustedExtrusionWidth,expectedExtrusionWidth,0.00001);
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
    int numberOfPaths = firstLayer->get_number_of_paths();
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
    int expectedNumberOfPaths{38};
    EXPECT_EQ(numberOfPathsInList,expectedNumberOfPaths);
}

TEST(PathList,givenShape_getCorrectNumberOfPointsInFirstPath)
{
    Shape shape;
    shape.set_height(10);
    shape.set_layer_height(0.26);
    std::vector<Layer*> layerList = shape.get_layer_list();
    Layer* firstLayer = layerList[0];
    std::vector<Path*> pathList = firstLayer->get_path_list();
    Path* firstPath = pathList[0];
    int numberOfPoints = firstPath->get_number_of_points();
    int expectedNumberOfPoints{11};
    EXPECT_EQ(numberOfPoints,expectedNumberOfPoints);
}

TEST(PathList,givenShape_getCorrectNumberOfPointsInFirstPathList)
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
    int expectedNumberOfPoints{11};
    EXPECT_EQ(numberOfPoints,expectedNumberOfPoints);
}

TEST(LayerList,whenAskedForPointsInLayer_getCorrectNumberOfPointsInLayer)
{
    Shape shape;
    int firstLayer{0};
    std::vector<Point> pointList = shape.get_points_in_layer(firstLayer);
    size_t numberOfPointsInLayer = pointList.size();
    int expectedNumberOfPointsInLayer{(38*11)};
    EXPECT_EQ(numberOfPointsInLayer,expectedNumberOfPointsInLayer);
}

TEST(ShapeList,whenAskedForPointsInShape_getCorrectNumberOfPointsInShape)
{
    Shape shape;
    std::vector<Point> pointList = shape.get_points();
    size_t numberOfPoints = pointList.size();
    int expectedNumberOfPoints{(38*38*11)};
    EXPECT_EQ(numberOfPoints,expectedNumberOfPoints);
}

TEST(PrintOut,whenGivenPoint_printPointToConsole)
{
    Point point{1,2,3};
    //    point.print();
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
    double layerHeight{0.26};
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
    Shape shape;
    std::vector<Layer*> layerList = shape.get_layer_list();
    Layer* firstLayer = layerList[0];
    std::vector<Path*> pathList = firstLayer->get_path_list();
    Path* secondPath = pathList[1];
    std::vector<Point*> pointList = secondPath->get_point_list();
    std::vector<Point> expectedPointList;
    for (int i{0}; i<11; i++)
    {
        double x = 10-i;
        double y{0.263158};
        double z{0.263158};
        Point point;
        point.set_x(x);
        point.set_y(y);
        point.set_z(z);
        expectedPointList.push_back(point);
    }
    EXPECT_POINT_LIST_EQ(pointList,expectedPointList);
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
    Shape shape;
    std::vector<Layer*> layerList = shape.get_layer_list();
    Layer* firstLayer = layerList[0];
    std::vector<Path*> pathList = firstLayer->get_path_list();
    Path* thirdPath = pathList[2];
    std::vector<Point*> pointList = thirdPath->get_point_list();
    std::vector<Point> expectedPointList;
    for (int i{0}; i<11; i++)
    {
        double x = i;
        double y{0.526316};
        double z{0.263158};
        Point point;
        point.set_x(x);
        point.set_y(y);
        point.set_z(z);
        expectedPointList.push_back(point);
    }
    EXPECT_POINT_LIST_EQ(pointList,expectedPointList);
}

TEST(PointLocations,whenConstructingShape_pointLocationsFollowSwitchbackPattern)
{
    Shape shape;
    std::vector<Layer*> layerList = shape.get_layer_list();
    Layer* firstLayer = layerList[0];
    std::vector<Path*> pathList = firstLayer->get_path_list();
    Path* thirdPath = pathList[2];
    std::vector<Point*> pointList3 = thirdPath->get_point_list();
    Path* fourthPath = pathList[3];
    std::vector<Point*> pointList4 = fourthPath->get_point_list();
    std::vector<Point> expectedThirdPointList;
    std::vector<Point> expectedFourthPointList;
    for (int i{0}; i<11; i++)
    {
        double x3 = i;
        double x4 = (10-i);
        double y3 = (0.263158*2);
        double y4 = (0.263158*3);
        double z{0.263158};
        Point point3;
        Point point4;
        point3.set_x(x3);
        point3.set_y(y3);
        point3.set_z(z);
        point4.set_x(x4);
        point4.set_y(y4);
        point4.set_z(z);
        expectedThirdPointList.push_back(point3);
        expectedFourthPointList.push_back(point4);
    }
    EXPECT_POINT_LIST_EQ(pointList3,expectedThirdPointList);
    EXPECT_POINT_LIST_EQ(pointList4,expectedFourthPointList);

}

TEST(Gcode,whenAskedToCreateEmptyFile_newEmptyFileCreated)
{
    Gcode gcode;
    gcode.create_empty_file();
    std::string suffix = ".txt";
    std::string fileName = gcode.get_file_name();
    EXPECT_TRUE(gcode.does_file_exist(fileName + suffix));
    gcode.delete_file();
}

TEST(Gcode,whenAskedToCreateFileWithName_namedFileCreated)
{
    Gcode gcode;
    Shape newShape;
    std::string fileName = "testFileName";
    gcode.generate_file(newShape,fileName);
    std::string suffix = ".txt";
    EXPECT_TRUE(gcode.does_file_exist(fileName + suffix));
    gcode.delete_file();
}

TEST(Gcode,whenAskedToDeleteFile_fileDeleted)
{
    Gcode gcode;
    Shape shape;
    std::string fileName = "deleteThisFile";
    gcode.generate_file(shape, fileName);
    std::string updatedFileName = gcode.get_file_name();
    std::string suffix = ".txt";
    EXPECT_TRUE(gcode.does_file_exist(fileName + suffix));

    gcode.delete_file();
    EXPECT_FALSE(gcode.does_file_exist(updatedFileName + suffix));
}

TEST(Gcode,whenAskedToGenerateGCodeFile_LayersPrintedInFile)
{
    Gcode newGcode;
    Shape newShape;
    std::string fileName = "testForLayers";
    newGcode.generate_file(newShape, fileName);
    // read some line from file
    newGcode.delete_file();
}

TEST(Gcode,whenAskedToGenerateGCodeFile_PointsPrintedInFile)
{
    Gcode newGcode;
    Shape newShape;
    std::string fileName = "testForPoints";
    newGcode.generate_file(newShape, fileName);
    // read some lines from file
    newGcode.delete_file();
}

TEST(Gcode,whenAskedForInitialExtrusionAmount_getZeroExtrusionAmount)
{
    Gcode newGcode;
    Shape newShape;
    double displacementA = newGcode.get_extruder_A_displacement();
    double displacementB = newGcode.get_extruder_B_displacement();
    double totalDisplacement = displacementA + displacementB;
    double expectedDisplacement = 0;
    EXPECT_DOUBLE_EQ(totalDisplacement,expectedDisplacement);
}

TEST(Gcode,whenAskedForLayerExtrusionAmount_getCorrectLayerExtrusionAmount)
{
    Gcode newGcode;
    Shape newShape;
    int firstLayer{0};
    double displacementA = newGcode.get_extruder_A_displacement(newShape, firstLayer);
    double displacementB = newGcode.get_extruder_B_displacement(newShape, firstLayer);
    double totalDisplacement = displacementA + displacementB;
    double expectedDisplacement = 0;
    EXPECT_DOUBLE_EQ(totalDisplacement,expectedDisplacement);
}


