#ifndef LAYER_H
#define LAYER_H
#include "path.h"

class Layer
{
public:
    ~Layer();
    Layer();
    double get_height() const;
    void   set_height(double const height);

protected:

private:
    double mHeight{0.26};

};

#endif // LAYER_H
