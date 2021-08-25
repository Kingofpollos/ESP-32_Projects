#include <Arduino.h>
#include <BluetoothSerial.h>
#include <Servo.h>
#include <MessageUtilities.h>

BluetoothSerial SerialBT;
Servo servo;
Servo servo2;
float pos_servo2;

float a[4] = {1.0, -1.73472577, 0.7660066, 0};
float b[4] = {0.00782021, 0.01564042, 0.00782021, 0};

ulong current_time, prev_time;
uint16_t dt_ms = 1000;

float i = 0;
float duty = 0;

void IRAM_ATTR ISR_encod()
{
  servo.encoder.encoder_isr();
}
void IRAM_ATTR ISR_encod2()
{
  servo2.encoder.encoder_isr();
}

void setup()
{
  SerialBT.begin("ESP-32");
  Serial.begin(115200);
  servo.encoder.setup(19, 5, 2248.86f, 200);
  servo2.encoder.setup(25, 26, 1632.67f, 200);//Second servomotor
  servo.driver.setup(14, 27, 0, 1, 0);
  servo2.driver.setup(32, 34, 2, 3,0);//Second servomotor
  servo.encoder.filt.setup(a, b, 0);
  servo2.encoder.filt.setup(a, b, 0);//Second servomotor
  //servo.setControlMode(SPEED);
  servo.setControlMode(HAPTICM);
  servo2.setControlMode(HAPTICM);

  attachInterrupt(19, ISR_encod, CHANGE);
  attachInterrupt(5, ISR_encod, CHANGE);
  attachInterrupt(26, ISR_encod2, CHANGE);
  attachInterrupt(25, ISR_encod2, CHANGE);
  prev_time = micros();
}

void loop()
{
  current_time = micros();
  if (current_time - prev_time > dt_ms)
  {
    prev_time = current_time;
    char message_out[64];

    float control_output = servo.calculateControlOutput(servo2.encoder.getAngle(), dt_ms/1000);
    servo.driver.setSpeed(control_output);

    float control_output2 = servo2.calculateControlOutput(servo.encoder.getAngle(), dt_ms/1000);
    servo2.driver.setSpeed(control_output2);
    
    sprintf(message_out, "%.2f,%.2f,        %.2f,%.2f,        %.2f,%.2f\r\n", servo2.encoder.getAngle(), servo.encoder.getAngle(), servo.encoder.getSpeed(), servo2.encoder.getSpeed(), control_output, control_output2);
    SerialBT.print(message_out);
    Serial.print(message_out);
  }
}
