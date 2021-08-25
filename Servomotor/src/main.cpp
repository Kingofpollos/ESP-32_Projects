#include <Arduino.h>
#include <BluetoothSerial.h>
#include <Servo.h>
#include <MessageUtilities.h>

BluetoothSerial SerialBT;
Servo servo;

float a[4] = {1.0, -1.73472577, 0.7660066, 0};
float b[4] = {0.00782021, 0.01564042, 0.00782021, 0};

ulong current_time, prev_time;
uint16_t dt_ms = 10;

float i = 0;
float duty = 0;

void IRAM_ATTR ISR_encod()
{
  servo.encoder.encoder_isr();
}

void setup()
{
  SerialBT.begin("ESP-32");
  Serial.begin(115200);
  servo.encoder.setup(19, 5, 979.62f, 200);
  servo.driver.setup(14, 27, 0, 1);
  servo.encoder.filt.setup(a, b, 0);
  /*
  With the codes below we can choose between control the speed or the position of the motor
  */
  //servo.setControlMode(SPEED);
  servo.setControlMode(POSITION);

  attachInterrupt(19, ISR_encod, CHANGE);
  attachInterrupt(5, ISR_encod, CHANGE);
  prev_time = millis();
}

void loop()
{
  /*
  To control this motor is required a BT connection, and the message sent is a float that is the reference
  */
  current_time = millis();
  if (current_time - prev_time > dt_ms)
  {
    prev_time = current_time;
    char message_out[64];
    if (SerialBT.available())
    {
      float data_in[6];
      char string_in[64];
      SerialBT.readStringUntil('\n').toCharArray(string_in, sizeof(string_in));
      parseString(string_in, ",", data_in);
      servo.setControlMode(data_in[0]);
      duty = data_in[1]; //duty is the reference sent by the user
    }

    float control_output = servo.calculateControlOutput(duty, dt_ms);
    servo.driver.setSpeed(control_output);

    sprintf(message_out, "%.2f,%.2f,%.2f,%.2f\r\n", duty, servo.encoder.getAngle(), servo.encoder.getSpeed(), control_output);//Return the value sent to the controller,
    //The position in °, the speed in °/sec and the output of the control
    SerialBT.print(message_out);
    Serial.print(message_out);
  }
}
