#ifndef __LEDSTRIP_H__
#define __LEDSTRIP_H__
#include <Arduino.h>
#include <ESP32_PWM.h>

PWM PODER;

class LEDstrip
{
private:
    uint8_t _pin;
    uint8_t _channel;
public:
    LEDstrip();
    ~LEDstrip();
    void setup(uint8_t pin, uint8_t channel);
    void power(float power);
};


/*
LEDstrip::LEDstrip()
{
}

LEDstrip::~LEDstrip()
{
}
*/

#endif // __LEDSTRIP_H__