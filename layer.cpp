#include "layer.h"

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
