#ifndef GCODE_H
#define GCODE_H
#include <fstream>
#include <sys/stat.h>
#include "shape.h"
#include <string>
#include "stdio.h"
#include <iomanip>
#include <ctime>
#include <sstream>

class Gcode
{
public:
    Gcode();
    void   generate_file(Shape& shape, std::string fileName);
    void   write_gcode(std::ofstream &fout, Shape &shape);
    std::ofstream create_empty_file();
    std::ofstream get_fout();
    std::string get_file_name() const;
    void   set_file_name(std::string const fileName);
    void   write_layer_gcode(std::ofstream& fout, Layer* layer, unsigned int numberOfLayers);
    void   write_points_in_layer(std::ofstream& fout, Layer* layer, unsigned int numberOfLayers);
    void   write_initial_gcode(std::ofstream& fout, Shape &shape);
    void   write_end_gcode(std::ofstream& fout);
    double get_extrusion_distance(Point currentPoint);
    void   increment_extruder_displacement(double materialRatio, double extrusionDistance);
    std::vector<double> get_extruder_displacement() const;
    double calculate_length(Point currentPoint);
    bool   does_file_exist(const std::string& completeFileName);
    void   delete_file();
    std::string get_time_string();
    std::string get_begin_layer_gcode(unsigned int layerNumber, unsigned int totalLayers);
    void   write_file_creation_information(std::ofstream &fout);
    void   write_basic_settings(std::ofstream& fout, Shape &shape);
    double get_syringe_diameter() const;
    void   set_syringe_diameter(double syringeDiameter);
    void   set_travel_jump(double const travelJump);
    double get_travel_jump() const;
    void   set_travel_speed(double const travelSpeed);
    double get_travel_speed() const;
    void   set_print_speed(double const printSpeed);
    double get_print_speed() const;
    void   set_travel_retraction_distance(double travelRetractionDistance);
    double get_travel_retraction_distance() const;
    void   set_material_switch_retraction_distance(double materialSwitchRetractionDistance);
    double get_material_switch_retraction_distance() const;


protected:

private:
    std::string make_file_name_unique();
    std::string mFileName{"newFile"};
    double mSyringeDiameter{14.9};
    double mSyringeCrossSectionalArea{174.366246256};
    double const pi{3.14159265359};
    unsigned int mPointCount{0};
    std::vector<double> mExtruderDisplacement{0,0};
    Point  mLastPoint{0,0,0};
    double mTravelJump{0};
    double mTravelSpeed{720};
    double mPrintSpeed{720};
    double mTravelRetractionDistance{0};
    double mMaterialSwitchRetractionDistance{0};
};

#endif // GCODE_H
