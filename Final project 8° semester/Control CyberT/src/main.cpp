#include <Arduino.h>
#include <BluetoothSerial.h>
#include <ESP32_PWM.h>
#include <MessageUtilities.h>
#include <HCSR.h>
#include <MPU6050_tockn.h>
#include <Wire.h>
#include <Encoder.h>
#include <TF.h>
#include <PID.h>

//Servomotor
int pin_servo = 26; //b1
int servo_channelS = 0;
int res = 10;
int frec = 50;
PWM servo; //Operates between 5 and 10%, where 5% is 0°, and 10% is 180°

//Main motor
int pin_motor = 23; //b6
int servo_channelM = 1;
PWM motor;

//Ultrasonicos
uint8_t echo[4] = {5, 19, 33, 32}; //Back, Left, Center, Right (b7, b1, b2, b3)
uint8_t trig = 25; //b2
float dist_T, dist_I, dist_C,dist_D   = 0.0;
HCSR OwO_T, OwO_I, OwO_C, OwO_D;
PWM OWOtrig;
int channelOWO = 2;
int frecOWO = 40;

//Comunications
String senal;
const int leng = 32;
char senalc[leng];
BluetoothSerial SerialBT;
//Control motor, servo, leftled, rightled, tokyo, manual or auto, vel estable
float control_input[7];//Size of the resultant array

//pull in de tiempo
ulong current_time, prev_time; 
uint16_t dt_ms = 20; //20

//Inertial sensor
MPU6050 mpu6050(Wire);
float acex = 0.0;

//Luces
uint8_t PinLeftLED = 12; //MA1
uint8_t PinRightLED = 18; //MA2
uint8_t PinTOKYO = 14; //MB1

//Encoder
Encoder Encod;
int A=4; 
int B=13;
float Ang=3.6;
float readEncod = 0.0;

//Filters
TF FiltEncod;
float a[4] = {1.0, -1.73472577, 0.7660066, 0};
float b[4] = {0.00782021, 0.01564042, 0.00782021, 0};
float readEncodFilt = 0.0;

//PID
PID controlVel;
float var_cont = 0.0;
float PID_ref;
float Kp = 3.9547;
float Ki = 85.7561;
float Kd = 0.5;
bool PIDflag = false;

//Define interruptions
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
void IRAM_ATTR ISR_Encod() { //va en el objeto
   Encod.ISR();
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  SerialBT.begin("Bbto es pvto");
  SerialBT.setTimeout(20);

  servo.setup(pin_servo, servo_channelS, frec, res);
  motor.setup(pin_motor, servo_channelM, frec, res);
  motor.setDuty(7.53716);
  servo.setDuty(7.53716);

  OwO_T.setup(trig, echo[0]);
  OwO_I.setup(trig, echo[1]);
  OwO_C.setup(trig, echo[2]);
  OwO_D.setup(trig, echo[3]);
  OWOtrig.setup(trig, channelOWO, frecOWO, res);

  attachInterrupt(echo[0], ISR_OwO_T , CHANGE);
  attachInterrupt(echo[1], ISR_OwO_I , CHANGE);
  attachInterrupt(echo[2], ISR_OwO_C , CHANGE);
  attachInterrupt(echo[3], ISR_OwO_D , CHANGE);
  OWOtrig.setDuty(40);

  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  pinMode(PinLeftLED, OUTPUT);
  pinMode(PinRightLED, OUTPUT);
  pinMode(PinTOKYO, OUTPUT);

  Encod.setup(A, B, Ang);
  FiltEncod.setup(a,b,0);
  attachInterrupt(A, ISR_Encod, RISING);
  attachInterrupt(B, ISR_Encod, RISING);

  controlVel.setup(Kp, Ki, Kd, dt_ms);
  prev_time = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  current_time = millis();
  if(current_time - prev_time >= dt_ms)
  {
    if (SerialBT.connected()) 
    {
      prev_time = current_time;
      prev_time = millis();

      mpu6050.update();


      senal = SerialBT.readStringUntil('\n');
      senal.toCharArray(senalc,leng);
      parseString(senalc, ",", control_input);

      servo.setDuty(5.0 + control_input[0]);

      readEncod = Encod.getRevSpeed();
      if (isnan(readEncod)){readEncod = 0.0f;}
      readEncodFilt = FiltEncod.getValue(readEncod);
      if (isnan(readEncodFilt)){readEncodFilt = 0.0f;}
      if (control_input[6] == 1 && PIDflag == false){PID_ref = readEncodFilt;PIDflag = true;}
      else if (control_input[6] == 1 && PIDflag == false && control_input[5] == 1)
      {//Activates if it's in automatic control
        PID_ref = 4.085; //(1 km/hr = 4.085)
        PIDflag = true;
      }
      else if (control_input[6] == 1 && PIDflag == true){control_input[1] = controlVel.applywLim(PID_ref, readEncodFilt, 3.2149, 3.214965);}//3.216
      else if (control_input[6] != 1 && PIDflag == true){PIDflag = false;controlVel.reset();}

      motor.setDuty(5.0 + control_input[1]);
      digitalWrite(PinLeftLED,control_input[2]);
      digitalWrite(PinRightLED,control_input[3]);
      digitalWrite(PinTOKYO,control_input[4]);
      if (control_input[5] == 1){dt_ms = 80;}
      else{dt_ms = 20;}
      

      dist_T= OwO_T.Dist();
      dist_I = OwO_I.Dist();
      dist_C = OwO_C.Dist();
      dist_D = OwO_D.Dist();

      acex = mpu6050.getRawAccX();
      

      SerialBT.printf("%.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n",dist_T, dist_I, dist_C, dist_D, acex, readEncodFilt);
      Serial.printf("%.2f, %.2f, %i, %i, %i -- %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n",control_input[0], control_input[1], control_input[2], control_input[3], control_input[4],dist_T, dist_I, dist_C,dist_D, acex, readEncodFilt);
      Serial.flush();
      SerialBT.flush();
    }
    else
    {
      servo.setDuty(7.53716);
      Serial.printf("No hay conexión, %i\n", SerialBT.available());
      SerialBT.println("Sijala");
    }
    Serial.flush();
    SerialBT.flush();
  }
}
