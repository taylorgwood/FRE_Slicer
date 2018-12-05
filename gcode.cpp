#include "gcode.h"

Gcode::Gcode()
{

}

void Gcode::generate_file(Shape& shape)
{
    std::ofstream fout = create_empty_file();
    generate_gcode(fout, shape);
    fout.close();
}

void Gcode::generate_gcode(std::ofstream& fout, Shape& shape)
{
    int numberOfLayers = shape.get_number_of_layers();
    for (int i{0}; i<numberOfLayers; i++)
    {
        fout << "Layer" << i << std::endl;
        int layerNumber{i};
        generate_layer_gcode(fout, shape, layerNumber);
    }
}

std::ofstream Gcode::create_empty_file()
{
    make_file_name_unique();
    std::string fileName = get_file_name();
    std::ofstream fout{fileName};
    if (fout.fail())
    {
        std::cout << "Failed to write to file." << std::endl;
    }
    return fout;
}

void Gcode::make_file_name_unique()
{
    std::string fileName = get_file_name();

    if (does_file_exist(fileName))
    {
        std::string suffix = "(1)";
        fileName = fileName + suffix;
    }

    set_file_name(fileName);
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

std::string Gcode::get_file_name() const
{
    return mFileName;
}

void Gcode::set_file_name(std::string const fileName)
{
    mFileName = fileName;
}

void Gcode::generate_layer_gcode(std::ofstream& fout, Shape& shape, int layerNumber)
{
    Layer* layer = shape.get_layer(layerNumber);
}
