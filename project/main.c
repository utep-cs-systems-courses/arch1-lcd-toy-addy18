#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "switches.h"
#include "statemachine.h"
#include "buzzer.h"
#include "led.h"

short redrawScreen = 1;
u_int fontFgColor = COLOR_PURPLE;
long COLOR1;
long COLOR2;

static int prev = 0;

u_char centerWidth = (screenWidth/2) + 1;
u_char centerHeight = (screenHeight/2) + 1;

static u_char colorState = 0;
/*
void draw_diamond(u_char col, u_char row, u_char size, u_int color1){  
      for(u_char r = 0; r < size; r++){
	for(u_char c = 0; c < r; c++){
	  drawPixel(col-c, row-r-1, color1);
	  drawPixel(col-c, row+r-(2*size), color1);
	  drawPixel(col+c, row-r-1, color1);
	  drawPixel(col+c, row+r-(2*size), color1);
	}
      }
}
*/

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
