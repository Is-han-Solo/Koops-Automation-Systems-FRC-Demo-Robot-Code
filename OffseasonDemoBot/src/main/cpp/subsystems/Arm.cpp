#include "RobotData.h"

void Arm::RobotInit(const RobotData &robotData, ArmData &armData)
{
    armMotor.RestoreFactoryDefaults();
    armMotor.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
    armMotor.SetInverted(false); // change this
    armMotor.EnableVoltageCompensation(10.5);
    armMotor.SetSmartCurrentLimit(35);
    armMotor.BurnFlash();
}

void Arm::RobotPeriodic(const RobotData &robotData, ArmData &armData)
{
    switch (robotData.controlData.mode) 
    {
        case MODE_ARM_DOWN:
        case MODE_ARM_UP:
        case MODE_TELEOP_SA:
            SemiAuto(robotData, armData);
            break;
        default:
            SemiAuto(robotData, armData);
            break;
    }
}

void Arm::SemiAuto(const RobotData &robotData, ArmData &armData)
{
    if (robotData.controlData.mode == MODE_ARM_DOWN)
    {
        if (!bottomLimitSwitch.Get())
        {
            armMotor.Set(-0.1); // change this
        }
        else
        {
            armMotor.Set(0);
        }
    }
    else if (robotData.controlData.mode == MODE_ARM_UP)
    {
        if (!topLimitSwitch.Get())
        {
            armMotor.Set(0.1); // change this
        }
        else
        {
            armMotor.Set(0);
        }
    }
}
