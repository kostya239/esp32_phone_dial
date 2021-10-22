#include "src/phone_dial/phone_dial.h"

constexpr unsigned int ROTOR_PIN = 33;
constexpr unsigned int PULSE_PIN = 32;

phone_dial dial = phone_dial();

void setup() {
  // dial.attach(ROTOR_PIN, PULSE_PIN);
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
  DIALING, // is this state necessary? we can probaly handle dialing during
           // HANDSET_OFF state and then go to OUTGOING_CALL state after...
  OUTGOING_CALL,
  INCOMING_CALL,
  CONVERSATION
};

bool is_handset_on();
bool is_call_incoming();
bool is_dialing();
bool is_number_ready();
bool is_interlocutor_dropped_outgoing_call();
bool is_incoming_call_accepted();
bool is_incoming_call_unanswered();
bool is_interlocutor_dropped_conversation();

auto current_state = phone_state_t::HANDSET_ON;

void loop() {
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
    if (is_dialing()) {
      current_state = phone_state_t::DIALING;
      DEBUG_PRINTLN("DEBUG: switching state HANDSET_OFF -> DIALING");
      break;
    }
    break;

  case phone_state_t::DIALING:
    if (is_handset_on()) {
      current_state = phone_state_t::HANDSET_ON;
      DEBUG_PRINTLN("DEBUG: switching state DIALING -> HANDSET_ON");
      break;
    }
    if (is_number_ready()) {
      current_state = phone_state_t::OUTGOING_CALL;
      DEBUG_PRINTLN("DEBUG: switching state DIALING -> OUTGOING_CALL");
      break;
    }
    break;

  case phone_state_t::OUTGOING_CALL:
    if (is_handset_on()) {
      current_state = phone_state_t::HANDSET_ON;
      DEBUG_PRINTLN("DEBUG: switching state OUTGOING_CALL -> HANDSET_ON");
      break;
    }
    if (is_interlocutor_dropped_outgoing_call()) {
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
    if (is_interlocutor_dropped_conversation()) {
      current_state = phone_state_t::HANDSET_OFF;
      DEBUG_PRINTLN("DEBUG: switching state OUTGOING_CALL -> HANDSET_OFF");
      break;
    }
  }
}

bool is_handset_on() {
  delay(3000);
  return false;
}
bool is_call_incoming() { return false; }
bool is_dialing() {
  delay(1000);
  return true;
}
bool is_number_ready() {
  delay(5000);
  return true;
}
bool is_interlocutor_dropped_outgoing_call() { return false; }
bool is_incoming_call_accepted() {
  delay(3000);
  return true;
}
bool is_incoming_call_unanswered() { return false; }
bool is_interlocutor_dropped_conversation() {
  delay(5000);
  return true;
}