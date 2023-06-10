#pragma once

#include <frc/DriverStation.h>
#include <frc/TimedRobot.h>
#include <rev/CANSparkMax.h>
#include <rev/SparkMaxPIDController.h>
#include <rev/CANEncoder.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/DutyCycle.h>
#include <frc/DigitalInput.h>
#include <frc/trajectory/TrapezoidProfile.h>
#include <frc/controller/ArmFeedforward.h>
#include <frc2/command/ProfiledPIDSubsystem.h>
#include <units/acceleration.h>
#include <units/length.h>
#include <units/time.h>
#include <units/velocity.h>
#include <units/voltage.h>
#include <units/angle.h>
#include <math.h>

#include "controller/Controller.h"

struct ArmData
{
    double armDownPosition = 10;
    double armUpPosition = 0;
};

class Arm
{
public:
    void RobotInit(const RobotData &robotData, ArmData &armData);
    void RobotPeriodic(const RobotData &robotData, ArmData &armData);
private:
    void SemiAuto(const RobotData &robotData, ArmData &armData);

    void MoveArm(double targetPos, const RobotData& robotData, double timeOffset);

    void DisableSoftLimits();
    void EnableSoftLimits();

    frc::DigitalInput topLimitSwitch{0};

    rev::CANSparkMax armMotor{armMotorID, rev::CANSparkMaxLowLevel::MotorType::kBrushless};
    rev::SparkMaxRelativeEncoder armRelativeEncoder = armMotor.GetEncoder();
    rev::SparkMaxPIDController armPIDController = armMotor.GetPIDController();

    bool armProfileActive = false;
    double armProfileStartPos = 0;
    double armProfileEndPos = 0;
    double armProfileStartTime = 0;    
    double armTimeOffset = 0;
    frc::TrapezoidProfile<units::degree> armProfile
    {
        frc::TrapezoidProfile<units::degrees>::Constraints{0_deg_per_s, 0_deg/(1_s * 1_s)},
        frc::TrapezoidProfile<units::degrees>::State{units::angle::degree_t{0}, units::angular_velocity::degrees_per_second_t{0}},
        frc::TrapezoidProfile<units::degrees>::State{units::angle::degree_t{0}, units::angular_velocity::degrees_per_second_t{0}}
    };

    double RotationsPerMinuteToDegreesPerSecond = (360.0) * (1.0 / 60.0);

    bool pastArmUpCommand = false;
    bool currentArmUpCommand = false;
    bool pastArmDownCommand = false;
    bool currentArmDownCommand = false;

    bool softLimitsEnabled = false;

    double armMinPosition = 0;
    double armMaxPosition = armMinPosition + 15;
};