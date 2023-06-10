#include "subsystems/EndEffector.h"
#include "RobotData.h"
#include <cmath>

void EndEffector::RobotInit(const RobotData &robotData)
{
    endEffectorTopRollers.RestoreFactoryDefaults();
    endEffectorTopRollers.SetInverted(true);
    endEffectorTopRollers.SetIdleMode(rev::CANSparkMax::IdleMode::kCoast);
    endEffectorTopRollers.SetSmartCurrentLimit(30);
    endEffectorTopRollers.EnableVoltageCompensation(10.5);

    endEffectorTopRollers.BurnFlash();

    endEffectorBottomRollers.RestoreFactoryDefaults();
    endEffectorBottomRollers.SetInverted(false);
    endEffectorBottomRollers.SetIdleMode(rev::CANSparkMax::IdleMode::kCoast);
    endEffectorBottomRollers.SetSmartCurrentLimit(30);
    endEffectorBottomRollers.EnableVoltageCompensation(10.5);

    endEffectorBottomRollers.BurnFlash();
}

void EndEffector::RobotPeriodic(const RobotData &robotData, EndEffectorData &endEffectorData)
{
    switch (robotData.controlData.mode) 
    {
        case MODE_TELEOP_SA:
            SemiAuto(robotData, endEffectorData);
            break;
        default:
            SemiAuto(robotData, endEffectorData);
            break;
    }
}

void EndEffector::SemiAuto(const RobotData &robotData, EndEffectorData &endEffectorData)
{
    if (robotData.controlData.intake && !robotData.pivotData.isThereCube)
    {
        SetEndEffectorRollerSpeed(0.5);
    } 
    else if (robotData.controlData.eject)
    {
        SetEndEffectorRollerSpeed(-0.5);
    }
}

void EndEffector::SetEndEffectorRollerSpeed(double rollerSpeed)
{
    endEffectorTopRollers.Set(rollerSpeed);
    endEffectorBottomRollers.Set(rollerSpeed);
}