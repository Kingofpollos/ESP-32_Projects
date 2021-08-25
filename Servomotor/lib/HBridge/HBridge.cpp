#include "HBridge.h"


void HBridge::setup(uint8_t inA, uint8_t inB, uint8_t pwm_chA, uint8_t pwm_chB, float duty_threshold, double frequency, uint8_t bits_resolution) 
{
    in[0].setup(inA, pwm_chA, frequency, bits_resolution);
    in[1].setup(inB, pwm_chB, frequency, bits_resolution);
    _b = duty_threshold;
    _m = (100.0f - duty_threshold) / 100.0f;
}

void HBridge::setSpeed(float speed) 
{
    float sp;
    if (speed > 0)
    {
        sp = _m * speed + _b;
        in[0].setDuty(sp);
        in[1].setDuty(0);
    }
    else if (speed < 0)
    {
        sp = _m * -speed + _b;
        in[0].setDuty(0);
        in[1].setDuty(sp);
    }
    else
    {
        in[0].setDuty(0);
        in[1].setDuty(0);
    }
    
    
}

void HBridge::setStop(uint8_t mode) 
{
    switch (mode)
    {
    case BRAKE:
        in[0].setDuty(100);
        in[1].setDuty(100);
        break;
    
    default:
        in[0].setDuty(0);
        in[1].setDuty(0);
        break;
    }
}

void HBridge::setFrequency(float frequency) 
{
    in[0].setFrequency(frequency);
    in[1].setFrequency(frequency);
}

HBridge::HBridge() 
{
    
}

HBridge::~HBridge() 
{
    
}
