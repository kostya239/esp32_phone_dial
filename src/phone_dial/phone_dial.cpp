#include "phone_dial.h"

phone_dial::phone_dial()
    : is_digit_ready{false}, pulse_ctr{0}, rotor_state{LOW}, pulse_state{LOW},
      rotor_bounce_ctr{0}, pulse_bounce_ctr{0} {}

void phone_dial::attach(unsigned int rotor_pin, unsigned int pulse_pin) {
  // NOTE: not every pin has pullup/pulldown resistors!
  pinMode(rotor_pin, INPUT_PULLDOWN);
  pinMode(pulse_pin, INPUT_PULLUP);
  this->rotor_pin = rotor_pin;
  this->pulse_pin = pulse_pin;
}

void phone_dial::update() {
  int rotor_pin_value = digitalRead(rotor_pin);
  if (rotor_bounce_ctr) {
    rotor_bounce_ctr -= 1;
  } else {
    int rotor_pin_value = digitalRead(rotor_pin);
    if (rotor_pin_value == HIGH && rotor_state == LOW) {
      rotor_state = HIGH;
      rotor_bounce_ctr = ROTOR_BOUNCE_CTR_VAL;
    }
    if (rotor_pin_value == LOW && rotor_state == HIGH) {
      rotor_state = LOW;
      rotor_bounce_ctr = ROTOR_BOUNCE_CTR_VAL;

      digit = pulse_ctr % 10;
      is_digit_ready = true;
      pulse_ctr = 0;
    }
  }

  if (rotor_state == HIGH) {
    if (pulse_bounce_ctr) {
      pulse_bounce_ctr -= 1;
    } else {
      int pulse_pin_value = digitalRead(pulse_pin);
      if (pulse_pin_value == HIGH && pulse_state == LOW) {
        pulse_state = HIGH;
        pulse_bounce_ctr = PULSE_BOUNCE_CTR_VAL;
        pulse_ctr += 1;
      }
      if (pulse_pin_value == LOW && pulse_state == HIGH) {
        pulse_state = LOW;
        pulse_bounce_ctr = PULSE_BOUNCE_CTR_VAL;
      }
    }
  }
}

bool phone_dial::ready() { return is_digit_ready; }

unsigned int phone_dial::read() {
  is_digit_ready = false;
  return digit;
}
