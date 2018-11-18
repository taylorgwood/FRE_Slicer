#include <iostream>
#include "gtest/gtest.h"
#include "point.h"

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

TEST(a,b_c)
{

}

