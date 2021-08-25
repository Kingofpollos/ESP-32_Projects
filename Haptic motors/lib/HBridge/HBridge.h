#ifndef _HBRIDGE_H
#define _HBRIDGE_H

#include <ESP32_PWM.h>

#define BRAKE 0
#define COAST 1

class HBridge
{
public:
    HBridge();
    ~HBridge();
    PWM in[2];
    void setup(uint8_t inA, uint8_t inB, uint8_t pwm_chA, uint8_t pwm_chB, float duty_threshold = 10.0f, double frequency = 20000.0f, uint8_t bits_resolution = 10);
    void setSpeed(float speed);
    void setStop(uint8_t mode);
    void setFrequency(float frequency);

private:
    float _m, _b;
};

#endif // _HBRIDGE_H