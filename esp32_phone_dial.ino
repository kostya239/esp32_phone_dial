#include "src/phone_dial/phone_dial.h"

constexpr unsigned int ROTOR_PIN = 33;
constexpr unsigned int PULSE_PIN = 32;

phone_dial dial = phone_dial();

void setup() {
  dial.attach(ROTOR_PIN, PULSE_PIN);
  Serial.begin(115200);
}

void loop() {
  dial.update();
  if (dial.ready()) {
    Serial.println(dial.read());
  }
}
