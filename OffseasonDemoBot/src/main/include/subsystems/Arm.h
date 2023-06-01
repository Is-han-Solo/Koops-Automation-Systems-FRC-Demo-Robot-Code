#pragma once

#include <frc/DigitalInput.h>
#include <rev/CANSparkMax.h>

#include "controller/Controller.h"

struct ArmData
{

};

class Arm
{
public:
    void RobotInit(const RobotData &robotData, ArmData &armData);
    void RobotPeriodic(const RobotData &robotData, ArmData &armData);
private:
    void SemiAuto(const RobotData &robotData, ArmData &armData);

    frc::DigitalInput topLimitSwitch{0};
    frc::DigitalInput bottomLimitSwitch{1};

    rev::CANSparkMax armMotor{armMotorID, rev::CANSparkMaxLowLevel::MotorType::kBrushless};
    rev::SparkMaxRelativeEncoder armRelativeEncoder = armMotor.GetEncoder();
};