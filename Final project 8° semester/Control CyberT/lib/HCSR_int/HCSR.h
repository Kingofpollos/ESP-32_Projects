#ifndef __HCSR_H__
#define __HCSR_H__
#include <Arduino.h>

class HCSR
{
private:
    ulong t_1 = 0; //va en el objeto
    ulong t_2 = 0; //va en el objeto
    //int dt_a = 0;
    int dt_p = 0; //va en el objeto
    float dist = 0.0; //va en el objeto
    bool prev_flag = false; //va en el objeto
    uint8_t _trig;
    uint8_t _echo;
public:
    HCSR(/* args */);
    ~HCSR();
    void setup(uint8_t trig, uint8_t echo);
    void IRAM_ATTR ISR();
    void Trigger();
    float Dist();
    int dt_a = 0;
};

#endif // __HCSR_H__