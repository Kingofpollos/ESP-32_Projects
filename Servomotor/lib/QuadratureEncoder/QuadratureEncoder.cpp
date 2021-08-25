#include "QuadratureEncoder.h"


void QuadratureEncoder::setup(uint8_t pinA, uint8_t pinB, float counts_per_revolution, uint16_t timeout_ms, uint16_t bouncing_time_us) 
{
    _pin[0] = pinA;
    _pin[1] = pinB;
    _degrees_per_count = 360.0/counts_per_revolution;
    _bouncing_time_us = bouncing_time_us;
    _timeout_ms = timeout_ms;
    pinMode(_pin[0], INPUT);//va en el objeto
    pinMode(_pin[1], INPUT);//va en el objeto
    t_1 = micros();
    t_2 = micros()+30;
}

float QuadratureEncoder::getAngle() 
{
    float deg;
    deg = _degrees_per_count * _counts;
    /*
    if (deg > 360.0)
    {
        deg -= 360.0;
        _counts = 0;
    }
    if (deg < -360)
    {
         deg += 360.0;
        _counts = _degrees_per_count/360.0;
           
    }
    */
    
    return deg; //regresa angulo xd
}

float QuadratureEncoder::getSpeed() 
{
    /*Regresa velocidad angular en °/seg*/
    float atemp;
    atemp = (_degrees_per_count/dt) * 1000000.0;
    if (isnan(atemp) || (micros()-t_1 > 30000))
    {
        atemp = 0.0f;
    }
    return filt.getValue(atemp * getDirection());
}

int8_t QuadratureEncoder::getDirection() 
{
    if (_counts_1 < _counts)
    {
        return 1;
    }
    else if (_counts_1 > _counts)
    {
        return -1;
    }
    else
    {
        return 0;
    }
    
}

void QuadratureEncoder::encoder_isr() 
{
    t_2 = micros();
    dt = t_2 - t_1;
    t_1 = t_2;

    static uint8_t enc_val = 0;
    
    enc_val = enc_val << 2;

    bitWrite(PIND, 2, digitalRead(_pin[0]));
    bitWrite(PIND, 3, digitalRead(_pin[1]));
    

    enc_val = enc_val | ((PIND & 0b1100) >> 2); //preguntar equivalente PIND

    _counts_1 = _counts;
    _counts = _counts + _lookup_table[enc_val & 0b1111];
}

QuadratureEncoder::QuadratureEncoder() 
{
    
}

QuadratureEncoder::~QuadratureEncoder() 
{
    
}
