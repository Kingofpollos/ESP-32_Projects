#ifndef _QUADRATUREENCODER_H
#define _QUADRATUREENCODER_H

#include <Arduino.h>
#include <FunctionalInterrupt.h>
#include <TF.h>

class QuadratureEncoder
{
public:
    QuadratureEncoder();
    ~QuadratureEncoder();
    void setup(uint8_t pinA, uint8_t pinB, float counts_per_revolution, uint16_t timeout_ms = 500, uint16_t bouncing_time_us = 20);
    float getAngle();
    float getSpeed();
    int8_t getDirection();
    void encoder_isr();
    TF filt;

private:
    void IRAM_ATTR handle();
    uint16_t _bouncing_time_us, _timeout_ms;
    uint8_t _pin[2];
    double _degrees_per_count;
    volatile float _speed;
    volatile long _counts, _counts_1;
    volatile ulong t_1, t_2, dt;

    volatile uint8_t _states;
    volatile int8_t _direction;
    const int8_t _lookup_table[16] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};

    uint8_t PIND = 0;
};

#endif // _QUADRATUREENCODER_H