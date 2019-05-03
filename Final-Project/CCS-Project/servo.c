#include "servo.h"
#include "lcd.h"
#include <math.h>
volatile int sign = 1;

void servo_init(void){
    SYSCTL_RCGCGPIO_R |= 0x2;//Enable Port B Timer
    SYSCTL_RCGCTIMER_R |= 0x2; //enable timer one

    GPIO_PORTB_DEN_R &= 0xDF; //disable digital for PB5

    GPIO_PORTB_DIR_R |= 0x20;//set PB5 as output
    GPIO_PORTB_AFSEL_R |= 0x20;//set PB5 as alternate function
    GPIO_PORTB_PCTL_R |= 0x700000;//select Timer 1 for PB5

    GPIO_PORTB_DEN_R |= 0x20;//enable PB5 digital function

    TIMER1_CTL_R &= 0xFEFF; //disable the port to modify 733
    TIMER1_CFG_R = 0x00000004;//selects 16 bit timer
    TIMER1_TBMR_R |= 0xA;// enable edge time mode and PWM mode
    TIMER1_CTL_R &= 0xBFFF; //do not invert output**
    TIMER1_CTL_R |= 0x0C00;//set both edges

    TIMER1_TBPR_R = 0x04; //write first 8 bits (pre-scaler)    320000
    TIMER1_TBILR_R = 0xE200; //set last 16 bits                320000
    TIMER1_TBPMR_R = 0x04;//match pre-scale
    TIMER1_TBMATCHR_R = 34260;//Set match value to set servo angle to about 90 (1ms)   **** for bot 13 is about 87
    TIMER1_CTL_R |= 0x0100; //re-enable port

}
void servo_move(float degrees){
    int val;
    val = ((MAX-MIN)*degrees*pow(180, -1)); //normalize and convert degrees into match value with respect to max and min
   if (((TIMER1_TBMATCHR_R + val) >= MIN) && val <= 0){ // if val is positive and under 180, add to match val (clockwise)
       TIMER1_TBMATCHR_R = TIMER1_TBMATCHR_R + val;
   }
   else if (((TIMER1_TBMATCHR_R + val) <= MAX) && val >= 0){// if val is negative and over 0, subtract from match val (counter_clockwise)
          TIMER1_TBMATCHR_R = TIMER1_TBMATCHR_R + val;
      }
}
void servo_set(float degrees){
    int val;
    val = MIN+((MAX-MIN)*degrees*pow(180, -1)); //convert degrees into match value
    TIMER1_TBMATCHR_R = val; // set value equal to match value
}



