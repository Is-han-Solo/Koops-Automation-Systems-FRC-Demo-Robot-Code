#include "auton/Auton.h"
#include "RobotData.h"

void Auton::RobotInit(ControlData &controlData, AutonData &autonData) {
    sendAutonSelectionChooser();

    controlData.saResetOdometry = false;
    
}

// creates pathGroup vector (list of strings that are interpretted by drivebase)
void Auton::AutonomousInit(AutonData &autonData)
{
    autonData.autonStep = -1;   // starts at -1 so that the stepper function can advance it to index 0 the first time

    // directory to deploy folder on roborio
    fs::path deployDirectory = frc::filesystem::GetDeployDirectory();

    autonData.autonRoutineName = autonChooser.GetSelected();
    fs::path autonDirectory = deployDirectory / "Autos" /  autonData.autonRoutineName;
    frc::SmartDashboard::PutString("autonDirectory", autonDirectory.string());

    std::ifstream inFile;
    inFile.open(autonDirectory.string());

    autonData.pathGroup.clear();

    if (inFile.fail()) {
    frc::SmartDashboard::PutString("fail", "failed");
    } else {
        std::string str;
        while (getline(inFile, str)) {
            frc::SmartDashboard::PutString("str", str);
            autonData.pathGroup.push_back(str);
        }
    }

    // remove newline char from all but the final line
    for (size_t i = 0; i < autonData.pathGroup.size(); i++) {
        std::string correctPathName = autonData.pathGroup[i];

        frc::SmartDashboard::PutBoolean("int bool" + std::to_string(i), correctPathName[correctPathName.length() - 1] == 13);
        frc::SmartDashboard::PutNumber(std::to_string(i) + "int", correctPathName[correctPathName.length() - 1]);

        // if the last char in the string is a newline, delete it for proper auton selection processing
        if (int(correctPathName[correctPathName.length() - 1]) == 13) {
            correctPathName = correctPathName.substr(0, correctPathName.length() - 1);  // get rid of hidden newline from file line read
        }
        
        autonData.pathGroup[i] = correctPathName;

        frc::SmartDashboard::PutString(std::to_string(i), autonData.pathGroup[i]);
        
        
        frc::SmartDashboard::PutString(std::to_string(i), autonData.pathGroup[i]);        
    }
}

void Auton::sendAutonSelectionChooser() {

    autonChooser.AddOption("potato", "potato"); // 1

    autonChooser.AddOption("SinglePlace", "SinglePlace"); // 2

    

    frc::SmartDashboard::PutData("Select Auton:", &autonChooser);
}


void Auton::AutonomousPeriodic(const RobotData &robotData, AutonData &autonData, ControlData &controlData, ControllerData &controllerData)
{

    frc::SmartDashboard::PutNumber("Auton Step", step);

    controlData.mode = MODE_TELEOP_SA;

    if (autonData.autonRoutineName == "potato") // 2 - TEST BLUE
    {
        potato(robotData, controlData, controllerData);
    }


}


void Auton::potato(const RobotData &robotData, ControlData &controlData, ControllerData &controllerData)
{
    double sec = robotData.timerData.secSinceEnabled;

    controlData.saResetOdometry = false;
}