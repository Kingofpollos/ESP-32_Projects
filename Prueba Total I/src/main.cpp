#include <Arduino.h>
#include <HCSR.h>
#include <TF.h>

uint8_t echo[4] = {5, 19, 33, 32}; //Trasero, Izq, Centro, Der
uint8_t trig = 26;

ulong current_time, prev_time; //pull in de tiempo
uint16_t dt_ms = 20;
float dist_T, dist_I, dist_C,dist_D   = 0.0;

//Pasabajas
float a[4] = {1.0, -1.73472577, 0.7660066, 0};
float b[4] = {0.00782021, 0.01564042, 0.00782021, 0};
//Crear objetos de Ultra
HCSR OwO_T, OwO_I, OwO_C, OwO_D;

//Definir interrupciones
void IRAM_ATTR ISR_OwO_T() { //va en el objeto
   OwO_T.ISR();
}
void IRAM_ATTR ISR_OwO_I() { //va en el objeto
   OwO_I.ISR();
}
void IRAM_ATTR ISR_OwO_C() { //va en el objeto
   OwO_C.ISR();
}
void IRAM_ATTR ISR_OwO_D() { //va en el objeto
   OwO_D.ISR();
}

void setup() {
  Serial.begin(115200);
  OwO_T.setup(trig, echo[0]);
  OwO_I.setup(trig, echo[1]);
  OwO_C.setup(trig, echo[2]);
  OwO_D.setup(trig, echo[3]);

  attachInterrupt(echo[0], ISR_OwO_T , CHANGE);
  attachInterrupt(echo[1], ISR_OwO_I , CHANGE);
  attachInterrupt(echo[2], ISR_OwO_C , CHANGE);
  attachInterrupt(echo[3], ISR_OwO_D , CHANGE);
  prev_time = millis();
}

void loop() {
  current_time = millis();
  if(current_time - prev_time >= dt_ms)
  {
    prev_time = current_time;
    prev_time = millis();
    OwO_T.Trigger();
    dist_T= OwO_T.Dist();
    dist_I = OwO_I.Dist();
    dist_C = OwO_C.Dist();
    dist_D = OwO_D.Dist();
    Serial.printf("%.3f,%.3f,%.3f,%.3f\n",dist_T, dist_I, dist_C,dist_D);
    
  }
}