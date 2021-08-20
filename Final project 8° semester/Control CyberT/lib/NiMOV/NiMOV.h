#ifndef _HBRIDGE_h
#define _HBRIDGE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "NiMU_ESP32.h"
#include <PWM_ESP32.h>

#define AIN1 12
#define AIN2 18
#define BIN1 14
#define BIN2 27
#define STOP 2
#define COAST 3

class NiMOV
{
public:
	PWM pwm1;
	PWM pwm2;
	NiMOV(uint8_t pin1, uint8_t channel1, uint8_t pin2, uint8_t channel2, double frequency, uint8_t bits_resolution, float max_speed );
	~NiMOV();
	float getPWM();
	void setSpeed(float raw_speed);
	void setStop(uint8_t mode);
private:
	float _max_speed;
	float _pwm;
};

#endif

