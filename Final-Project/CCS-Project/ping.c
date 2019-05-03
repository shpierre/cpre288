
/*
 * ping.c
 *
 *  Created on: Mar 13, 2019
 *      Author: zchiggs
 */

#include "ping.h"
volatile int pingStat;
volatile uint32_t startVal;
volatile uint32_t endVal;
volatile int diff;
volatile float time_sec;
volatile float Sonar_Distance;
volatile int overflow;

void input_edge_timer_init(void)
{
    SYSCTL_RCGCTIMER_R |= 0x8; //enable timer
    TIMER3_CTL_R &= 0xFEFF; //disable the port to modify
    TIMER3_CFG_R = 0x00000004; //selects 32 bit timer
    TIMER3_TBMR_R |= 0x17; //sets as count up, sets capture mode and edge time mode
    TIMER3_TBMR_R &= 0x17; //sets as count up, sets capture mode and edge time mode
    TIMER3_CTL_R |= 0xC00; //set both edge
    TIMER3_IMR_R |= 0x400; //enable capture mode interrupts
    TIMER3_CTL_R |= 0x100; //re enable timer
    TIMER3_ICR_R |= 0x200; //clear interrupts
    NVIC_EN1_R |= 0x10;
    NVIC_PRI13_R &= 0x10;
    //Enables Interrupt and bind handler
    IntRegister(INT_TIMER3B, ping_handler);
    IntMasterEnable(); //enables interrupts

}
void ping_init(void)
{

    SYSCTL_RCGCGPIO_R |= 0x2; //Enable GPIO port B clock
    GPIO_PORTB_AFSEL_R |= 0x08; //Enable alternate function of PB3
    GPIO_PORTB_PCTL_R |= 0x7000; // Enable alternate function 7 of PB3
    GPIO_PORTB_DEN_R |= 0x08; //Enables PB3's digital functions
    GPIO_PORTB_DIR_R &= 0xF7; //Sets PB3 as an input

}

void send_pulse(void)
{
    GPIO_PORTB_AFSEL_R &= 0xF7; //Disable alternate function of PB3
    GPIO_PORTB_DIR_R |= 0x08; //Set as output
    GPIO_PORTB_DATA_R &= 0xF7; // set low
    GPIO_PORTB_DATA_R |= 0x8; // set high
    timer_waitMicros(5); // wait(based on data sheet)
    GPIO_PORTB_DATA_R &= 0xF7; // set low
    GPIO_PORTB_AFSEL_R |= 0x08; //Enable alternate function of PB3
    GPIO_PORTB_PCTL_R |= 0x7000; // Enable alternate function 7 of PB3
    GPIO_PORTB_DIR_R &= 0xF7; //Sets PB3 as an input
    pingStat = 1; //pulse has been sent

}
void ping_handler(void)
{
    TIMER3_ICR_R |= 0xFFF; //clear interrupts
    if (pingStat == 1)
    { //if pulse was just sent
        startVal = TIMER3_TBR_R; //record clock time
        pingStat = 2; // prepare for negative edge
    }
    else if (pingStat == 2)
    { // if positive edge was triggered before
        endVal = TIMER3_TBR_R; // record clock time
        diff = endVal - startVal; // calculate pulse width in clock cycles
        if (diff > 0)
        { //if no overflow
            time_sec = (.0000000625) * diff; //difference in clock cycles * by the clock period (1/16MHz) to get pulse width in sec
            Sonar_Distance = 34000 * time_sec / 2; // multiply by the speed of sound per cms, and divide by 2 to get 1 way length
        }
        else
        {   //check for overflow
            overflow = overflow + 1;
            diff = (endVal + 16777216) - startVal; // add 2^24
            time_sec = (.0000000625) * diff; //*by the clock period (1/16MHz)
            Sonar_Distance = 34000 * time_sec / 2; // multiply by the speed of sound per cms, and divide by 2 to get 1 way length
        }
        pingStat = 0; // print data
    }
}

