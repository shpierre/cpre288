/*
 * servo.c
 *
 * Calibrated for CyBot #4
 *
 *  Created on: Mar 27, 2019
 *      Author: aidans
 */
#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include "driverlib/interrupt.h"
#include <inc/tm4c123gh6pm.h>

void servo_init(void)
{
    //Enable clock for port b
    SYSCTL_RCGCGPIO_R |= 0x02;
    //Enable alternate function
    GPIO_PORTB_AFSEL_R |= 0x20;
    //Select alternate function
    GPIO_PORTB_PCTL_R |= 0x00700000;
    //Enable PB5
    GPIO_PORTB_DEN_R |= 0x20;
    //Set PB5 as an output
    GPIO_PORTB_DIR_R |= 0x20;
    //Enable clock for TIMER 1B
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1;
    //Disable the TIMER for config
    TIMER1_CTL_R &= ~TIMER_CTL_TBEN;
    //Select TIMER configuration
    TIMER1_CFG_R |= 0x04;
    //Set TIMER mode, PWM mode with periodic count
    TIMER1_TBMR_R |= 0xA;
    // start value in clock cycles (ms to clock cycles)
    unsigned int pwm_period;
    pwm_period = 20 * 1000 * 16;
    // lower 16 bits of start value
    TIMER1_TBILR_R = pwm_period & 0xFFFF;
    // next 8 bits of the start value
    TIMER1_TBPR_R = pwm_period >> 16;
    //match value
    TIMER1_TBMATCHR_R = (195 * 100 * 16);
    TIMER1_TBPMR_R = (195 * 100 * 16) >> 16;
    //Enable the TIMER
    TIMER1_CTL_R |= TIMER_CTL_TBEN;
}

int servo_move(float degrees)
{

    unsigned int newval;
    newval = (TIMER1_TBPMR_R << 16) + TIMER1_TBMATCHR_R - (160 * degrees);

    TIMER1_TBMATCHR_R = (newval);
    TIMER1_TBPMR_R = (newval) >> 16;

    return 0;
}
