#include "Servo.h"


void Servo::setControlMode(uint8_t mode) 
{
    _control_mode = (controlled_variable) mode;
}



float Servo::calculateControlOutput(float ref, int dt_ms) 
{
    float equisde;
    switch (_control_mode)
    {
    case NONE:
        return 0.0;
        break;
    case SPEED:
        pid.setup(0.20739/3, 0.64896/5, 0.00062752*0, dt_ms);
        return pid.applywLim(ref,encoder.getSpeed(),-100.0, 100.0);
        break;
    case POSITION:
        pid.setup(0.20739*1.5, 0.64896/1.5, 0.00062752*0, dt_ms);
        equisde = pid.applywLims(ref, encoder.getAngle(),-100.0, 100.0, 5);
        /*
        if (abs(encoder.getAngle()-ref) > 180.0)
        {
            equisde = equisde * -1;
        }
        */
        return equisde;
        break;
    case HAPTICM:
        pid.setup(0.20739*2, 0.64896/2, 0.00062752*0, dt_ms);
        return pid.applyHapticwLimsM(encoder.getAngle(), ref, -100, 100, 3);
        break;
    case HAPTICF:
        pid.setup(0.20739/3, 0.64896/2, 0.00062752*0, dt_ms);
        return pid.applyHapticwLimsF(ref, encoder.getAngle(), -100, 100, 3);
        break;
    default:
        return pid.applywLims(ref, encoder.getAngle(),-100.0, 100.0, 10);
        break;
    }
}
