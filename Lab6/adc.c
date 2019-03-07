/*
 * adc.c
 *
 *  Created on: Mar 5, 2019
 *      Author: Aidan
 */
#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
void adc_init(void){
     // Turn on clock for Port B
     SYSCTL_RCGCGPIO_R |= 0b000010;
     SYSCTL_RCGCADC_R |= 0x1; // Enable ADC0 clock

     //Enable alternate function
     GPIO_PORTB_AFSEL_R |= 0b00010000; //set PB4 to alternate function

     //Configure pins for input and analog mode
     GPIO_PORTB_DIR_R &= 0b11101111; //Set PB4 to input mode
     GPIO_PORTB_DEN_R &= 0b11101111; //Disable digital mode PB4
     GPIO_PORTB_AMSEL_R |= 0b00010000; //Enable analog mode PB4

     //Use default sample trigger source
     GPIO_PORTB_ADCCTL_R = 0x0;

     //Configure ADC0
     //ADC0_ADCCC_R = 0x0; //Use SysClk as ADC clk
     ADC0_ACTSS_R &= 0xFFFE; //Disable ADC0 SS0 by clearing bit 0

     // Set ADC SS0 functionality
     ADC0_EMUX_R &= 0xFFF0;
     ADC0_SSMUX0_R |= 0x000A; //Set 1st sample to user AIN10 ADC pin

     //enable oversampling to average
     //ADC0_SAC_R |= ACD_SAC_AVG_16X;
     ADC0_SAC_R |= 0x00000004;

     //
     ADC0_SSCTL0_R |= (ADC_SSCTL0_IE0 | ADC_SSCTL0_END0);

     // Re-enable ADC SS0
     ADC0_ACTSS_R |= 0x1; //Re-enable ADC0 SS0 by setting bit 0


}

int adc_read(void){
    //Initialize the conversation
    ADC0_PSSI_R |= ADC_PSSI_SS0;

    //Wait for the conversation to be complete
    while((ADC0_RIS_R & ADC_RIS_INR0) == 0){}
    ADC0_ISC_R = ADC_ISC_IN0;
    //Get result from FIFO
    return ADC0_SSFIFO0_R;
}

