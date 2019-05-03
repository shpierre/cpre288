#include "adc.h"
#include <math.h>
/*
 * adc.c
 *
 *  Created on: Mar 10, 2019
 *      Author: zchiggs
 */

void PortB_Init(void)
{
    SYSCTL_RCGCADC_R |= 0x1; //Enable ADC Clock
    SYSCTL_RCGCGPIO_R |= 0x2; //Enable GPIO port B clock
    GPIO_PORTB_AFSEL_R |= 0x10; //Sets PB4 to alternate function
    GPIO_PORTB_DEN_R &= 0xEF; //Disables PB4's digital functions
    GPIO_PORTB_AMSEL_R |= 0x10; //Disable analog isolation on PB4
}

void ADC_init(void)
{
    ADC0_ACTSS_R &= 0xFFFFFFF0; //Disable Sample Sequencers for configuration
    ADC0_SSMUX0_R = 0x0000000A; //Select AIN10(PB4) as input for ADC0 sample sequencer 4
    ADC0_SSCTL0_R = 0x00000006; //Configures SS0 to take one sample and place into fifo
    ADC0_EMUX_R &= 0xFFFF0000; //Configure all of the SS to trigger on change in PSSI input
    ADC0_SAC_R |= 0x4; //Configure ADC hardware averager to take 16 samples
    ADC0_ACTSS_R |= 0x1; //Enable SS0
}

