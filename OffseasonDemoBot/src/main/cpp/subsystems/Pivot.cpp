#include "RobotData.h"

void Pivot::RobotInit(const RobotData &robotData, PivotData &pivotData)
{
    pivotMotor.RestoreFactoryDefaults();

    pivotAbsoluteEncoder.SetInverted(false); // change this if needed
    pivotAbsoluteEncoder.SetPositionConversionFactor(1); // change this
    pivotAbsoluteEncoder.SetZeroOffset(0); // change this 

    pivotMotor.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
    pivotMotor.SetInverted(false); // change this
    pivotMotor.EnableVoltageCompensation(10.5);
    pivotMotor.SetSmartCurrentLimit(40);

    pivotPIDController.SetP(0.2, 0); // change this
    pivotPIDController.SetOutputRange(-1,1);
    pivotRelativeEncoder.SetPositionConversionFactor(1); // change this

    pivotMotor.BurnFlash();

    pivotRelativeEncoder.SetPosition(10);

    pivotPIDController.SetFeedbackDevice(pivotAbsoluteEncoder);

}

void Pivot::RobotPeriodic(const RobotData &robotData, PivotData &pivotData, ControlData &controlData)
{
    switch (robotData.controlData.mode) 
    {
        case MODE_TELEOP_SA:
            SemiAuto(robotData, pivotData, controlData);
            break;
        default:
            SemiAuto(robotData, pivotData, controlData);
            break;
    }

    IsAbsoluteEncoderInitialized(pivotData);

    if (pivotRelativeEncoder.GetVelocity() < 0.1)
    {
        SeedRelativePosition(pivotData);
    }

    frc::SmartDashboard::PutNumber("Pivot RELATIVE Position", pivotRelativeEncoder.GetPosition());
    frc::SmartDashboard::PutNumber("Pivot ABSOLUTE Position", pivotAbsoluteEncoder.GetPosition());

    pivotData.pastCubeRead = pivotData.isThereCube;
    pivotData.isThereCube = cubeLimitSwitch.Get();
}

void Pivot::SemiAuto(const RobotData &robotData, PivotData &pivotData, ControlData &controlData)
{
    if (!softLimitsEnabled) 
    {
        EnableSoftLimits();
    }

    if (pivotData.absoluteEncoderInitialized && pivotRunMode != ABSOLUTE)
    {
        pivotRunMode = ABSOLUTE;
        pivotPIDController.SetFeedbackDevice(pivotAbsoluteEncoder);
    }

    if (!pivotData.absoluteEncoderInitialized && pivotRunMode != RELATIVE)
    {
        pivotRunMode = RELATIVE;
        pivotPIDController.SetFeedbackDevice(pivotRelativeEncoder);
    }

    if ((pivotRunMode == ABSOLUTE && robotData.pivotData.absoluteEncoderInitialized) || (pivotRunMode == RELATIVE))
    {

        if (robotData.controlData.intake)
        {
            SetAngle(robotData.pivotData.intakePosition);
        }

        if (robotData.controlData.pivotHighPosition)
        {
            SetAngle(robotData.pivotData.highPosition);
        }
        else if (robotData.controlData.pivotMidPosition)
        {
            SetAngle(robotData.pivotData.midPosition);
        }
        else if (robotData.controlData.pivotLowPosition)
        {
            SetAngle(robotData.pivotData.lowPosition);
        }
        else if (robotData.controlData.pivotEdgeCommunityPosition)
        {
            SetAngle(robotData.pivotData.edgeCommunityPosition);
        }
        else if (robotData.controlData.pivotHomePosition)
        {
            SetAngle(robotData.pivotData.homePosition);
        }

        if (((!robotData.pivotData.isThereCube) && (robotData.pivotData.pastCubeRead)) ||
            ((robotData.pivotData.isThereCube) && (!robotData.pivotData.pastCubeRead)))
        {
            SetAngle(robotData.pivotData.homePosition);
        }
    }
}

void Pivot::SetAngle(double position)
{
    pivotPIDController.SetReference(position, rev::CANSparkMax::ControlType::kPosition);
}

void Pivot::EnableSoftLimits() 
{
    pivotMotor.EnableSoftLimit(rev::CANSparkMax::SoftLimitDirection::kReverse, true);
    pivotMotor.EnableSoftLimit(rev::CANSparkMax::SoftLimitDirection::kForward, true);

    pivotMotor.SetSoftLimit(rev::CANSparkMax::SoftLimitDirection::kReverse, pivotMinPosition + 0.05);
    pivotMotor.SetSoftLimit(rev::CANSparkMax::SoftLimitDirection::kForward, pivotMaxPosition - 0.1);
    softLimitsEnabled  = true;
}

void Pivot::DisableSoftLimits() 
{
    pivotMotor.EnableSoftLimit(rev::CANSparkMax::SoftLimitDirection::kReverse, false);
    pivotMotor.EnableSoftLimit(rev::CANSparkMax::SoftLimitDirection::kForward, false);

    softLimitsEnabled  = false;
}

void Pivot::IsAbsoluteEncoderInitialized(PivotData &pivotData)
{
    if (pivotAbsoluteEncoder.GetPosition() >= 0.01)
    {
        pivotData.absoluteEncoderInitialized = true;
    }
    else 
    {
        pivotData.absoluteEncoderInitialized = false;
        pivotRunMode = NONE;
    }
}

void Pivot::SeedRelativePosition(PivotData &pivotData)
{
    if (pivotData.absoluteEncoderInitialized)
    {
        pivotRelativeEncoder.SetPosition(pivotAbsoluteEncoder.GetPosition());
    }
}