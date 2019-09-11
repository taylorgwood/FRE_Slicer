#include "gcode.h"

Gcode::Gcode()
{
}

void Gcode::generate_file(Shape& shape, std::string fileName)
{
    mExtruderDisplacement = {0,0};
    set_file_name(fileName);
    std::ofstream fout = create_empty_file();
    write_gcode(fout, shape);
    fout.close();
}

void Gcode::write_gcode(std::ofstream& fout, Shape& shape)
{
    write_initial_gcode(fout, shape);
    unsigned int numberOfLayers = shape.get_number_of_layers();
    for (unsigned int i{0}; i<numberOfLayers; i++)
    {
        unsigned int layerNumber{i};
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
    set_file_name(uniqueFileName);
    std::ofstream fout = get_fout();
    return fout;
}

std::ofstream Gcode::get_fout()
{
    std::string fileName = get_file_name();
    std::string suffix = ".txt";
    //    std::string suffix = "";
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
    std::string suffix = ".txt";
    //    std::string suffix = "";
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
    fout << std::endl;
    fout << "; Original file name: " << get_file_name() << std::endl;
    fout << std::endl;
    fout << "; Last version update: July 9, 2019" << std::endl;
    write_file_creation_information(fout);
    write_gcode_settings(fout);
    write_print_settings(fout, shape);

    fout << "G21 ; set units to millimeters" << std::endl;
    fout << "G90 ; use absolute coordinates" << std::endl;
    Point firstLocation = shape.get_layer(0)->get_point_list().at(0);
    fout << std::endl;
    fout << ";   Reset all axes:" << std::endl;
    double startPrintPlungeDistance = get_start_print_plunge_distance();
    fout << "G92 " << "X" << firstLocation.get_x() << " Y" << firstLocation.get_y() << " Z" << firstLocation.get_z()+startPrintPlungeDistance << " A0 " << "B0 " << std::endl;
    fout << "G1  " << "X" << firstLocation.get_x() << " Y" << firstLocation.get_y() << " Z" << firstLocation.get_z()+startPrintPlungeDistance << " A0 " << "B0 ";
    fout << " ; Start print plunge distance:   " << get_start_print_plunge_distance() << " mm (-Z)" << std::endl;

    fout << std::endl;
}

std::string Gcode::get_begin_layer_gcode(unsigned int layerNumber, unsigned int totalLayers)
{
    std::string beginLayerGcode = ";   Beginning layer " + std::to_string(layerNumber + 1) + " of " + std::to_string(totalLayers) + " ----------------------------";
    return beginLayerGcode;
}

void Gcode::write_layer_gcode(std::ofstream&  fout, Layer* layer, unsigned  int numberOfLayers)
{ 
    unsigned int layerNumber = layer->get_number();
    if (layerNumber > 0)
    {
        fout << "G1  F" << get_travel_speed()*60;
        fout << '\t' << '\t' << " ; Travel speed: " << get_travel_speed() << " mm/s" << std::endl;
        double travelJump = get_travel_jump();
        double travelJog = get_travel_jog();
        if (layerNumber%2 != 0)
        {
            travelJog = -travelJog;
        }
        double zLocation = layer->get_location();
        fout << "G1 " << " Z" << zLocation + travelJump;
        fout << '\t' << '\t' << " ; Travel jump distance: " << travelJump << " mm" << std::endl;

        fout << "G1 ";
        fout << " A" << get_extruder_displacement()[0] - get_travel_retraction_distance().at(0);
        fout << " B" << get_extruder_displacement()[1] - get_travel_retraction_distance().at(1);
        fout << '\t' << " ; Travel retraction distance: (A,B) " << get_travel_retraction_distance().at(0) << "," << get_travel_retraction_distance().at(1) << " mm" << std::endl;
        fout << "G1 " << " Y" << mLastPoint.get_y()+travelJog;
        fout << '\t' << '\t' << " ; Travel jog distance: " << travelJog << " mm" << std::endl;
        Point firstPoint = layer->get_point_list().at(0);
        fout << "G1 " << " X" << firstPoint.get_x() << " Y" << firstPoint.get_y()+travelJog << " Z" << firstPoint.get_z() + travelJump << std::endl;
        fout << "G1 " << " X" << firstPoint.get_x() << " Y" << firstPoint.get_y() << " Z" << firstPoint.get_z() + travelJump << std::endl;
    }

    fout << "G1  F" << get_print_speed()*60 << '\t' << '\t' << " ; Print speed: " << get_print_speed() << " mm/s" << std::endl;
    fout << std::endl;

    write_points_in_layer(fout, layer, numberOfLayers);
}

void Gcode::write_points_in_layer(std::ofstream& fout, Layer* layer, unsigned int numberOfLayers)
{
    unsigned int streamSize{6};
    fout << std::fixed << std::setprecision(streamSize);

    std::vector<Point> pointsInLayer = layer->get_point_list();
    if (mSimplifyPointList == true)
    {
        pointsInLayer = layer->get_simplified_point_list();
    }
    unsigned int numberOfPointsInLayer = static_cast<unsigned int>(pointsInLayer.size());
    for (unsigned int i{0}; i<numberOfPointsInLayer; i++)
    {
        unsigned int pointNumber{i};
        Point point = pointsInLayer[i];
        fout << "G1 ";
        fout << " X" << point.get_x();
        fout << " Y" << point.get_y();
        double materialRatio = point.get_material();
//        double lastMaterialRatio = mLastPoint.get_material();
        double extrusionDistance = get_extrusion_distance(point);
        if (pointNumber == 0)
        {
            extrusionDistance = 0;
            fout << " Z" << point.get_z();
        }

        increment_extruder_displacement(materialRatio,extrusionDistance);

        fout << " A" << get_extruder_displacement()[0];
        fout << " B" << get_extruder_displacement()[1];

        if (mPointCount%20 == 0)
        {
            fout << "  ; Layer " << std::to_string(layer->get_number()+1) << " of " << std::to_string(numberOfLayers);
        }
        fout << std::endl;

        if (get_material_switch_retraction_distance() > 0)
        {
        if (i<numberOfPointsInLayer-1)
        {
        Point nextPoint = pointsInLayer[i+1];
        double nextMaterialRatio = nextPoint.get_material();
        if (abs(nextMaterialRatio - materialRatio) > 0.0001)
        {
            double retractionDistance = get_material_switch_retraction_distance();
            double differenceA = nextMaterialRatio - materialRatio;
            double differenceB = -differenceA;
            if (differenceA > 0)
            {
                differenceA = 0;
            }
            if (differenceB > 0)
            {
                differenceB = 0;
            }
            double retractA = differenceA*retractionDistance;
            double retractB = differenceB*retractionDistance;
            fout << "G1 ";
            fout << " X" << point.get_x();
            fout << " Y" << point.get_y();
            fout << " A" << get_extruder_displacement()[0] + retractA;
            fout << " B" << get_extruder_displacement()[1] + retractB;
            fout << "  ; Material Switch Retraction "; // << retractA << ", " << retractB << " mm (A,B)";
            fout << std::endl;
        }
        }
        }
        mPointCount += 1;
        mLastPoint = point;
    }
    fout << std::defaultfloat;
}

void Gcode::write_end_gcode(std::ofstream& fout)
{
    fout << std::endl;
    fout << "G1 " << " F" << get_travel_speed()*60;
    fout << '\t' << '\t' << " ; Travel speed: " << get_travel_speed() << " mm/s" << std::endl;

    fout << "G1 ";
    fout << " A" << get_extruder_displacement()[0] - get_travel_retraction_distance().at(0);
    fout << '\t' << " B" << get_extruder_displacement()[1] - get_travel_retraction_distance().at(1);
    fout << '\t' << " ; Travel retraction distance: (A,B) " << get_travel_retraction_distance().at(0) << "," << get_travel_retraction_distance().at(1) << " mm" << std::endl;

    fout << "G1 " << " Z" << get_finish_print_jump_distance();
    fout << '\t' << '\t' << " ; Jump distance: " << get_finish_print_jump_distance() << " mm" << std::endl;
    fout << "G1 " << " X" << get_finish_print_jog_distance();
    fout << '\t' << '\t' << " ; Jog distance: " << get_finish_print_jog_distance() << " mm" << std::endl;

    fout << ";   End of file" << std::endl;
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
    double syringeDiameter = get_syringe_diameter();
    double syringeCrossSectionalArea = pi*(syringeDiameter*syringeDiameter)/4;
    double extrusionDistance = printVolume/syringeCrossSectionalArea;
    return extrusionDistance;
}

std::vector<double> Gcode::get_extruder_displacement() const
{
    return mExtruderDisplacement;
}

void Gcode::increment_extruder_displacement(double materialRatio, double extrusionDistance)
{
    double extruderStepA = materialRatio*extrusionDistance;
    double extruderStepB = (1-materialRatio)*extrusionDistance;

    bool singleMaterial = is_single_material();
    if (singleMaterial == true)
    {
        extruderStepA = 0;
        extruderStepB = 0;
        bool extruderChoiceA = get_extruder_choice().at(0);
        bool extruderChoiceB = get_extruder_choice().at(1);
        if (extruderChoiceA == true)
        {
            extruderStepA = extrusionDistance;
        }
        if (extruderChoiceB == true)
        {
            extruderStepB = extrusionDistance;
        }
    }
    mExtruderDisplacement[0] += extruderStepA;
    mExtruderDisplacement[1] += extruderStepB;
}

double Gcode::calculate_length(Point currentPoint)
{
    double length{0};
    Point lengthVector = mLastPoint - currentPoint;
    if (lengthVector.get_z() < 0.001)
    {
        length = lengthVector.get_magnitude();
    }
    return length;
}

void Gcode::delete_file()
{
    std::string fileName = get_file_name();
    std::string suffix = ".txt";
    //    std::string suffix = "";
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
    fout << "; Code written by Taylor Greenwood." << std::endl;
    fout << std::endl;
}

void Gcode::write_gcode_settings(std::ofstream& fout)
{
    fout << "; Gcode Settings:         " << std::endl;
    fout << ";   Syringe Diameter:     " << get_syringe_diameter() << " mm " << std::endl;
    fout << ";   Travel Jump:          " << get_travel_jump() << " mm (+Z, between layers)" << std::endl;
    fout << ";   Travel Jog:           " << get_travel_jog() << " mm (+-Y, between layers)" << std::endl;
    fout << ";   Start Print Plunge:   " << get_start_print_plunge_distance() << " mm (-Z)" << std::endl;
    fout << ";   Finish Print Jump:    " << get_finish_print_jump_distance() << " mm (+Z)" << std::endl;
    fout << ";   Finish Print Jog:     " << get_finish_print_jog_distance() << " mm (+X)" << std::endl;
    fout << ";   Travel Speed:         " << get_travel_speed() << " mm/s" << std::endl;
    fout << ";   Print Speed:          " << get_print_speed() << " mm/s" << std::endl;
    fout << ";   Retraction Speed:     " << get_retraction_speed() << " mm" << std::endl;
    fout << ";   Travel Retraction:    " << get_travel_retraction_distance().at(0) << "," << get_travel_retraction_distance().at(1) << " mm (A,B)" << std::endl;
    fout << ";   Material Chng Retract:" << get_material_switch_retraction_distance() << " mm, scaled" << std::endl;
    fout << std::endl;
}


void Gcode::write_print_settings(std::ofstream& fout, Shape& shape)
{
    std::string isSimplified = "Disabled";
    if (mSimplifyPointList == true)
    {
        isSimplified = "Enabled";
    }
    fout << "; Simplify Points:        " << isSimplified << std::endl;
    fout << std::endl;

    double layerHeight = shape.get_layer_height();
    unsigned int numberOfLayers = shape.get_number_of_layers();
    fout << "; Layer Height:           " << layerHeight << " mm" << std::endl;
    fout << "; Number Of Layers:       " << numberOfLayers << std::endl;
    fout << std::endl;

    Layer* bottomLayer = shape.get_layer(0);
    Layer* topLayer = shape.get_layer(shape.get_number_of_layers()-1);
    double extrusionWidth = bottomLayer->get_extrusion_width();
    double extrusionMultiplier = bottomLayer->get_extrusion_multiplier();
    double infillPercentage = bottomLayer->get_infill_percentage();
    double infillAngle = bottomLayer->get_infill_angle();
    Path*  secondPath = bottomLayer->get_path(1);
    double resolutionLength1 = secondPath->get_resolution_length();
    Path*  thirdPath = bottomLayer->get_path(2);
    double resolutionLength2 = thirdPath->get_resolution_length();
    double materialResolution = resolutionLength1;
    if (resolutionLength2 > resolutionLength1)
    {
        materialResolution = resolutionLength2;
    }
    fout << "; Bottom Layer Settings: " << std::endl;
    fout << ";   Extrusion Width:      " << extrusionWidth << " mm, *includes infill percentage" << std::endl;
    fout << ";   Extrusion Multiplier: " << extrusionMultiplier << "x" << std::endl;
    fout << ";   Infill Percentage:    " << infillPercentage << "%" << std::endl;
    fout << ";   Infill Angle:         " << infillAngle << " deg" << std::endl; // Don't put a degree sign here - throws an error in Mach3
    fout << ";   Resolution Length:    " << materialResolution << " mm" << std::endl;
    fout << std::endl;

    double shapeHeight = shape.get_height();
    double firstLayerWidth = bottomLayer->get_width();
    double firstLayerLength = bottomLayer->get_length();
    double topLayerWidth = topLayer->get_width();
    double topLayerLength = topLayer->get_length();
    fout << "; Shape Settings:        " << std::endl;
    fout << ";   Shape Height:         " << shapeHeight << " mm" << std::endl;
    fout << ";   Bottom Layer Width:   " << firstLayerWidth << " mm" << std::endl;
    fout << ";   Bottom Layer Length:  " << firstLayerLength << " mm" << std::endl;
    fout << ";   Top Layer Width:      " << topLayerWidth << " mm" << std::endl;
    fout << ";   Top Layer Length:     " << topLayerLength << " mm" << std::endl;
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

void Gcode::set_travel_jump(const double travelJump)
{
    mTravelJump = travelJump;
}

double Gcode::get_travel_jump() const
{
    return  mTravelJump;
}

void Gcode::set_travel_jog(const double travelJog)
{
    mTravelJog = travelJog;
}

double Gcode::get_travel_jog() const
{
    return  mTravelJog;
}

void Gcode::set_finish_print_jump_distance(const double finishPrintJumpDistance)
{
    mFinishPrintJumpDistance = finishPrintJumpDistance;
}

double Gcode::get_finish_print_jump_distance() const
{
    return mFinishPrintJumpDistance;
}

void Gcode::set_finish_print_jog_distance(const double finishPrintJogDistance)
{
    mFinishPrintJogDistance = finishPrintJogDistance;
}

double Gcode::get_finish_print_jog_distance() const
{
    return mFinishPrintJogDistance;
}

void Gcode::set_start_print_plunge_distance(const double startPrintPlungeDistance)
{
    mStartPrintPlungeDistance = startPrintPlungeDistance;
}

double Gcode::get_start_print_plunge_distance() const
{
    return mStartPrintPlungeDistance;
}

void   Gcode::set_travel_speed(double const travelSpeed)
{
    mTravelSpeed = travelSpeed;
}

double Gcode::get_travel_speed() const
{
    return mTravelSpeed;
}

void   Gcode::set_print_speed(double const printSpeed)
{
    mPrintSpeed = printSpeed;
}

double Gcode::get_print_speed() const
{
    return mPrintSpeed;
}

void   Gcode::set_retraction_speed(double const retractionSpeed)
{
    mRetractionSpeed = retractionSpeed;
}

double Gcode::get_retraction_speed() const
{
    return mRetractionSpeed;
}

void   Gcode::set_travel_A_retraction_distance(double travelARetractionDistance)
{
    mTravelRetractionDistance.at(0) = travelARetractionDistance;
}

void   Gcode::set_travel_B_retraction_distance(double travelBRetractionDistance)
{
    mTravelRetractionDistance.at(1) = travelBRetractionDistance;
}

std::vector<double> Gcode::get_travel_retraction_distance() const
{
    return mTravelRetractionDistance;
}

void   Gcode::set_material_switch_retraction_distance(double materialSwitchRetractionDistance)
{
    mMaterialSwitchRetractionDistance = materialSwitchRetractionDistance;
}

double Gcode::get_material_switch_retraction_distance() const
{
    return mMaterialSwitchRetractionDistance;
}

void Gcode::set_simplify_point_list(bool const isTrue)
{
    mSimplifyPointList = isTrue;
}

bool Gcode::get_simplify_point_list() const
{
    return mSimplifyPointList;
}

void Gcode::set_extruder_choice(std::vector<bool> extruderChoice)
{
    mSingleMaterialExtruderChoice = extruderChoice;
}

std::vector<bool> Gcode::get_extruder_choice() const
{
    return mSingleMaterialExtruderChoice;
}

void Gcode::set_single_material(bool const singleMaterial)
{
    mSingleMaterial = singleMaterial;
}

bool Gcode::is_single_material() const
{
    return mSingleMaterial;
}
