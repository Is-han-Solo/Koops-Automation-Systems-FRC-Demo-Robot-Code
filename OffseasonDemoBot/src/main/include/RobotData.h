#pragma once

// includes other files' data
#include "controller/Controller.h"
#include "common/Gyro.h"
#include "common/Timer.h"
#include "common/ConfigurationFiles.h"

#include "subsystems/Drivebase.h"

#include "auton/Auton.h"
#include "common/Limelight.h"

//could be separated into all separate files for the data *from* each subsystem
//commented out variables are not in use
struct RobotData
{
    ConfigData configData;

    ControllerData controllerData;
    ControlData controlData;
    GyroData gyroData;
    TimerData timerData;


    AutonData autonData;

    DrivebaseData drivebaseData;
    LimelightData limelightData;

    
};