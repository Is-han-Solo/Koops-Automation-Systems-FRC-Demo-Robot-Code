#pragma once

#include <frc/DriverStation.h>
#include <frc/TimedRobot.h>
#include <rev/CANSparkMax.h>
#include <rev/SparkMaxPIDController.h>
#include <rev/CANEncoder.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <units/angle.h>
#include <math.h>

struct PivotData
{
    bool absoluteEncoderInitialized;

    double highPosition = 20;
    double midPosition = 15;
    double lowPosition = 12;
    double homePosition = 10;
    double edgeCommunityPosition = 21;
};

enum PivotRunMode
{
    ABSOLUTE,
    RELATIVE,
    NONE
};

class Pivot
{
public:
    void RobotInit(const RobotData &robotData, PivotData &pivotData);
    void RobotPeriodic(const RobotData &robotData, PivotData &pivotData, ControlData &controlData);
private:
    PivotRunMode pivotRunMode = ABSOLUTE;

    void SemiAuto(const RobotData &robotData, PivotData &pivotData, ControlData &controlData);

    void SetAngle(double position);

    void DisableSoftLimits();
    void EnableSoftLimits();

    void IsAbsoluteEncoderInitialized(PivotData &pivotData);

    void SeedRelativePosition(PivotData &pivotData);

    bool softLimitsEnabled = false;

    rev::CANSparkMax pivotMotor{pivotMotorID, rev::CANSparkMaxLowLevel::MotorType::kBrushless};
    rev::SparkMaxRelativeEncoder pivotRelativeEncoder = pivotMotor.GetEncoder();
    rev::SparkMaxPIDController pivotPIDController = pivotMotor.GetPIDController();
    rev::SparkMaxAbsoluteEncoder pivotAbsoluteEncoder = pivotMotor.GetAbsoluteEncoder(rev::SparkMaxAbsoluteEncoder::Type::kDutyCycle);

    double pivotMinPosition = 10;
    double pivotMaxPosition = pivotMinPosition + 40; // needs to change
    
};
