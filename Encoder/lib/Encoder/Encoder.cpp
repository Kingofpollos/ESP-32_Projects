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
    pinMode(_A, INPUT);//va en el objeto
    pinMode(_Be, INPUT);//va en el objeto
    t_1 = micros();
    t_2 = micros()+1;
}

void IRAM_ATTR Encoder::ISR() 
{
    //Se activa cuando hay un cambio de señal LOW-HIGH
    t_2 = micros();
    dt = t_2 - t_1;
    t_1 = t_2;
}

float Encoder::getAngSpeed() 
{
    return (_Ang/dt) * 1000000;
}

float Encoder::getRevSpeed() 
{
    return (_Ang/dt)*1000000/360;
}

//Se puede si configuramos la interrupción con flanco de subida