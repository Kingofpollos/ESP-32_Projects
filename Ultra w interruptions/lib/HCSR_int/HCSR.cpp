#include "HCSR.h"

HCSR::HCSR(/* args */) 
{
    
}

HCSR::~HCSR() 
{
    
}


void HCSR::setup(uint8_t trig, uint8_t echo) 
{
    _trig = trig;
    _echo = echo;
    pinMode(_trig, OUTPUT);
    pinMode(_echo, INPUT);
}

void IRAM_ATTR HCSR::ISR() 
{
    if ( digitalRead(_echo) == HIGH && prev_flag == false)
   {
     t_1 = micros();
     prev_flag = true;
   }
   else if (digitalRead(_echo) == LOW && prev_flag == true)
   {
     t_2 = micros();
     dt_a = t_2 - t_1;
     prev_flag = false;
   }
}

void HCSR::Trigger() //Sends trigger signal (It takes 12ms)
{
  //This can be replaced with a PWM signal of 40Hz and a duty of 40%
  digitalWrite(_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trig, LOW);
}



float HCSR::Dist() 
{
    if (dt_a < 116 || dt_a > 23200)
    {
      dt_a = dt_p;
      dist = dt_a/58.0;
    }
    else
    {
      dt_p = dt_a;
      dist = dt_p/58.0;
    }
    return dist;
}


