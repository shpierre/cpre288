

#ifndef PING_H_
#define PING_H_
#include "timer.h"

extern volatile int pingStat;
extern volatile uint32_t startVal;
extern volatile uint32_t endVal;
extern volatile int diff;
extern volatile float time_sec;
extern volatile float Sonar_Distance;
extern volatile int overflow;

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inc/tm4c123gh6pm.h>
#include "timer.h"

void input_edge_timer_init(void);
void ping_init(void);
void send_pulse(void);
void ping_handler(void);
#endif /* PING_H_ */
