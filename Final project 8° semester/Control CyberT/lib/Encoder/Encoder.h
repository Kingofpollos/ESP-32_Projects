#ifndef __ENCODER_H__
#define __ENCODER_H__
#include <Arduino.h>

class Encoder
{
private:
    uint8_t _A;
    uint8_t _Be;
    float _Ang;
    ulong t_1;
    ulong t_2;
    int dt;
public:
    Encoder(/* args */);
    ~Encoder();
    void setup(uint8_t A, uint8_t Be, float Ang);
    void IRAM_ATTR ISR();
    float getAngSpeed();
    float getRevSpeed();
};


#endif // __ENCODER_H__