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
            controlData.mode = MODE_TELEOP_MANUAL;
            break;
        case 90:    // right
            controlData.mode = MODE_TELEOP_SA;
            break;
        case 180:   // down
            controlData.mode = MODE_TELEOP_DISABLE_BEAMS;
            break;
        case 270:   // left
            //controlData.mode = MODE_TELEOP_ADVANCED_SA;
            break;
        default:
            controlData.mode = MODE_TELEOP_SA;
            break;
            
    }
//-----------------------------------------------------------------------------------------------------------------------------------
//        DRIVEBASE / PRIMARY DRIVER BUTTONS:
//-----------------------------------------------------------------------------------------------------------------------------------

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

    controlData.dbInverted = controllerData.pLPalmSwitch;
    // if you're inverted then you swtich sides for driving so it's intuitive
    if (controlData.dbInverted)
    {
        controlData.lDrive = -controllerData.pRYStick;
        controlData.rDrive = -controllerData.pLYStick;
    }
    else
    {
        controlData.lDrive = controllerData.pLYStick;
        controlData.rDrive = controllerData.pRYStick;
    }


}



