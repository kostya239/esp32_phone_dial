#ifndef PHONE_DIAL_H
#define PHONE_DIAL_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

constexpr unsigned int ROTOR_BOUNCE_CTR_VAL = 1000;
constexpr unsigned int PULSE_BOUNCE_CTR_VAL = 1500;

class phone_dial {
public:
  phone_dial();
  void attach(unsigned int rotor_pin, unsigned int pulse_pin);
  void update();
  bool ready();
  unsigned int read();

private:
  unsigned int rotor_pin;
  unsigned int pulse_pin;
  int rotor_state;
  int pulse_state;
  unsigned int digit;
  bool is_digit_ready;
  unsigned int pulse_ctr;
  unsigned int rotor_bounce_ctr;
  unsigned int pulse_bounce_ctr;
};

#endif
