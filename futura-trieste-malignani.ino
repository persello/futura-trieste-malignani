#include <FastPID.h>
#include <TimerOne.h>
#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu(Wire);
FastPID PID(0.1, 0.4, 0.5, 50, 32, true);
void setup()
{
  Timer1.initialize(100);
  setupMotors();
  Serial.begin(115200);
  Wire.begin();
  mpu.begin();
  mpu.calcGyroOffsets(true);
  Timer1.attachInterrupt(updateMotors);
}

void loop()
{  
  mpu.update();
  int32_t output = PID.step(0, (int)(mpu.getAngleY()));
  setSpeed1(constrain(-output * 100, -100000, 100000));
  setSpeed2(constrain(-output * 100, -100000, 100000));
}
