
// 29.06.22 für Test mit dem Controllino Maxi eine Lebenslichtfunktion für Ausgang D11 ergänzt
void lifeLedD11(unsigned long waitTime) {
  // const unsigned long waitTime = 500;
  static unsigned long oldTime = 0;
  static int state = LOW;

  if (oldTime + waitTime < millis()) {
    if (state == LOW) state = HIGH; else state = LOW;
    oldTime = millis();
    digitalWrite(CONTROLLINO_D11, state);
  }
} //  Ende void lifeLedD11