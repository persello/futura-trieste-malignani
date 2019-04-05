#include <MPU6050_tockn.h>
#include <Wire.h>
#include <AutoPID.h>

double PIDOut;
double setpoint = 0;
double input = 0;
MPU6050 mpu(Wire);
AutoPID PID(&input, &setpoint, &PIDOut, -100000.0, 100000.0, 1000, 0, 0);

void setup()
{
  setupMotors();
  Serial.begin(115200);
  Wire.begin();
  mpu.begin();
  mpu.calcGyroOffsets(true);
}

void loop()
{
  mpu.update();
  input = mpu.getAngleY();
  PID.run();
  setSpeed1(PIDOut);
  setSpeed2(PIDOut);
  updateMotors();
}
