#define RELAY_SIG 4

bool is_relay_on = false;
bool is_relay_on_forced = false;

void init_relay() {
  pinMode(RELAY_SIG, OUTPUT);
}

void update_relay() {
  if(is_relay_on || is_relay_on_forced) {
    digitalWrite(RELAY_SIG, HIGH);
  }
  else {
    digitalWrite(RELAY_SIG, LOW);
  }
}

void turn_relay_on() {
  is_relay_on = true;
}

void turn_relay_off() {
  is_relay_on = false;
}

void force_relay() {
  is_relay_on_forced = true;
}

void unforce_relay() {
  is_relay_on_forced = false;
}

