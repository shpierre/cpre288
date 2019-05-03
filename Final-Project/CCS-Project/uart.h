/*
 * uart.h
 *
 *  Created on: Feb 20, 2019
 *      Author: Aidan
 */

#ifndef UART_H_
#define UART_H_

#include "timer.h"
#include <inc/tm4c123gh6pm.h>

extern volatile char buffer[21]; // create our line buffer
extern volatile char cur; // initialize current character to null
extern volatile int len; // number of chars printed this line

void uart_init(void);
void uart_sendChar(char data);
void uart_sendStr(const char *data);
char uart_receive(void);
void UART1_Handler(void);

#endif /* UART_H_ */
