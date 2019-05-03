
#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

#ifndef SERVO_H_
#define SERVO_H_
#define MAX 50833 //180 on left bot 1:49533      bot 9:50833
#define MIN 22100 //            bot 1:22800      bot 9:22100
extern volatile int sign;
#include "button.h"
#include "ping.h"

void servo_init(void);
void servo_move(float degrees);
void servo_set(float degrees);
#endif /* SERVO_H_ */
