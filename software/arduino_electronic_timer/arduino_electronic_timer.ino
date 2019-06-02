
void setup() {
  init_buttons();
  init_relay();
  init_timer();
}

void loop() {
  read_buttons();
  update_relay();
  do_timing();
}
