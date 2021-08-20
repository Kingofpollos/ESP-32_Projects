#include "LEDstrip.h"
LEDstrip::LEDstrip() 
{
    
}

LEDstrip::~LEDstrip() 
{
    
}

void LEDstrip::setup(uint8_t pin, uint8_t channel) 
{
    _pin = pin;
    _channel = channel;
    PODER.setup(_pin, _channel, 50, 10);
}

void LEDstrip::power(float power) 
{
    PODER.setDuty(power);
}

