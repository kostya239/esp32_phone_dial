#define DEBUG

constexpr unsigned int ROTOR_PIN = 33;
constexpr unsigned int PULSE_PIN = 32;
constexpr unsigned int MAX_NUMBER_LENGTH = 20;
constexpr unsigned int ROTOR_BOUNCE_CTR_VAL = 10000;
constexpr unsigned int PULSE_BOUNCE_CTR_VAL = 10000;

enum class rotor_state_t {
  idle,
  active
};

rotor_state_t rotor_state = rotor_state_t::idle;

enum class pulse_state_t {
  low,
  high
};

pulse_state_t pulse_state = pulse_state_t::low;

unsigned int pulse_ctr = 0;
unsigned int number_buffer[MAX_NUMBER_LENGTH] = {0};
unsigned int number_length = 0;

unsigned int rotor_bounce_ctr = 0;
unsigned int pulse_bounce_ctr = 0;

// TODO: replace global variables with local static
inline void handle_rotor() {
  if (rotor_bounce_ctr) {
    rotor_bounce_ctr -= 1;
  } else {
    // Transition between two states
    if (digitalRead(ROTOR_PIN) && rotor_state == rotor_state_t::idle) {
      rotor_state = rotor_state_t::active;
      rotor_bounce_ctr = ROTOR_BOUNCE_CTR_VAL;
    }
    if (rotor_state == rotor_state_t::active && !digitalRead(ROTOR_PIN)) {
      rotor_state = rotor_state_t::idle;
      rotor_bounce_ctr = ROTOR_BOUNCE_CTR_VAL;
#ifdef DEBUG
      Serial.print(">>> DBG: Current digit: ");
      Serial.println(pulse_ctr);
#endif

      if (number_length == MAX_NUMBER_LENGTH) {
        number_length = 0;
#ifdef DEBUG
        Serial.println(">>> DBG: Number buffer overflow");
#endif
      }

      number_buffer[number_length] = pulse_ctr % 10;
      pulse_ctr = 0;
      number_length += 1;
    }
  }

  // Counting impulses
  if (rotor_state == rotor_state_t::active) {
    if (pulse_bounce_ctr) {
      pulse_bounce_ctr -= 1;
    } else {
      if (digitalRead(PULSE_PIN) && pulse_state == pulse_state_t::low) {
        pulse_state = pulse_state_t::high;
        pulse_bounce_ctr = PULSE_BOUNCE_CTR_VAL;

        pulse_ctr += 1;
      }
      if (pulse_state == pulse_state_t::high && !digitalRead(PULSE_PIN)) {
        pulse_state = pulse_state_t::low;
        pulse_bounce_ctr = PULSE_BOUNCE_CTR_VAL;
      }
    }
  }
}

void setup() {
  // NOTE: not every pin has pullup/pulldown resistors!
  pinMode(ROTOR_PIN, INPUT_PULLDOWN);
  pinMode(PULSE_PIN, INPUT_PULLUP);
  Serial.begin(115200);
}

void loop() {
  handle_rotor();
}
