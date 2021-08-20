#include "Encoder.h"


Encoder::Encoder(/* args */) 
{
    
}

Encoder::~Encoder() 
{
    
}

void Encoder::setup(uint8_t A, uint8_t Be, float Ang) 
{
    _A = A;
    _Be = Be;
    _Ang = Ang;
    pinMode(_A, INPUT);
    pinMode(_Be, INPUT);
    t_1 = micros();
    t_2 = micros()+1;
}

void IRAM_ATTR Encoder::ISR() 
{
    //This activates when the signal changes LOW-HIGH
    t_2 = micros();
    dt = t_2 - t_1;
    t_1 = t_2;
}

float Encoder::getAngSpeed() 
{
    //deg/sec
    return (_Ang/dt) * 1000000;
}

float Encoder::getRevSpeed() 
{
    //Rev/sec
    return (_Ang/dt)*1000000/360;
}
