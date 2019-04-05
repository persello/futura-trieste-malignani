#define PIN_STEP1        13   // Step out
#define PIN_STEP2        11   // Step out
#define PIN_DIR1         12   // Direction out
#define PIN_DIR2         10   // Direction out
#define PIN_ENABLE        7

#define MOTOR_STEPS     200   // Step per revolution del motore

uint32_t period1;             // us
uint32_t period2;             // us
uint16_t lastRPM1_1000;       // milliRPM
uint16_t lastRPM2_1000;       // milliRPM
uint8_t enable;               // Driver enable

void setup() {
  //////// IMPOSTAZIONE I/O ////////
  pinMode(PIN_STEP1, OUTPUT);
  pinMode(PIN_STEP2, OUTPUT);
  pinMode(PIN_DIR1, OUTPUT);
  pinMode(PIN_DIR2, OUTPUT);
  pinMode(PIN_ENABLE, OUTPUT);

  setSpeed1(100000);
  setSpeed2(100000);
  Serial.begin(115200);
}

void loop() {
  //////// GENERAZIONE ONDA QUADRA STEP OUT, NON BLOCCANTE, DUTY CYCLE 0.5 ////////
  digitalWrite(PIN_STEP1, (micros() % period1) > (period1 / 2));
  digitalWrite(PIN_STEP2, (micros() % period2) > (period2 / 2));
}

void setSpeed1(int32_t rpm_1000) {
  // Passi al minuto = rpm * stepsMotore * microSteps
  lastRPM1_1000 = rpm_1000;    // Salvo ultimo valore di velocita in caso di ricalcolo
  digitalWrite(PIN_DIR1, rpm_1000 > 0);
  digitalWrite(PIN_ENABLE, lastRPM1_1000 == 0 && lastRPM2_1000 == 0);
  period1 = 60000000000UL / (abs(rpm_1000) * (uint32_t)MOTOR_STEPS * 16);
}

void setSpeed2(int32_t rpm_1000) {
  // Passi al minuto = rpm * stepsMotore * microSteps
  lastRPM2_1000 = rpm_1000;    // Salvo ultimo valore di velocita in caso di ricalcolo
  digitalWrite(PIN_DIR2, rpm_1000 < 0);
  digitalWrite(PIN_ENABLE, lastRPM1_1000 == 0 && lastRPM2_1000 == 0);
  period2 = 60000000000UL / (abs(rpm_1000) * (uint32_t)MOTOR_STEPS * 16);
}
