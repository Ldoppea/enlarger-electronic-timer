#include <TimerOne.h>
#include "TM1637.h"

#define TIMER_INTERVAL_IN_MS 100
#define TIMER_MAX_IN_MS 99999
#define TIMER_MIN_IN_MS 0
#define TIMER_SMALL_STEP_LIMIT_IN_MS 10000

#define DISPLAY_CLK 5//pins definitions for TM1637 and can be changed to other ports    
#define DISPLAY_DIO 6

int8_t TimeDisp[] = {0x00,0x00,0x00,0x00};
unsigned char ClockPoint = 1;

unsigned long timestamp_ms_current = 0;
unsigned long timestamp_ms_target = 0;
unsigned long timer_ms_duration = 0;
bool timer_on = false;

bool button_increase_pressed = false;
bool button_decrease_pressed = false;

TM1637 tm1637(DISPLAY_CLK, DISPLAY_DIO);

void init_timer()
{
  Serial.begin(9600);
  tm1637.set(BRIGHT_DARKEST);
  tm1637.init();
  Timer1.initialize((long)TIMER_INTERVAL_IN_MS*(long)1000);//timing for 500ms
  Timer1.attachInterrupt(TimingISR);//declare the interrupt serve routine:TimingISR  
}

void do_timing()
{
    TimeUpdate();
    tm1637.display(TimeDisp);
}

void TimingISR()
{
  if(button_increase_pressed) {
    if(timer_ms_duration >= TIMER_SMALL_STEP_LIMIT_IN_MS) {
      timer_ms_duration += TIMER_INTERVAL_IN_MS * 10;
    } else {
      timer_ms_duration += TIMER_INTERVAL_IN_MS;
    }
    
    if(timer_ms_duration > TIMER_MAX_IN_MS) {
      timer_ms_duration = TIMER_MAX_IN_MS;
    }
  }
  
  if(button_decrease_pressed) {
    if(timer_ms_duration >= TIMER_SMALL_STEP_LIMIT_IN_MS) {
      timer_ms_duration -= TIMER_INTERVAL_IN_MS * 10;
    } else {
      timer_ms_duration -= TIMER_INTERVAL_IN_MS;
    }
    
    if(timer_ms_duration < TIMER_MIN_IN_MS) {
      timer_ms_duration = TIMER_MIN_IN_MS;
    }
  }

  ClockPoint = (~ClockPoint) & 0x01;
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

  TimeDisp[3] = (timer_ms_duration / 10) % 10;
  TimeDisp[2] = ((timer_ms_duration / 10) / 10) % 10;
  TimeDisp[1] = ((timer_ms_duration / 10) / 100) % 10;
  TimeDisp[0] = ((timer_ms_duration / 10) / 1000) % 10;
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
