#include "layer.h"
#include <cmath>

Layer::~Layer()
{

}

Layer::Layer()
{

}

double Layer::get_height() const
{
    return mHeight;
}

void Layer::set_height(const double height)
{
    mHeight = height;
}

double Layer::get_extrusion_width() const
{
    return mExtrusionWidth;
}

void Layer::set_extrusion_width(const double extrusionWidth)
{
    mExtrusionWidth = extrusionWidth;
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
    return mHeight;
}

void Layer::set_extrusion_multiplier(const double extrusionMultiplier)
{
    mExtrusionMultiplier = extrusionMultiplier;
}

double Layer::get_real_extrusion_width() const
{
    double infillRatio = mInfillPercentage/100;
    double realExtrusionWidth = mExtrusionWidth/infillRatio;
    return realExtrusionWidth;
}

double Layer::get_diameter_of_print() const
{
    double volume = get_volume();
    double realExtrusionWidth = get_real_extrusion_width();
    double diameterOfPrint = sqrt(volume*4*realExtrusionWidth/(mArea*pi));
    return diameterOfPrint;
}

double Layer::get_volume() const
{
    double infillRatio = mInfillPercentage/100;
    double volume = mArea*mHeight*infillRatio*mExtrusionMultiplier;
    return volume;
}

double Layer::get_area() const
{
    return mArea;
}

void Layer::set_area(const double area)
{
    mArea = area;
}
