#ifndef GCODE_H
#define GCODE_H
#include <fstream>
#include <sys/stat.h>
#include "shape.h"

class Gcode
{
public:
    Gcode();
    void   generate_file(Shape& shape);
    void   write_gcode(std::ofstream &fout, Shape &shape);
    std::ofstream create_empty_file();
    std::ofstream get_fout();
    std::string get_file_name() const;
    void   set_file_name(std::string const fileName);
    void   write_layer_gcode(std::ofstream& fout, Layer* layer);
    void   write_points_in_layer(std::ofstream& fout, Layer* layer);
    void   write_initial_gcode(std::ofstream& fout);
    void   write_points_in_path(std::ofstream &fout, Path* path);
    double get_extrusion_distance(double diameter, int mPointCount);
    double get_extruder_A_displacement() const;
    double get_extruder_B_displacement() const;
    void   increment_extruder_A_displacement(double extruderAStep);
    void   increment_extruder_B_displacement(double extruderBStep);
    double calculate_length();

protected:

private:
    std::string make_file_name_unique();
    bool   does_file_exist(const std::string& fileName);
    std::string mFileName{"newFile"};
    double mSyringeDiameter{14.9};
    double mSyringeCrossSectionalArea{174.366246256};
    double const pi{3.14159265359};
    double mPointCount{0};
    double mExtruderADisplacement{0};
    double mExtruderBDisplacement{0};

};

#endif // GCODE_H
