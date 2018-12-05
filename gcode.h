#ifndef GCODE_H
#define GCODE_H
#include <fstream>
#include <sys/stat.h>
#include "shape.h"

class Gcode
{
public:
    Gcode();
    void generate_file(Shape &shape);
    void write_gcode(std::ofstream& fout, Shape &shape);
    std::ofstream create_empty_file();
    std::string get_file_name() const;
    void set_file_name(std::string const fileName);
    void write_layer_gcode(std::ofstream& fout, Shape& shape, int layerNumber);
    void write_points_in_layer(std::ofstream& fout, Layer *layer);
    void write_initial_gcode(std::ofstream& fout, Shape& shape);



protected:

private:
    void make_file_name_unique();
    bool does_file_exist(const std::string& fileName);
    std::string mFileName{"newFile.txt"};

};

#endif // GCODE_H
