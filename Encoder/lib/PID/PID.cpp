#include "PID.h"
PID::PID(/* args */) 
{
    
}

PID::~PID() 
{
    
}

void PID::setup(float Kp, float Ki, float Kd, uint16_t T) 
{
 _Kp = Kp;
 _Ki = Ki;
 _Kd = Kd;
 _T = T;  
 a = _Kp + _Kd;
 b = _Ki*T/1000.0 - _Kp - 2*_Kd;

}

float PID::apply(float ref, float sens_read) 
{
    err = ref - sens_read;
    output = output_1 + err*a + err_1*b + err_2*_Kd;
    output_1 = output;
    err_2 = err_1;
    err_1 = err;
    return output;
}

float PID::applywLim(float ref, float sens_read, float lim_inf, float lim_sup) 
{
    //This punction limits the PID to a range determined by the user
    err = ref - sens_read; 
    output = output_1 + err*a + err_1*b + err_2*_Kd;
    if (output > lim_sup)
    {
        output = lim_sup;
    }
    else if (lim_inf > output)
    {
        output = lim_inf;
    }
    
    output_1 = output;
    err_2 = err_1;
    err_1 = err;
    return output;
}
