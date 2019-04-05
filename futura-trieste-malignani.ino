#include <PID_v1.h>

#include <TimerOne.h>
#include <MPU6050_tockn.h>
#include <Wire.h>

double setpoint, input, output;

MPU6050 mpu(Wire);
PID pid(&input, &output, &setpoint, 5000, 0, 600, DIRECT);
void setup()
{
  Serial.begin(115200);
  Timer1.initialize(10);
  setupMotors();
  Wire.begin();
  mpu.begin();
  mpu.setGyroOffsets(-6.73, -0.35, -0.91);
  Timer1.attachInterrupt(update);
  setpoint = 0;
  pid.SetMode(AUTOMATIC);
  pid.SetOutputLimits(-80000, 80000);
}

void loop()
{
  mpu.update();
  input = mpu.getAngleY();
  pid.Compute();
  Serial.println(output);
  setSpeed1(-output);
  setSpeed2(-output);
  update();
}

void update(void) {
  updateMotors();
}
