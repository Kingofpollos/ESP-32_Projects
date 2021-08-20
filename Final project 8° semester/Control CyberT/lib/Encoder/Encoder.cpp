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
    t_1 = micros()+15;
    t_2 = micros()+30;
}

void IRAM_ATTR Encoder::ISR() 
{
    //SActivates when there is a change in the signal LOW-HIGH
    t_2 = micros();
    dt = t_2 - t_1;
    t_1 = t_2;
}

float Encoder::getAngSpeed() 
{
    float atemp;
    atemp = (_Ang/dt) * 1000000.0;
    if (isnan(atemp))
    {
        atemp = 0.0f;
    }
    return atemp;
}

float Encoder::getRevSpeed() 
{
    float atemp;
    atemp = (_Ang/dt) * 1000000/360.0;
    if (isnan(atemp))
    {
        atemp = 0.0f;
    }
    return atemp;
}
