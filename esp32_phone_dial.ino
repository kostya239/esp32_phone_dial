#include "src/phone_dial/phone_dial.h"

constexpr unsigned int ROTOR_PIN = 33;
constexpr unsigned int PULSE_PIN = 32;

phone_dial dial = phone_dial();

void setup() {
  dial.attach(ROTOR_PIN, PULSE_PIN);
  Serial.begin(115200);
}

#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_PRINTLN(x)
#endif

enum class phone_state_t {
  HANDSET_ON,
  HANDSET_OFF,
  OUTGOING_CALL,
  INCOMING_CALL,
  CONVERSATION
};

int number[11];

bool is_handset_on();
bool is_call_incoming();
bool is_number_ready();
bool is_they_dropped_outgoing_call();
bool is_incoming_call_accepted();
bool is_incoming_call_unanswered();
bool is_they_dropped_conversation();

void on_number_ready();

auto current_state = phone_state_t::HANDSET_ON;

void loop() {
dial.update();

  switch (current_state) {

  case phone_state_t::HANDSET_ON:
    if (!is_handset_on()) {
      current_state = phone_state_t::HANDSET_OFF;
      DEBUG_PRINTLN("DEBUG: switching state HANDSET_ON -> HANDSET_OFF");
      break;
    }
    if (is_call_incoming()) {
      current_state = phone_state_t::INCOMING_CALL;
      DEBUG_PRINTLN("DEBUG: switching state HANDSET_ON -> INCOMING_CALL");
      break;
    }
    break;

  case phone_state_t::HANDSET_OFF:
    if (is_handset_on()) {
      current_state = phone_state_t::HANDSET_ON;
      DEBUG_PRINTLN("DEBUG: switching state HANDSET_OFF -> HANDSET_ON");
      break;
    }
    if (is_number_ready()) {
      current_state = phone_state_t::OUTGOING_CALL;
      DEBUG_PRINTLN("DEBUG: switching state HANDSET_OFF -> OUTGOING_CALL");
      on_number_ready();
      break;
    }
    break;

  case phone_state_t::OUTGOING_CALL:
    if (is_handset_on()) {
      current_state = phone_state_t::HANDSET_ON;
      DEBUG_PRINTLN("DEBUG: switching state OUTGOING_CALL -> HANDSET_ON");
      break;
    }
    if (is_they_dropped_outgoing_call()) {
      current_state = phone_state_t::HANDSET_OFF;
      DEBUG_PRINTLN("DEBUG: switching state OUTGOING_CALL -> HANDSET_OFF");
      break;
    }
    if (is_incoming_call_accepted()) {
      current_state = phone_state_t::CONVERSATION;
      DEBUG_PRINTLN("DEBUG: switching state OUTGOING_CALL -> CONVERSATION");
      break;
    }
    break;

  case phone_state_t::INCOMING_CALL:
    if (!is_handset_on()) {
      current_state = phone_state_t::CONVERSATION;
      DEBUG_PRINTLN("DEBUG: switching state INCOMING_CALL -> CONVERSATION");
      break;
    }
    if (is_incoming_call_unanswered()) {
      current_state = phone_state_t::HANDSET_ON;
      DEBUG_PRINTLN("DEBUG: switching state INCOMING_CALL -> HANDSET_ON");
      break;
    }
    break;

  case phone_state_t::CONVERSATION:
    if (is_handset_on()) {
      current_state = phone_state_t::HANDSET_ON;
      DEBUG_PRINTLN("DEBUG: switching state CONVERSATION -> HANDSET_ON");
      break;
    }
    if (is_they_dropped_conversation()) {
      current_state = phone_state_t::HANDSET_OFF;
      DEBUG_PRINTLN("DEBUG: switching state CONVERSATION -> HANDSET_OFF");
      break;
    }
  }
}

bool is_handset_on() { return false; }
bool is_call_incoming() { return false; }
bool is_number_ready() {
  static int ctr = 0;
  if (ctr < 11) {
    if (dial.ready()) {
      number[ctr] = dial.read();
      if (ctr == 10) return true;
      ++ctr;
    }
  } else {
    ctr = 0;
    if (dial.ready()) {
      number[ctr] = dial.read();
      ++ctr;
    }
  }
  return false;
}
bool is_they_dropped_outgoing_call() { return false; }
bool is_incoming_call_accepted() { return false; }
bool is_incoming_call_unanswered() { return false; }
bool is_they_dropped_conversation() { return true; }

void on_number_ready()
{
  Serial.print("calling... ");
  for (int i = 0; i < 11; ++i) {
    Serial.print(number[i]);
  }
  Serial.println("");
}