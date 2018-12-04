#include "shape.h"
#include <fstream>
#include <sys/stat.h>

Shape::~Shape()
{
    //    layerArray = nullptr;
}

Shape::Shape():mLayerList{new std::vector<Layer*>}
{
    create_layers();
    //    create_layer_array();
    //    fill_layer_array();
}

int Shape::get_number_of_layers()
{
    double exactNumberOfLayers = mHeight/mLayerHeight;
    int flooredNumberOfLayers = int(floor(mHeight/mLayerHeight));
    int numberOfLayers{flooredNumberOfLayers};
    if ((exactNumberOfLayers-flooredNumberOfLayers)>=0.5)
    {
        numberOfLayers = flooredNumberOfLayers+1;
    }
    return numberOfLayers;
}

void Shape::adjust_layer_height()
{
    int numberOfLayers = get_number_of_layers();
    double realLayerHeight = mHeight/numberOfLayers;
    set_layer_height(realLayerHeight);
}

void Shape::set_layer_height(const double layerHeight)
{
    mLayerHeight = layerHeight;
}

double Shape::get_layer_height() const
{
    return mLayerHeight;
}

void Shape::set_height(const double height)
{
    mHeight = height;
}

double Shape::get_height() const
{
    return mHeight;
}

void Shape::create_layers()
{
    int numberOfLayers = get_number_of_layers();
    adjust_layer_height();
    double layerLocation{0};
    for (int i{0}; i<numberOfLayers; i++)
    {
        int layerNumber{i};
        double layerHeight = get_layer_height();
        layerLocation += layerHeight;
        Layer* newLayer = new Layer(layerNumber,layerLocation);
        mLayerList->push_back(newLayer);
    }
}

std::vector<Layer*> Shape::get_layer_list()
{
    return *mLayerList;
}

std::vector<Point> Shape::get_points_in_layer(int layerNumber)
{
    std::vector<Layer*> layerList = get_layer_list();
    Layer* layer = layerList[layerNumber];
    std::vector<Point> pointsInLayer = layer->get_points();
    return pointsInLayer;
}

std::vector<Point> Shape::get_points()
{
    std::vector<Point> pointList;
    int numberOfLayers = get_number_of_layers();
    for (int i{0}; i<numberOfLayers; i++)
    {
        std::vector<Layer*> layerList = get_layer_list();
        Layer *layer = layerList[1];
        std::vector<Point> pointsInLayer = layer->get_points();
        size_t numberOfPointsInLayer = pointsInLayer.size();
        for (int j{0}; j<numberOfPointsInLayer; j++)
        {
            Point point = pointsInLayer[j];
            pointList.push_back(point);
        }
    }
    return pointList;
}

std::vector<double> Shape::get_layer_locations()
{
    std::vector<double> layerLocationVector;
    double layerLocation{0};
    std::vector<Layer*> layerList = get_layer_list();
    size_t numberOfLayers = get_number_of_layers();
    for (int i{0}; i<numberOfLayers; i++)
    {
        Layer *layer = layerList[i];
        double layerHeight = layer->get_height();
        layerLocation += layerHeight;
        layerLocationVector.push_back(layerLocation);
    }
    return layerLocationVector;
}

void Shape::generate_gcode(std::string fileName)
{
    // create file
    // loop through layers, write_layer_gcode_to_file();
        // loop through paths, write_path_points_to_file();
    std::ofstream fout{fileName};
    if (fout.fail())
    {
        std::cout << "Failed to write to file" << std::endl;
        return;
    }
    int numberOfChecks{0};
    check_file_name(fileName,numberOfChecks);

    fout << "Hello File!";

    fout.close();
}

std::string Shape::check_file_name(std::string fileName, int numberOfChecks)
{
    std::string newFileName = fileName;

    if (does_file_exist(fileName))
    {
        // do something here;
    }

    return newFileName;
}

bool Shape::does_file_exist(const std::string& fileName)
{
    struct stat buf;
    if (stat(fileName.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}
