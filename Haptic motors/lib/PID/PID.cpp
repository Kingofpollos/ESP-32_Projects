#include "PID.h"
PID::PID(/* args */) 
{
    
}

PID::~PID() 
{
    
}

void PID::setup(float Kp, float Ki, float Kd, uint16_t T) 
{
//T es en ms
 _Kp = Kp;
 _Ki = Ki;
 _Kd = Kd;
 _T = T;  
 a = _Kp + _Kd;
 b = _Ki*T/1000.0 - _Kp - 2*_Kd;

}

float PID::apply(float ref, float sens_read) 
{
    err = ref - sens_read; //Puede ser input o output
    output = output_1 + err*a + err_1*b + err_2*_Kd;
    output_1 = output;
    err_2 = err_1;
    err_1 = err;
    return output;
}

float PID::applywLim(float ref, float sens_read, float lim_inf, float lim_sup) 
{
    err = ref - sens_read; //Puede ser input o output
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

float PID::applywLims(float ref, float sens_read, float lim_inf, float lim_sup, float err_a) 
{
    err = ref - sens_read; //Puede ser input o output
    if (abs(err) <= err_a)
    {
        err = 0.0f;
        output_1 = 0.0f;
    }
    
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

float PID::applyHapticwLimsM(float pos_m, float pos_f, float lim_inf, float lim_sup, float err_a) 
{
    output = _Kp*(pos_f-pos_m) + _Kd*(pos_f_1-pos_m_1);
    pos_m_1 = pos_m;
    pos_f_1 = pos_f;
    output_1 = output;
    return output;
}

float PID::applyHapticwLimsF(float pos_m, float pos_f, float lim_inf, float lim_sup, float err_a) 
{
    output = _Kp*(pos_m-pos_f) + _Kd*(pos_m_1-pos_f_1);
    pos_m_1 = pos_m;
    pos_f_1 = pos_f;
    output_1 = output;
    return output;
}

void PID::reset() 
{
    err = 0.0;
    err_1 = 0.0;
    err_2 = 0.0;
    output = 0;
    output_1 = 0.0;
}