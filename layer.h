#ifndef LAYER_H
#define LAYER_H
#include "path.h"

class Layer
{
public:
    ~Layer();
    Layer();
    double get_height() const;
    void   set_height(const double height);
    double get_extrusion_width() const;
    void   set_extrusion_width(const double extrusionWidth);
    double get_infill_percentage() const;
    void   set_infill_percentage(const double infillPercentage);
    double get_extrusion_multiplier() const;
    void   set_extrusion_multiplier(const double extrusionMultiplier);
    double get_modified_extrusion_width() const;
    double get_diameter_of_print() const;
    double get_volume() const;
    double get_area() const;
    void   set_area(const double area);
    int    get_number_of_paths();
    void   adjust_extrusion_width();

protected:

private:
    double const pi{3.14159265359};
    double mHeight{0.26};
    double mExtrusionWidth{0.26};
    double mInfillPercentage{100};
    double mExtrusionMultiplier{1};
    double mArea{100};
    double mWidth{10};

};

#endif // LAYER_H
