#include <Arduino.h>
#include <BluetoothSerial.h>
#include <Encoder.h>
#include <ESP32_PWM.h>
#include <MessageUtilities.h>
#include <PID.h>
#include <TF.h>

//Motor
uint8_t pinmotor1 = 33;
uint8_t pinmotor2 = 34;
PWM motor;

//Encoder
Encoder uwu;
int A=14;
int B=12;
float resolucion=3.6;
float readEncod;

//Pull in de tiempo
ulong current_time, prev_time; //pull in de tiempo
uint16_t dt_ms = 25;

//PID
PID control;
float var_cont;

//Filtro
TF filt;//Pasabajas
float a[4] = {4.0,0,0,0};
float b[4] = {1.0,1.0,1.0,1.0};
float readEncodFilt;

//Comunicaciones
String senal;
const int leng = 32;
char senalc[leng];
BluetoothSerial SerialBT;
float control_input[2];//Tamaño de el array resultante


void IRAM_ATTR ISR_uwu() { //va en el objeto
   uwu.ISR();
}


void setup() {
  Serial.begin(115200);
  SerialBT.begin("Bbto es grande");
  SerialBT.setTimeout(20);
  uwu.setup(A,B,resolucion);
  attachInterrupt(A, ISR_uwu, RISING); //no va en el objeto, isr viene del objeto
  attachInterrupt(B, ISR_uwu, RISING);
  motor.setup(pinmotor1, 0, 40.0, 10);
  pinMode(pinmotor2, OUTPUT);
  control.setup(3.9547, 65.7561, 0.5, dt_ms);//3.9547, 65.7561, 0.5
  filt.setup(a,b,0);
  digitalWrite(pinmotor2, LOW);
  motor.setDuty(50);
  prev_time = millis();
  // put your setup code here, to run once:
}

void loop() {
  current_time = millis();
  if(current_time - prev_time >= dt_ms)
  {
    prev_time = current_time;
    prev_time = millis();

    senal = SerialBT.readStringUntil('\n');
    senal.toCharArray(senalc,leng);
    parseString(senalc, ",", control_input);

    digitalWrite(pinmotor2, LOW);
    readEncod=uwu.getRevSpeed();
    readEncodFilt = filt.getValue(readEncod);
    var_cont = control.applywLim(control_input[0], readEncodFilt, 0.0, 100.0);
    motor.setDuty(var_cont);
    Serial.printf(" %.1f, %.1f, %.1f\n", readEncodFilt,var_cont, control_input[0]);
    SerialBT.printf("%.2f, %.2f, %.2f\n", readEncodFilt, var_cont, readEncod);
    Serial.flush();
    
  }
  // put your main code here, to run repeatedly:
}