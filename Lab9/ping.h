/*
 * ping.h
 *
 *  Created on: Mar 10, 2019
 *      Author: Aidan
 */
#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>

#ifndef PING_H_
#define PING_H_

extern volatile unsigned current_time;
extern volatile unsigned last_time;
extern volatile int update;

void TIMER3B_Handler(void);
void portb_init(void);
void timer_init(void);
void ping_init(void);
void sendPulse(void);
void ping_read(void);

#endif /* PING_H_ */
