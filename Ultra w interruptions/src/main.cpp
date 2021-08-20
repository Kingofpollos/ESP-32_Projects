#include <Arduino.h>
#include <HCSR.h>
#include <TF.h>

uint8_t echo = 32; 
uint8_t trig = 33;

ulong current_time, prev_time; //pull in de tiempo
uint16_t dt_ms = 20;
float dist = 0.0;
float distf = 0.0;
//Low-pass filter
float a[4] = {1.0, -1.73472577, 0.7660066, 0};
float b[4] = {0.00782021, 0.01564042, 0.00782021, 0};

HCSR OwO;
TF filt_OwO;

void IRAM_ATTR ISR_OwO() { //From the object
   OwO.ISR();
}

void setup() {
  Serial.begin(115200);
  OwO.setup(trig, echo);
  attachInterrupt(echo, ISR_OwO , CHANGE); 
  filt_OwO.setup(a,b,0);
  prev_time = millis();
}

void loop() {
  current_time = millis();
  if(current_time - prev_time >= dt_ms)
  {
    prev_time = current_time;
    prev_time = millis();
    OwO.Trigger();
    dist = OwO.Dist();
    distf = filt_OwO.getValue(dist);
    Serial.printf("%.3f,%.3f\n",dist, distf);
    
  }
}
