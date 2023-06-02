#pragma once

#include "Constants.h"
#include "auton/Auton.h"

#include <frc/DriverStation.h>
#include <frc/TimedRobot.h>
#include <rev/CANSparkMax.h>
#include <rev/SparkMaxPIDController.h>
#include <rev/CANEncoder.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/DutyCycle.h>
#include <frc/DigitalInput.h>

struct EndEffectorData
{
    
};

class EndEffector
{
public:
    void RobotInit(const RobotData &robotData);
    void RobotPeriodic(const RobotData &robotData, EndEffectorData &endEffectorData);
private:
    void SetEndEffectorRollerSpeed(double rollerSpeed);
    void SemiAuto(const RobotData &robotData, EndEffectorData &endEffectorData);

    rev::CANSparkMax endEffectorTopRollers = rev::CANSparkMax(endEffectorTopRollerID, rev::CANSparkMax::MotorType::kBrushless);
    rev::CANSparkMax endEffectorBottomRollers = rev::CANSparkMax(endEffectorBottomRollerID, rev::CANSparkMax::MotorType::kBrushless);
    
};