#include <msp430.h>
#include "led.h"

unsigned char red_on = 0, green_on = 0;
unsigned char led_changed = 0;

static char redVal[] = {0, LED_RED}, greenVal[] = {LED_GREEN};
void led_init(){
  P1DIR |= LEDS;
  led_changed = 1;
  led_update();
}

void led_update(){
  if(led_changed){ //if led_changed = 1
    char ledFlags = redVal[red_on] | greenVal[green_on];

    P1OUT &= (0xff - LEDS) | ledFlags; //clear bits
    P1OUT |= ledFlags;// set bits
    led_changed = 0;
  }
}
