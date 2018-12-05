#include "gcode.h"

Gcode::Gcode()
{

}


void Gcode::generate_gcode(std::string& fileName)
{
    std::ofstream fout = create_file(fileName);
    // loop through layers, write_layer_gcode_to_file();
        // loop through paths, write_path_points_to_file();


    fout << "Hello File!";

    fout.close();

}

std::ofstream Gcode::create_file(std::string& fileName)
{
    int numberOfChecks{0};
    std::string newFileName = make_unique_file_name(fileName,numberOfChecks);
    std::ofstream fout{fileName};
    if (fout.fail())
    {
        std::cout << "Failed to write to file." << std::endl;
    }
    return fout;
}

std::string Gcode::make_unique_file_name(std::string& fileName, int numberOfChecks)
{
    std::string newFileName = fileName;

    if (does_file_exist(fileName))
    {
        // do something here;
    }

    return newFileName;
}

bool Gcode::does_file_exist(const std::string& fileName)
{
    struct stat buf;
    if (stat(fileName.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}
