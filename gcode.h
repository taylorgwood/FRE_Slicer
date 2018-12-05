#ifndef GCODE_H
#define GCODE_H
#include <fstream>
#include <sys/stat.h>
#include "shape.h"

class Gcode
{
public:
    Gcode();
    void generate_gcode(std::string &fileName);
    std::ofstream create_file(std::string& fileName);

protected:

private:
    std::string make_unique_file_name(std::string& fileName, int numberOfChecks);
    bool does_file_exist(const std::string& fileName);

};

#endif // GCODE_H
