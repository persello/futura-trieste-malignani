#define PIN_STEP1        13   // Step out
#define PIN_STEP2        11   // Step out
#define PIN_DIR1         12   // Direction out
#define PIN_DIR2         10   // Direction out
#define PIN_ENABLE        7

#define MOTOR_STEPS     200   // Motor steps per revolution

uint32_t lastRPM1_1000;       // milliRPM
uint32_t lastRPM2_1000;       // milliRPM
uint8_t enable;               // Driver enable

void setupMotors(void) {
  //////// I/O SETTING ////////
  pinMode(PIN_STEP1, OUTPUT);
  pinMode(PIN_STEP2, OUTPUT);
  pinMode(PIN_DIR1, OUTPUT);
  pinMode(PIN_DIR2, OUTPUT);
  pinMode(PIN_ENABLE, OUTPUT);

  setSpeed1(0);
  setSpeed2(0);
}

volatile uint16_t period1 = 0;    // 1 = 10us
volatile uint16_t period2 = 0;    // same...
volatile uint16_t count1 = 0;     // ...
volatile uint16_t count2 = 0;     // ...

void updateMotors(void) {
  //////// PULSE GENERATION, NON-BLOCKING ////////
  if(count1 > period1) {
    PORTB |= (1 << PB5);
    count1 = 0;
  } else {
    PORTB &= ~(1 << PB5);
  }
  if(count2 > period2) {
    PORTB |= (1 << PB3);
    count2 = 0;
  } else {
    PORTB &= ~(1 << PB3);
  }
  // Both variables increment every 10us
  count1++;
  count2++;
}

void setSpeed1(int32_t rpm_1000) {
  lastRPM1_1000 = rpm_1000;    // Last speed value gets saved
  digitalWrite(PIN_DIR1, rpm_1000 > 0);
  digitalWrite(PIN_ENABLE, lastRPM1_1000 == 0 && lastRPM2_1000 == 0);
  // Step per minute = rpm * motorSteps * microSteps
  // Conversion from step per minute to step per 10us
  period1 = 6000000000UL / ((abs(rpm_1000) * (uint32_t)MOTOR_STEPS * 16));
}

void setSpeed2(int32_t rpm_1000) {
  lastRPM2_1000 = rpm_1000;    // Last speed value gets saved
  digitalWrite(PIN_DIR2, rpm_1000 < 0);
  digitalWrite(PIN_ENABLE, lastRPM1_1000 == 0 && lastRPM2_1000 == 0);
  // Step per minute = rpm * motorSteps * microSteps
  // Conversion from step per minute to step per 10us  
  period2 = 6000000000UL / ((abs(rpm_1000) * (uint32_t)MOTOR_STEPS * 16));
}
