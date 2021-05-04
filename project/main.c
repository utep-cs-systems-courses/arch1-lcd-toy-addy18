#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "switches.h"
#include "statemachine.h"
#include "buzzer.h"
#include "led.h"

short redrawScreen = 1;

void wdt_c_handler(){
  static int count = 0;
  static int count2 = 0;
  static int count3 = 0;
 
   if(button_state == 1){
     if((++count % 5) == 0) buzzer_advance();
     if(count == 250){  // every second
      state_advance();
      count = 0;
    }
   }
   if(button_state == 3){
    if(++count3 == 64){ //1/4 
      count3 = 0;
      redrawScreen = 1;
    }
  }
   if(++count2 == 125){ //half a second
    state_advance();
    count2 = 0;
    redrawScreen = 1;
  }
}

int main(){
  P1DIR |= LED_GREEN;
  P1OUT |= LED_GREEN; //red light will be on when msp is turned on
  
  configureClocks();
  lcd_init();
  switch_init();
  init_buzzer();
  
  enableWDTInterrupts(); //enable periodic interrupt
  or_sr(0x8);      // GIE (enable interrupts)

  clearScreen(COLOR_BLACK);

  drawString11x16(20,20,"Adelyn", COLOR_GREEN, COLOR_BLUE);
  while(1){ // on forever
    state_advance();
  
    P1OUT &= ~LED_GREEN; /* green led off*/
    or_sr(0x10); /* cpu off*/
    P1OUT |= LED_GREEN; /*green led on*/ 
  }
}
