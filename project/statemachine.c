#include <msp430.h>
#include <libTimer.h>
#include "statemachine.h"
#include "led.h"
#include "buzzer.h"
#include "switches.h"
#include "lcdutils.h"
#include "lcddraw.h"

char button_state = 0;
extern short int state = 0;
extern freq = 500;

/*
state 1: 1 fish 
state 2: 2 fish 
state 3: 3 fish 
state 4: no fish 
*/

char toggle_red(){
  static char state = 0;
  switch(state){
  case 0: red_on = 1; state = 1; break;
  case 1: red_on = 0; state = 0; break;
  }
  return 1;
}


char toggle_green(){
  char changed = 0;
  if(red_on){
    green_on ^= 1;
    changed = 1;
  }
  return changed;
}


// helps in buzzer_advance.s
short get_period(short freq){
  return 2000000/freq;
}


// blinks green led while playing sounds from buzzer advance 
char state1(){
// drawString11x16(30, 10, "1 fish", COLOR_GREEN, COLOR_RED);
  switch(state){
  case 0:
    clearScreen(COLOR_BLUE);
    red_on = 1;
    drawFish(40, 50, COLOR_ORANGE, 20); // change position
    state = 1;
    break;
  case 1:
    clearScreen(COLOR_BLUE);
    red_on = 0;
    drawFish(70, 90, COLOR_ORANGE, 20);
    state = 0; 
    break;
}
return 1;
}


// red led flashes 
char state2(){
//green_on = 0;
  switch(state){
  case 0:
    clearScreen(COLOR_BLACK);
    green_on = 1;
    drawFish(40, 50, COLOR_BLUE, 20);
    drawFish(80, 20, COLOR_ORANGE, 20);
    state = 1;
    buzzer_set_period(1000);
    break;
  case 1:
    clearScreen(COLOR_BLACK);
    green_on = 0;
    drawFish(80, 50, COLOR_BLUE, 20);
    drawFish(40, 80, COLOR_ORANGE, 20);
    state = 0;
    buzzer_set_period(0);
    break;
  }
return 1;
}

// red and green toggle
char state3(){
green_on = 0;
  char changed = 0;
  static enum {R=0, G=1} color = G;
  switch(color){
  case R:
    clearScreen(COLOR_PINK);
    changed = toggle_red();
    drawFish(30, 50, COLOR_BLUE, 20);
    drawFish(90, 30, COLOR_ORANGE, 20);
    drawFish(60, 10, COLOR_WHITE, 20);
    color = G;
    buzzer_set_period(2000);
    break;
  case G:
    clearScreen(COLOR_PINK);
    drawFish(90, 10, COLOR_BLUE, 20);
    drawFish(60, 50, COLOR_ORANGE, 20);
    drawFish(30, 30, COLOR_WHITE, 20);
    changed = toggle_green();
    color = R;
    buzzer_set_period(1000);
    break;
  }
  led_changed = changed;
  led_update();
}


// turns off leds 
char state4(){
  drawString11x16(20,20, "No Fish", COLOR_GREEN, COLOR_BLACK);
  green_on = 0;
  red_on = 0;
  buzzer_set_period(0);
  led_changed = 1;
  led_update();
  return 1;
}

/*
void buzzer_advance(){
if(state) freq += 255;
else freq -= 450;
short cycles = 2000000/freq;
buzzer_set_period(cycles);
}
*/
