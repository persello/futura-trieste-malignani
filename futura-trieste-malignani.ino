#include <PID_v1.h>

#include <TimerOne.h>
#include <MPU6050_tockn.h>
#include <Wire.h>

double setpoint, input, output;
uint32_t kp, ki, kd;
double offset = 0.0;
uint32_t limit = 90000;

MPU6050 mpu(Wire);
PID pid(&input, &output, &setpoint, kp, ki, kd, DIRECT);
void setup()
{
  Serial.begin(57600);
  Timer1.initialize(10);
  setupMotors();
  Wire.begin();
  mpu.begin();
  mpu.setGyroOffsets(-6.73, -0.35, -0.91);
  Timer1.attachInterrupt(update);
  setpoint = 0;
  pid.SetMode(AUTOMATIC);
  pid.SetOutputLimits(-200000, 200000);
  pid.SetSampleTime(5);
}

void loop()
{
  mpu.update();
  input = mpu.getAngleY() + offset;
  pid.Compute();
  Serial.println(mpu.getAngleY());
  if (Serial.available()) {
    switch (Serial.read()) {
      case 'p':
        kp = Serial.parseInt();
        break;
      case 'i':
        ki = Serial.parseInt();
        break;
      case 'd':
        kd = Serial.parseInt();
        break;
      case 'o':
        offset = Serial.parseFloat();
        break;
      case 'l':
        limit = Serial.parseInt();
        pid.SetOutputLimits(-limit, limit);
        break;
    }
  }
  pid.SetTunings(kp, ki, kd);
  setSpeed1(-output);
  setSpeed2(-output);
  update();
}

void update(void) {
  updateMotors();
}
