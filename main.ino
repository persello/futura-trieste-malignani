void setup() {
  setupMotors();
  Serial.begin(115200);
}

void loop() {
  updateMotors();
}
