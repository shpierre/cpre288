/*
 * uart.c
 *
 *  Created on: Feb 20, 2019
 *      Author: Aidan
 */

#include <stdint.h>
#include <stdbool.h>
#include <inc/tm4c123gh6pm.h>

#include "timer.h"
#include "lcd.h"
#include "driverlib/interrupt.h"
#include "button.h"
#include "uart.h"

#define INT_UART1 22

volatile char buffer[21]; // create our line buffer
volatile char cur = '\0'; // initialize current character to null
volatile int len = 0; // number of chars printed this line

void uart_init(void)
{
    SYSCTL_RCGCGPIO_R |= 0x02; // enable clock GPIOB
    SYSCTL_RCGCUART_R |= 0x02; // enable clock UART1
    GPIO_PORTB_AFSEL_R = 0x03; // sets pb0 and pb1 as peripherals
    GPIO_PORTB_PCTL_R = 0x11; // pmc0 and pmc1
    GPIO_PORTB_DEN_R = 0x03; // enables pb0 and pb1
    GPIO_PORTB_DIR_R = 0x01; // sets pb0 as output, pb1 as input

    //desired baud values
    uint16_t ibrd = 8; // 104 for 9600, 8 for 115200
    uint16_t fbrd = 44; // 11 for 9600, 44 for 115200

    UART1_CTL_R &= 0xFFFFFFFE; // disable UART1
    UART1_IBRD_R = ibrd; // write integer portion of BRD to IBRD
    UART1_FBRD_R = fbrd; // write fractional portion of BRD to FBRD
    UART1_LCRH_R = 0x60; // write serial communication parameters
    UART1_CC_R = 0x00; // use system clock as clock source

    UART1_ICR_R |= 0x10; // clear RX interrupt status flag
    UART1_IM_R |= 0x10; // enable RX interrupts

    //NVIC_PRI1_R |= 0xE0000; // Set UART1 pri to 1
    //NVIC_EN0_R |= 0x20; // enable interrupt 0
    NVIC_PRI4_R |= 0x00200000;
    NVIC_EN0_R |= 0x00000040;

    IntRegister(INT_UART1, UART1_Handler); // bind register interrupt requests to handler

    UART1_CTL_R |= 0x01; // enable UART1
    IntMasterEnable();
}

// blocking call to send 1 char over UART1
void uart_sendChar(char data)
{
    while (UART1_FR_R & 0x20)
        ; // wait until there is room to send data
    UART1_DR_R = data; // send data
}

/**
 * @brief sends an entire string of characters over UART1 module
 * @param data pointer to the first index of the string to be sent
 */
void uart_sendStr(const char *data)
{
    //until we reach a null character
    while (*data != '\0')
    {
        //send the current character
        uart_sendChar(*data);
        // increment the pointer.
        data++;
    }

}

// blocking call to receive over UART1, returns char data
char uart_receive(void)
{
    char data = 0; // initialize char data to null character
    while (UART1_FR_R & UART_FR_RXFE)
        ; // wait to receive
    data = (char) (UART1_DR_R & 0xFF); // mask the 4 error bits and grab 8 data bits
    return data; // returns received data
}

// interrupt handler for UART1
// part 3
void UART1_Handler(void)
{
    //received a byte
    if (UART1_MIS_R & UART_MIS_RXMIS)
    {
        cur = uart_receive();
        buffer[len] = cur; // add cur to buffer
        len++; // increment length
        buffer[len] = '\0'; // append null character
        if (cur == '\r') // if we get an ENTER
        {
            uart_sendChar('\n');
            buffer[len - 1] = '\0'; // set last character to null byte
            lcd_printf("%s", buffer); // print buffer
            len = 0; // reset length
            buffer[0] = '\0'; // reset buffer;
        }
        else if (len >= 20) // if we are at the end of the line
        {
            uart_sendChar(cur);
            lcd_printf("%s", buffer); // print buffer
            len = 0; // reset length
            buffer[0] = '\0'; // reset buffer;
        }
        else{// otherwise
            uart_sendChar(cur);
            lcd_printf("Index: %d\nCharacter: %c\n", len, cur); // print current character and its index
        }
        UART1_ICR_R |= UART_ICR_RXIC; //clear interrupt
    }
    //sent a byte
    else if (UART1_MIS_R & UART_MIS_TXMIS)
    {
        UART1_ICR_R |= UART_ICR_TXIC; //clear interrupt
    }

}
