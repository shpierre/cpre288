/*
 * ping.c
 *
 *  Created on: Mar 10, 2019
 *      Author: Aidan
 */
#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include "driverlib/interrupt.h"
#include <inc/tm4c123gh6pm.h>

volatile unsigned current_time = 0;
volatile unsigned last_time = 0;
volatile double dist = 0;
volatile int update = 0;
extern volatile int overflows;

void TIMER3B_Handler(void)
{
    if (update == 0)
    {
        last_time = current_time;
        current_time = TIMER3_TBR_R;
        update = 1; //indicates rising edge of echo pulse
    }
    else if (update == 1)
    {
        last_time = current_time;
        current_time = TIMER3_TBR_R;
        update = 2; //indicates falling edge of echo pulse
    }
    //Clear interrupt
    TIMER3_ICR_R |= 0x400;

}

void portb_init(void)
{
    //Enable clock for port b
    SYSCTL_RCGCGPIO_R |= 0x02;
    //Enable alternate function
    GPIO_PORTB_AFSEL_R |= 0x08;
    //Select alternate function
    GPIO_PORTB_PCTL_R |= 0x7000;
    //Enable PB3
    GPIO_PORTB_DEN_R |= 0x08;
    //Set PB3 as an input
    GPIO_PORTB_DIR_R &= 0xF7;
}

void timer_init(void)
{
    //Enable clock for timer 3
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3;
    //Disable the timer for config
    TIMER3_CTL_R &= ~TIMER_CTL_TBEN;
    //Select timer configuration
    TIMER3_CFG_R |= 0x04;
    //Set timer mode, input capture mode, edge time count up
    TIMER3_TBMR_R = 0x17;
    //Set both edges to trigger interrupt
    TIMER3_CTL_R |= 0x0c00;
    //Load initial max value
    TIMER3_TBPR_R = 0xff;
    //Load initial max value
    TIMER3_TBILR_R = 0xffff;
    //Clear interrupt
    TIMER3_ICR_R |= 0x400;
    //Enable interrupt
    TIMER3_IMR_R |= 0x400;
    //Enable the timer
    TIMER3_CTL_R |= TIMER_CTL_TBEN;
    //Sets bits 7:5 to 0b001
    NVIC_PRI9_R |= 0x20;
    //Enable interrupt for T3CCP1
    NVIC_EN1_R |= 0x10;
    //Link interrupt to handler
    IntRegister(INT_TIMER3B, TIMER3B_Handler);
}

void ping_init(void)
{
    portb_init();
    timer_init();
}

void sendPulse(void)
{
    //Start T3B
    TIMER3_CTL_R &= ~TIMER_CTL_TBEN;
    //Disable alternate function for PB3
    GPIO_PORTB_AFSEL_R &= 0xF7;
    //Set PB3 as an output
    GPIO_PORTB_DIR_R |= 0x08;
    //Set PB3 high
    GPIO_PORTB_DATA_R |= 0x08;
    //Wait 10 us
    timer_waitMicros(10);
    //Set PB3 low
    GPIO_PORTB_DATA_R &= 0xF7;
    //Set PB3 as an input
    GPIO_PORTB_DIR_R &= 0xF7;
    //Enable alternate function
    GPIO_PORTB_AFSEL_R |= 0x08;
    //Stop T3B
    TIMER3_CTL_R |= TIMER_CTL_TBEN;
}

void ping_read(void)
{
    double pulse_width = 0;
    //Send trigger
    sendPulse();
    //Wait for falling edge
    while (update < 2)
        ;
    if (update == 2)
    {
        //Calculate pulse width in microseconds
        pulse_width = (double) current_time - last_time;
        //Check for and handle overflow
        if (current_time < last_time)
        {
            overflows++;
            pulse_width = (double) (overflows << 24) + pulse_width;
        }

        //Convert from clock cycles to microseconds
        dist = pulse_width / 16;
        //Convert to milliseconds
        dist /= 1000;
        //Divide time by two to take into account the time to travel to object AND BACK
        dist /= 2;
        //Multiply by 100 to convert meters to cm, divide by 1000 to convert s to ms
        //Divide by speed of sound, after all this we get that we need to divide by 34 to get cm/ms
        dist *= 34;
        update = 0;
    }

}
