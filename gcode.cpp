#include "gcode.h"

Gcode::Gcode()
{
}

void Gcode::generate_file(Shape& shape, std::string fileName)
{
    set_file_name(fileName);
    std::ofstream fout = create_empty_file();
    write_gcode(fout, shape);
    fout.close();
}

void Gcode::write_gcode(std::ofstream& fout, Shape& shape)
{
    write_initial_gcode(fout, shape);
    int numberOfLayers = shape.get_number_of_layers();
    for (int i{0}; i<numberOfLayers; i++)
    {
        int layerNumber{i};
        fout << get_begin_layer_gcode(layerNumber) << std::endl;
        Layer* layer = shape.get_layer(layerNumber);
        write_layer_gcode(fout, layer);
    }
}

std::ofstream Gcode::create_empty_file()
{
    std::string uniqueFileName = make_file_name_unique();
    std::string suffix = ".txt";

    set_file_name(uniqueFileName);
    std::ofstream fout = get_fout();
    return fout;
}

std::ofstream Gcode::get_fout()
{
    std::string fileName = get_file_name();
    std::string suffix = ".txt";
    std::string completeFileName = fileName + suffix;
    std::ofstream fout{completeFileName};
    if (fout.fail())
    {
        std::cout << "Failed to write to file." << std::endl;
    }
    return fout;
}

std::string Gcode::make_file_name_unique()
{
    std::string fileName = get_file_name();
    std::string uniqueFileName = fileName;
    int incrementCount{1};
    std::string suffix = ".txt";
    while(does_file_exist(uniqueFileName + suffix))
    {
        std::string incrementText = std::to_string(incrementCount);
        uniqueFileName = fileName + incrementText;
        incrementCount++;
    }
    return uniqueFileName;
}

bool Gcode::does_file_exist(const std::string& completeFileName)
{
    struct stat buf;
    if (stat(completeFileName.c_str(), &buf) != -1)
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

void Gcode::write_layer_gcode(std::ofstream&  fout, Layer* layer)
{
    double zLocation = layer->get_location();
    fout << "G1 " << " Z" << zLocation;
    fout << " F1200.000" << std::endl;

    write_points_in_layer(fout, layer);
}

void Gcode::write_points_in_layer(std::ofstream& fout, Layer* layer)
{
    std::vector<Path*> pathsInLayer = layer->get_path_list();
    size_t numberOfPathsInLayer = layer->get_number_of_paths();
    for (int i{0}; i<numberOfPathsInLayer; i++)
    {
        Path* path = pathsInLayer[i];
        write_points_in_path(fout, path);
    }
}

void Gcode::write_initial_gcode(std::ofstream& fout, Shape& shape)
{
    write_file_creation_information(fout);
    write_basic_settings(fout, shape);

    // Startup code
    fout << "G28 ; home all axes" << std::endl;
    fout << "G21 ; set units to millimeters" << std::endl;
    fout << "G90 ; use absolute coordinates" << std::endl;
    fout << std::endl;
}

std::string Gcode::get_time_string()
{
    auto time = std::time(nullptr);
    auto localTime = *std::localtime(&time);

    std::ostringstream oss;
    oss << std::put_time(&localTime, "%m-%d-%Y at %H:%M:%S");
    auto timeString = oss.str();
    return timeString;
}

void Gcode::write_points_in_path(std::ofstream& fout, Path* path)
{
    double diameter = path->get_diameter();
    std::vector<Point*> pointsInPath = path->get_point_list();
    size_t numberOfPointsInPath = path->get_number_of_points();
    for (int i{0}; i<numberOfPointsInPath; i++)
    {
        Point* point = pointsInPath[i];
        fout << "G1 ";
        fout << " X" << point->get_x();
        fout << " Y" << point->get_y();
        double materialRatio = point->get_material();
        double extrusionDistance = get_extrusion_distance(diameter, path, i);
        increment_extruder_displacement(materialRatio,extrusionDistance);
        fout << " A" << get_extruder_displacement()[0];
        fout << " B" << get_extruder_displacement()[1];
        fout << std::endl;
        mPointCount += 1;
    }
}

double Gcode::get_extrusion_distance(double diameter, Path* path, int pointCount)
{
    double printCrossSectionalArea = pi*(diameter*diameter)/4;
    double printLength = calculate_length(path, pointCount);
    double printVolume = printCrossSectionalArea*printLength;
    double extrusionDistance = printVolume/mSyringeCrossSectionalArea;
    return extrusionDistance;
}

std::vector<double> Gcode::get_extruder_displacement() const
{
    return mExtruderDisplacement;
}

void Gcode::increment_extruder_displacement(double materialRatio, double extrusionDistance)
{
    double extruderStepA = (materialRatio*extrusionDistance);
    double extruderStepB = ((1-materialRatio)*extrusionDistance);
    mExtruderDisplacement[0] += extruderStepA;
    mExtruderDisplacement[1] += extruderStepB;
}

double Gcode::calculate_length(Path* path, int pointCount)
{
    double length{0};
    std::vector<Point*> pointsInPath = path->get_point_list();
    size_t numberOfPointsInPath = path->get_number_of_points();
    if (pointCount != 0)
    {
        Point* previousPoint = pointsInPath[pointCount-1];
        Point* currentPoint = pointsInPath[pointCount];
        Point lengthVector = *currentPoint - *previousPoint;
        length = lengthVector.get_magnitude();
    }
    return length;
}

void Gcode::delete_file()
{
    std::string fileName = get_file_name();
    std::string suffix = ".txt";
    std::string completeFileName = fileName + suffix;
    if(does_file_exist(completeFileName))
    {
        std::remove(completeFileName.c_str());
        if( std::remove(completeFileName.c_str()) == 0 )
        {
            std::cout << "Error deleting file" << std::endl;
        }
    }
}

std::string Gcode::get_begin_layer_gcode(int layerNumber)
{
    std::string beginLayerGcode = "; Beginning layer: " + layerNumber;
    return beginLayerGcode;
}

void Gcode::write_file_creation_information(std::ofstream& fout)
{
    fout << "; File created on " << get_time_string() << std::endl;
    fout << "; Gcode generated by FRE_Slicer." << std::endl;
    fout << "; Code written by Taylor Greenwood, November 2018." << std::endl;
    fout << std::endl;
}

void Gcode::write_basic_settings(std::ofstream& fout, Shape& shape)
{
    double layerHeight = shape.get_layer_height();
    int    numberOfLayers = shape.get_number_of_layers();
    fout << "; Layer Height:          " << layerHeight << " mm" << std::endl;
    fout << "; Number Of Layers:      " << numberOfLayers << std::endl;
    fout << std::endl;

    Layer* firstLayer = shape.get_layer(0);
    double extrusionWidth = firstLayer->get_extrusion_width();
    double extrusionMultiplier = firstLayer->get_extrusion_multiplier();
    double infillPercentage = firstLayer->get_infill_percentage();
    Path*  firstPath = firstLayer->get_path(0);
    double materialResolution = firstPath->get_resolution();
    fout << "; First Layer Settings:  " << std::endl;
    fout << ";  Extrusion Width:      " << extrusionWidth << " mm" << std::endl;
    fout << ";  Extrusion Multiplier: " << extrusionMultiplier << std::endl;
    fout << ";  Infill Percentage:    " << infillPercentage << std::endl;
    fout << ";  Material Resolution:  " << materialResolution << " mm" << std::endl;
    fout << std::endl;

    double shapeHeight = shape.get_height();
    double firstLayerWidth = firstLayer->get_width();
    double firstLayerLength = firstLayer->get_length();
    fout << "; Shape Settings:        " << std::endl;
    fout << ";  Shape Height:         " << shapeHeight << " mm" << std::endl;
    fout << ";  First Layer Width:    " << firstLayerWidth << " mm" << std::endl;
    fout << ";  First Layer Length:   " << firstLayerLength << " mm" << std::endl;
    fout << std::endl;
}
