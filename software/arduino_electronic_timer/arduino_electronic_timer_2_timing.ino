#include <TimerOne.h>
#include "TM1637.h"

#define ARDUINO_TIMER_RUN_INTERVAL_IN_MS 10
#define ARDUINO_TIMER_SLOW_INTERVAL_IN_MS 100
#define ARDUINO_TIMER_FAST_INTERVAL_IN_MS 10

#define TIMER_MAX_IN_MS 99999
#define TIMER_MIN_IN_MS 0
#define TIMER_SMALL_STEP_LIMIT_IN_MS 10000
#define TIMER_STEP 100

#define DISPLAY_CLK 5//pins definitions for TM1637 and can be changed to other ports    
#define DISPLAY_DIO 6

int8_t TimeDisp[] = {0x00,0x00,0x00,0x00};
unsigned char ClockPoint = 1;

long timer_period = ARDUINO_TIMER_SLOW_INTERVAL_IN_MS;
unsigned long timestamp_ms_current = 0;
unsigned long timestamp_ms_target = 0;
long timer_ms_duration = 0;
long timer_ms_target_duration = 0;
bool is_timer_on = false;
bool is_timer_paused = false;


bool button_increase_pressed = false;
bool button_decrease_pressed = false;

TM1637 tm1637(DISPLAY_CLK, DISPLAY_DIO);

void init_timer()
{
  Serial.begin(9600);
  tm1637.set(BRIGHT_DARKEST);
  tm1637.init();
  Timer1.initialize((long)timer_period*(long)1000);
  Timer1.attachInterrupt(TimingISR);//declare the interrupt serve routine:TimingISR  
}

void do_timing()
{
    TimeUpdate();
    tm1637.display(TimeDisp);
}

void TimingISR()
{
  if(is_button_timer_pressed == true) {
    button_timer_press_duration += timer_period;
    if(button_timer_press_duration > BUTTON_TIMER_STOP_DURATION_IN_MS) {
      stop_timer();
    }
    if(button_timer_press_duration > BUTTON_TIMER_RESET_DURATION_IN_MS) {
      timer_ms_target_duration = 0;
    }
  }
  
  if(is_timer_on == true && is_timer_paused == false) {
    timer_ms_duration -= ARDUINO_TIMER_RUN_INTERVAL_IN_MS;
    if(timer_ms_duration < 0) {
      stop_timer();
    }
  } else {
    if(button_increase_pressed) {    
      if(timer_ms_target_duration <= TIMER_SMALL_STEP_LIMIT_IN_MS && timer_ms_target_duration + TIMER_STEP >= TIMER_SMALL_STEP_LIMIT_IN_MS) {
        change_timer_period(ARDUINO_TIMER_FAST_INTERVAL_IN_MS);
      }
    
      if(timer_ms_target_duration + TIMER_STEP <= TIMER_MAX_IN_MS) {
        timer_ms_target_duration += TIMER_STEP;
      }
    }
    
    if(button_decrease_pressed) {
      if(timer_ms_target_duration >= TIMER_SMALL_STEP_LIMIT_IN_MS && timer_ms_target_duration - TIMER_STEP <= TIMER_SMALL_STEP_LIMIT_IN_MS) {
        change_timer_period(ARDUINO_TIMER_SLOW_INTERVAL_IN_MS);
      }
      
      if(timer_ms_target_duration - TIMER_STEP >= TIMER_MIN_IN_MS) {
        timer_ms_target_duration -= TIMER_STEP;
      }
    }
  }

  ClockPoint = (~ClockPoint) & 0x01;
}

void display_timer_not_running() {
  TimeDisp[3] = 0;
  TimeDisp[2] = timer_ms_target_duration >= TIMER_SMALL_STEP_LIMIT_IN_MS ? ((timer_ms_target_duration / 10) / 100 * 10) % 10 : ((timer_ms_target_duration / 10) / 10) % 10;
  TimeDisp[1] = ((timer_ms_target_duration / 10) / 100) % 10;
  TimeDisp[0] = ((timer_ms_target_duration / 10) / 1000) % 10;
}

void display_timer_running() {
  TimeDisp[3] = (timer_ms_duration / 10) % 10;
  TimeDisp[2] = ((timer_ms_duration / 10) / 10) % 10;
  TimeDisp[1] = ((timer_ms_duration / 10) / 100) % 10;
  TimeDisp[0] = ((timer_ms_duration / 10) / 1000) % 10;
}

void TimeUpdate(void)
{
  if(ClockPoint)
  {
    tm1637.point(POINT_ON);
  }
  else
  {
    tm1637.point(POINT_OFF); 
  }

  if(is_timer_on == true) {
    display_timer_running();
  } else {
    display_timer_not_running();
  }
}

void increase_timer() {
  button_increase_pressed = true;
}

void stop_increase_timer() {
  button_increase_pressed = false;
}

void decrease_timer() {
  button_decrease_pressed = true;
}

void stop_decrease_timer() {
  button_decrease_pressed = false;
}

void start_timer() {
  change_timer_period(ARDUINO_TIMER_RUN_INTERVAL_IN_MS);
  timer_ms_duration = timer_ms_target_duration >= TIMER_SMALL_STEP_LIMIT_IN_MS ? (timer_ms_target_duration / 1000 * 1000) : (timer_ms_target_duration / 100 * 100);
  is_timer_on = true;
  turn_relay_on();
}

void pause_timer() {
  is_timer_paused = !is_timer_paused;

  if(is_timer_paused) {
    turn_relay_off();
  } else {
    turn_relay_on();
  }
}

void stop_timer() {
  timer_ms_duration = 0;
  is_timer_on = false;
  is_timer_paused = false;
  
  if(timer_ms_target_duration <= TIMER_SMALL_STEP_LIMIT_IN_MS) {
    change_timer_period(ARDUINO_TIMER_SLOW_INTERVAL_IN_MS);
  } else {
    change_timer_period(ARDUINO_TIMER_FAST_INTERVAL_IN_MS);
  }

  turn_relay_off();
}

void toggle_timer() {
  if(is_timer_on == true) {
    pause_timer();
  } else {
    start_timer();
  }
}

void change_timer_period(long new_period) {
  timer_period = new_period;
  Timer1.setPeriod(new_period*(long)1000);
}
