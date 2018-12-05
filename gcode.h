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
    void generate_gcode(std::ofstream& fout, Shape &shape);
    std::ofstream create_empty_file();
    std::string get_file_name() const;
    void set_file_name(std::string const fileName);
    void generate_layer_gcode(std::ofstream& fout, Shape& shape, int layerNumber);

protected:

private:
    void make_file_name_unique();
    bool does_file_exist(const std::string& fileName);
    std::string mFileName{"newFile.txt"};

};

#endif // GCODE_H
