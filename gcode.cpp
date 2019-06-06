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
        fout << std::endl;
        fout << get_begin_layer_gcode(layerNumber, numberOfLayers) << std::endl;
        Layer* layer = shape.get_layer(layerNumber);
        write_layer_gcode(fout, layer, numberOfLayers);
    }
    write_end_gcode(fout);
}

std::ofstream Gcode::create_empty_file()
{
    std::string uniqueFileName = make_file_name_unique();
    std::string suffix = ".gcode";

    set_file_name(uniqueFileName);
    std::ofstream fout = get_fout();
    return fout;
}

std::ofstream Gcode::get_fout()
{
    std::string fileName = get_file_name();
    std::string suffix = ".gcode";
    std::string completeFileName = fileName + suffix;
    std::ofstream fout{completeFileName};
    bool failedToWrite = fout.fail();
    if (failedToWrite)
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
    std::string suffix = ".gcode";
    bool fileExists = does_file_exist(uniqueFileName + suffix);
    while(fileExists)
    {
        std::string incrementText = std::to_string(incrementCount);
        uniqueFileName = fileName + incrementText;
        incrementCount++;
        fileExists = does_file_exist(uniqueFileName + suffix);
    }
    return uniqueFileName;
}

bool Gcode::does_file_exist(const std::string& completeFileName)
{
    struct stat buf;
    if (stat(completeFileName.c_str(), &buf) == 0)
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

void Gcode::write_initial_gcode(std::ofstream& fout, Shape& shape)
{
    write_file_creation_information(fout);
    write_basic_settings(fout, shape);

    fout << "G21 ; set units to millimeters" << std::endl;
    fout << "G90 ; use absolute coordinates" << std::endl;
    Point firstLocation = shape.get_layer(0)->get_points().at(0);
    fout << std::endl;
    fout << "; Reset all axes:" << std::endl;
    fout << "G92 " << "X" << firstLocation.get_x() << " Y" << firstLocation.get_y() << " Z" << firstLocation.get_z() << " A0 " << "B0 " << std::endl;
    fout << std::endl;
}

std::string Gcode::get_begin_layer_gcode(int layerNumber, int totalLayers)
{
    std::string beginLayerGcode = "; Beginning layer " + std::to_string(layerNumber + 1) + " of " + std::to_string(totalLayers) + " ----------------------------";
    return beginLayerGcode;
}

void Gcode::write_layer_gcode(std::ofstream&  fout, Layer* layer, int numberOfLayers)
{
    if (layer->get_number() > 0)
    {
        fout << "G1  F" << get_translation_speed() << " ; Translation speed: " << get_translation_speed() << std::endl;
        double layerJump = get_layer_jump();
        double zLocation = layer->get_location();
        fout << "G1 " << " Z" << zLocation + layerJump << " ; Layer jump distance: " << layerJump << std::endl;

        fout << "G1 ";
        fout << " A" << get_extruder_displacement()[0] - get_layer_retraction_distance();
        fout << " B" << get_extruder_displacement()[1] - get_layer_retraction_distance();
        fout << " ; Layer retraction distance: " << get_layer_retraction_distance() << " mm" << std::endl;

        Point firstPoint = layer->get_points().at(0);
        fout << "G1 " << " X" << firstPoint.get_x() << " Y" << firstPoint.get_y() << " Z" << firstPoint.get_z() + layerJump << std::endl;
    }

    write_points_in_layer(fout, layer, numberOfLayers);
}

void Gcode::write_points_in_layer(std::ofstream& fout, Layer* layer, int numberOfLayers)
{
    fout << "G1  F" << get_print_speed() << " ; Print speed: " << get_print_speed() << std::endl;
    std::vector<Point> pointsInLayer = layer->get_points();
    int numberOfPointsInLayer = static_cast<int>(pointsInLayer.size());
    for (int i{0}; i<numberOfPointsInLayer; i++)
    {
        int pointNumber{i};
        Point point = pointsInLayer[i];
        fout << "G1 ";
        fout << " X" << point.get_x();
        fout << " Y" << point.get_y();
        double materialRatio = point.get_material();
        double extrusionDistance = get_extrusion_distance(point);
        if (pointNumber == 0)
        {
            extrusionDistance = 0;
            fout << " Z" << point.get_z();
        }
        increment_extruder_displacement(materialRatio,extrusionDistance);

        if (mLastPoint.get_material() != materialRatio)
        {
            double lastMaterialRatio = mLastPoint.get_material();
            double retractionDistance = get_material_switch_retraction_distance();
            double differenceA = lastMaterialRatio - (materialRatio);
            double differenceB = (-differenceA);
            if (differenceA > 0)
            {
                differenceA = 0;
            }
            if (differenceB > 0)
            {
                differenceB = 0;
            }
            fout << " A" << get_extruder_displacement()[0] + differenceA*retractionDistance;
            fout << " B" << get_extruder_displacement()[1] + differenceB*retractionDistance;
        }
        else
        {
            fout << " A" << get_extruder_displacement()[0];
            fout << " B" << get_extruder_displacement()[1];
        }
        if (mPointCount%20 == 0)
        {
            fout << "  ; Layer " << std::to_string(layer->get_number()+1) << " of " << std::to_string(numberOfLayers);
        }
        fout << std::endl;
        mPointCount += 1;
        mLastPoint = point;
    }
}

void Gcode::write_end_gcode(std::ofstream& fout)
{
    fout << "; End of file" << std::endl;
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

double Gcode::get_extrusion_distance(Point currentPoint)
{
    double diameter = currentPoint.get_diameter();
    double printCrossSectionalArea = pi*(diameter*diameter)/4;
    double printLength = calculate_length(currentPoint);
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

double Gcode::calculate_length(Point currentPoint)
{
    double length{0};
    Point lengthVector = mLastPoint - currentPoint;
    if (lengthVector.get_z() == 0)
    {
        length = lengthVector.get_magnitude();
    }
    return length;
}

void Gcode::delete_file()
{
    std::string fileName = get_file_name();
    std::string suffix = ".gcode";
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

    Layer* bottomLayer = shape.get_layer(0);
    Layer* topLayer = shape.get_layer(shape.get_number_of_layers()-1);
    double extrusionWidth = bottomLayer->get_extrusion_width();
    double extrusionMultiplier = bottomLayer->get_extrusion_multiplier();
    double infillPercentage = bottomLayer->get_infill_percentage();
    double infillAngle = bottomLayer->get_infill_angle();
    Path*  secondPath = bottomLayer->get_path(1);
    double materialResolution1 = secondPath->get_resolution();
    Path*  thirdPath = bottomLayer->get_path(2);
    double materialResolution2 = thirdPath->get_resolution();
    double materialResolution = materialResolution1;
    if (materialResolution2 > materialResolution1)
    {
        materialResolution = materialResolution2;
    }
    fout << "; Bottom Layer Settings: " << std::endl;
    fout << "; _Extrusion Width:      " << extrusionWidth << " mm" << std::endl;
    fout << "; _Extrusion Multiplier: " << extrusionMultiplier << "x" << std::endl;
    fout << "; _Infill Percentage:    " << infillPercentage << "%" << std::endl;
    fout << "; _Infill Angle:         " << infillAngle << "°" << std::endl;
    fout << "; _Material Resolution:  " << materialResolution << " mm" << std::endl;
    fout << std::endl;

    double shapeHeight = shape.get_height();
    double firstLayerWidth = bottomLayer->get_width();
    double firstLayerLength = bottomLayer->get_length();
    double topLayerWidth = bottomLayer->get_width();
    double topLayerLength = bottomLayer->get_length();
    fout << "; Shape Settings:        " << std::endl;
    fout << "; _Shape Height:         " << shapeHeight << " mm" << std::endl;
    fout << "; _Bottom Layer Width:   " << firstLayerWidth << " mm" << std::endl;
    fout << "; _Bottom Layer Length:  " << firstLayerLength << " mm" << std::endl;
    fout << "; _Top Layer Width:      " << topLayerWidth << " mm" << std::endl;
    fout << "; _Top Layer Length:     " << topLayerLength << " mm" << std::endl;
    fout << std::endl;
}

double Gcode::get_syringe_diameter() const
{
    return mSyringeDiameter;
}

void Gcode::set_syringe_diameter(double syringeDiameter)
{
    mSyringeDiameter = syringeDiameter;
}

void Gcode::set_layer_jump(const double layerJump)
{
    mLayerJump = layerJump;
}

double Gcode::get_layer_jump() const
{
    return  mLayerJump;
}

void   Gcode::set_translation_speed(double const translationSpeed)
{
    mTranslationSpeed = translationSpeed;
}

double Gcode::get_translation_speed() const
{
    return mTranslationSpeed;
}

void   Gcode::set_print_speed(double const printSpeed)
{
    mPrintSpeed = printSpeed;
}

double Gcode::get_print_speed() const
{
    return mPrintSpeed;
}

void   Gcode::set_layer_retraction_distance(double layerRetractionDistance)
{
    mLayerRetractionDistance = layerRetractionDistance;
}

double Gcode::get_layer_retraction_distance() const
{
    return mLayerRetractionDistance;
}

void   Gcode::set_material_switch_retraction_distance(double materialSwitchRetractionDistance)
{
    mMaterialSwitchRetractionDistance = materialSwitchRetractionDistance;
}

double Gcode::get_material_switch_retraction_distance() const
{
    return mMaterialSwitchRetractionDistance;
}

