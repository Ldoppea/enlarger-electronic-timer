#include <Bounce2.h>

#define BUTTON_FORCE 13
#define BUTTON_INCREASE 12
#define BUTTON_DECREASE 11
#define BUTTON_TIMER 10

#define BUTTON_TIMER_STOP_DURATION_IN_MS 1500
#define BUTTON_TIMER_RESET_DURATION_IN_MS 3000

int buttonState0 = 0;
int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;

bool is_button_timer_pressed = false;
unsigned long button_timer_press_duration = 0;

Bounce debouncerForce = Bounce();
Bounce debouncerIncrease = Bounce();
Bounce debouncerDecrease = Bounce();
Bounce debouncerTimer = Bounce();

void init_buttons() {
  pinMode(BUTTON_FORCE, INPUT_PULLUP);
  pinMode(BUTTON_INCREASE, INPUT_PULLUP);
  pinMode(BUTTON_DECREASE, INPUT_PULLUP);
  pinMode(BUTTON_TIMER, INPUT_PULLUP);

  debouncerForce.attach(BUTTON_FORCE);
  debouncerIncrease.attach(BUTTON_INCREASE);
  debouncerDecrease.attach(BUTTON_DECREASE);
  debouncerTimer.attach(BUTTON_TIMER);
  
  debouncerForce.interval(10); // 5 milliseconds.
  debouncerIncrease.interval(10); // 5 milliseconds.
  debouncerDecrease.interval(10); // 5 milliseconds.
  debouncerTimer.interval(10); // 5 milliseconds.
}

void read_buttons(){
  debouncerForce.update();
  debouncerIncrease.update();
  debouncerDecrease.update();
  debouncerTimer.update();

  // Force Button
  if(debouncerForce.fell())
  {
    force_relay();
  }
 
  if(debouncerForce.rose())
  {
    unforce_relay();
  }

  // Increase Timer
  if(debouncerIncrease.fell()) {
    increase_timer();
  }
  
  if(debouncerIncrease.rose()) {
    stop_increase_timer();
  }

  // Decrease Timer
  if(debouncerDecrease.fell()) {
    decrease_timer();
  }
  
  if(debouncerDecrease.rose()) {
    stop_decrease_timer();
  }

  // Timer button
  if(debouncerTimer.fell()) {
    button_timer_press_duration = 0;
    is_button_timer_pressed = true;
  }
  
  if(debouncerTimer.rose()) {
    is_button_timer_pressed = false;
    
    if(button_timer_press_duration < BUTTON_TIMER_STOP_DURATION_IN_MS) {
      toggle_timer();
    }
  }
  
  buttonState2 = digitalRead(BUTTON_DECREASE);
  buttonState3 = digitalRead(BUTTON_TIMER);
}

