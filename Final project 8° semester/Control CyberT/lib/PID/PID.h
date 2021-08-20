#ifndef __PID_H__
#define __PID_H__
#include <Arduino.h>

class PID
{
private:
    float _Kp;
    float _Ki;
    float _Kd;
    uint16_t _T;
    float a;
    float b;
    float err = 0.0;
    float err_1 = 0.0;
    float err_2 = 0.0;
    float output = 0;
    float output_1 = 0.0;

public:
    PID(/* args */);
    ~PID();
    void setup(float Kp, float Ki, float Kd, uint16_t T);
    float apply(float ref, float sens_read);
    float applywLim(float ref, float sens_read, float lim_inf, float lim_sup);
    void reset();
};
/*
PID::PID()
{
}

PID::~PID()
{
}
*/

#endif // __PID_H__