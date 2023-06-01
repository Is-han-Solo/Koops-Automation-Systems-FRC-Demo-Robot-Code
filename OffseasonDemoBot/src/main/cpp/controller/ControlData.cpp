#include "controller/Controller.h"
#include "RobotData.h"

// for updating states of control variables (to be accessed by other subsystems)
void Controller::updateControlData(const RobotData &robotData, const ControllerData &controllerData, ControlData &controlData)
{
    frc::SmartDashboard::PutBoolean("Semi-Auto", controlData.mode == MODE_TELEOP_SA);
    // states:
    controlData.shift = controllerData.sLBumper;
    
    switch (controllerData.sDPad) {
        case -1:
            break;
        case 0: // up
            controlData.mode = MODE_ARM_UP;
            break;
        case 90:    // right
            controlData.mode = MODE_TELEOP_SA;
            break;
        case 180:   // down
            controlData.mode = MODE_ARM_DOWN;
            break;
        case 270:   // left
            
            break;
        default:
            controlData.mode = MODE_TELEOP_SA;
            break;
            
    }

    // note: when pRShoulderSwitch is held, driving is sensitive to turning, while not held (default driving mode) driving is less sensitive to turning and good for quick staright movements and steady arcs (won't turn super easily)
    if (controlData.turnResponsive)
    {
        controlData.maxStraight = 1;
        controlData.maxTurn = 1;
    }
    else
    {
        controlData.maxStraight = 1;
        controlData.maxTurn = 0.3;
    }

    controlData.lDrive = controllerData.pLYStick;
    controlData.rDrive = controllerData.pRYStick;

    controlData.eject = (controllerData.pLTrigger > 0.3);
    controlData.intake = (controllerData.pRTrigger > 0.3);

    controlData.pivotEdgeCommunityPosition = controllerData.sXBtn;
    controlData.pivotLowPosition = controllerData.sABtn;
    controlData.pivotMidPosition = controllerData.sBBtn;
    controlData.pivotHighPosition = controllerData.sYBtn;
    controlData.pivotHomePosition = controllerData.sRBumper;

}



