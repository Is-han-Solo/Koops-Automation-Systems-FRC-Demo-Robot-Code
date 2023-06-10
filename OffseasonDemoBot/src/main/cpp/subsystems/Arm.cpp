#include "RobotData.h"

void Arm::RobotInit(const RobotData &robotData, ArmData &armData)
{
    armMotor.RestoreFactoryDefaults();
    armMotor.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
    armMotor.SetInverted(false); // change this
    armMotor.EnableVoltageCompensation(10.5);
    armMotor.SetSmartCurrentLimit(35);

    armPIDController.SetP(0.2, 0); // change this
    armPIDController.SetOutputRange(-1,1);
    armRelativeEncoder.SetPositionConversionFactor(1);

    armMotor.BurnFlash();
}

void Arm::RobotPeriodic(const RobotData &robotData, ArmData &armData)
{
    switch (robotData.controlData.mode) 
    {
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
    if (!softLimitsEnabled)
    {
        EnableSoftLimits();
    }

    pastArmDownCommand = currentArmDownCommand;
    currentArmDownCommand = robotData.controlData.armDown;

    pastArmUpCommand = currentArmUpCommand;
    currentArmUpCommand = robotData.controlData.armUp;

    if (currentArmUpCommand && (currentArmUpCommand != pastArmUpCommand))
    {
        MoveArm(armData.armDownPosition, robotData, 0);
    }
    
    if (currentArmDownCommand && (currentArmDownCommand != pastArmDownCommand))
    {
        MoveArm(armData.armUpPosition, robotData, 0);
    }

    if (armProfileActive)
    {
        if (robotData.timerData.secSinceEnabled > armProfileStartTime)
        {
            units::time::second_t elapsedTime{robotData.timerData.secSinceEnabled - armProfileStartTime};
            auto setpoint = armProfile.Calculate(elapsedTime);

            armPIDController.SetReference(setpoint.position.value(), rev::CANSparkMax::ControlType::kPosition);
            frc::SmartDashboard::PutNumber("armPos TRAP", setpoint.position.value());
            if (armProfile.IsFinished(elapsedTime))
            {
                armProfileActive = false;
                
            }
        }
    }

    if (std::abs(robotData.controllerData.sLYStick > 0.1))
    {
        DisableSoftLimits();

        armMotor.Set(robotData.controllerData.sLYStick * 0.4);
    }

    if (topLimitSwitch.Get())
    {
        armRelativeEncoder.SetPosition(0);
    }
}

void Arm::MoveArm(double targetPos, const RobotData& robotData, double timeOffset)
{
    armTimeOffset = timeOffset;
    armProfileActive = true;
    armProfileStartTime = robotData.timerData.secSinceEnabled+timeOffset;

    armProfileStartPos = armRelativeEncoder.GetPosition();
    armProfileEndPos = targetPos;

    armProfile = frc::TrapezoidProfile<units::degrees>
    {
        frc::TrapezoidProfile<units::degrees>::Constraints{260_deg_per_s, 120_deg/(1_s * 1_s)},
        frc::TrapezoidProfile<units::degrees>::State{units::angle::degree_t{armProfileEndPos}, units::angular_velocity::degrees_per_second_t{0}},
        frc::TrapezoidProfile<units::degrees>::State{units::angle::degree_t{armProfileStartPos}, units::angular_velocity::degrees_per_second_t{armRelativeEncoder.GetVelocity() * RotationsPerMinuteToDegreesPerSecond}}
    };

}

void Arm::EnableSoftLimits() 
{
    armMotor.EnableSoftLimit(rev::CANSparkMax::SoftLimitDirection::kReverse, true);
    armMotor.EnableSoftLimit(rev::CANSparkMax::SoftLimitDirection::kForward, true);

    armMotor.SetSoftLimit(rev::CANSparkMax::SoftLimitDirection::kReverse, armMinPosition + 0.05);
    armMotor.SetSoftLimit(rev::CANSparkMax::SoftLimitDirection::kForward, armMaxPosition - 0.1);
    softLimitsEnabled  = true;
}

void Arm::DisableSoftLimits() 
{
    armMotor.EnableSoftLimit(rev::CANSparkMax::SoftLimitDirection::kReverse, false);
    armMotor.EnableSoftLimit(rev::CANSparkMax::SoftLimitDirection::kForward, false);

    softLimitsEnabled  = false;
}