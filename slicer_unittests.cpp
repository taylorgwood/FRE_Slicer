#include <iostream>
#include "gtest/gtest.h"
#include "point.h"
#include "path.h"
#include "layer.h"
#include "shape.h"

void EXPECT_POINT_EQ(Point firstPoint, Point secondPoint)
{
    EXPECT_DOUBLE_EQ(firstPoint.get_x(),secondPoint.get_x());
    EXPECT_DOUBLE_EQ(firstPoint.get_y(),secondPoint.get_y());
    EXPECT_DOUBLE_EQ(firstPoint.get_z(),secondPoint.get_z());
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
    layer.set_area(100);
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

TEST(LayerArray,givenShape_getCorrectlySizedLayerArray)
{
    Shape shape;
    shape.set_height(10);
    shape.set_layer_height(0.26);
    int numberOfLayers{38};
    std::vector<Layer*> layerList = shape.get_layer_list();
    size_t numberOfLayersInList = layerList.size();
    EXPECT_EQ(numberOfLayers,numberOfLayersInList);
    //    std::vector<Layer*> layerList = shape.get_layer_list()
//    int numberOfLayersInList = shape.mLayerList->size();
}
