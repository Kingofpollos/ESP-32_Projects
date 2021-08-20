#include <Arduino.h>
#include <ESP32_PWM.h>
#include <DHT.h>


//Front LED strip
uint8_t pin1 = 12;//33 MA1
uint8_t channel1 = 0;
PWM LED1;
//Light sensor
uint8_t pinlight = 32;

//33 DHT11 ,MB1 servo, MA1 luz delantera, 32 sensor de luz

//DHT
#define DHTPIN 33
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float temp = 0.0;
float hum = 0.0;
float temp_1 = 0.0;
float hum_1 = 0.0;

//Servomotor for the whipers
uint8_t pinServo = 14;
int servo_channelS = 1;
int res = 10;
int frec = 50;
float duty = 7.5;
PWM servo;
bool arriba = true;

//pull in de tiempo
ulong current_time, prev_time; 
uint16_t dt_ms = 20;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  LED1.setup(pin1, channel1, 50, 10);
  servo.setup(pinServo, servo_channelS, frec, 10);
  servo.setDuty(duty);
  pinMode(pinlight, INPUT);
  dht.begin();

  prev_time = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(current_time - prev_time >= dt_ms)
  {
    hum = dht.readHumidity();
    temp = dht.readTemperature();
    if (isnan(hum) || isnan(temp))
      {
        hum = hum_1;
        temp = temp_1;
      }
    else
      {
        hum_1 = hum;
        temp_1 = temp;
      }

    if (digitalRead(pinlight) == 1)
    {
      LED1.setDuty(99);
    }
    else
    {
      LED1.setDuty(0);
    }
    if ((hum >= 70) && (arriba == true) && (duty <= 12))
    {
      duty += 0.005;
    }
    else if ((hum >= 70) && (duty >= 12) && (arriba == true))
    {
      duty -= 0.005;
      arriba = false;
    }
    else if ((hum >= 70) && (arriba == false) && (duty >= 7.5))
    {
      duty -= 0.005;
    }
    else if ((hum >= 0) && (arriba == false) && (duty <= 7.5))
    {
      duty += 0.005;
      arriba = true;
    }
    else
    {
      duty = 12;
    }
    servo.setDuty(duty);
    
    
  Serial.printf("%i, %0.2f, %0.2f\n ",digitalRead(pinlight), hum, duty);
  }
}
