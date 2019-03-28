/*
 * servo.h
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

#ifndef SERVO_H_
#define SERVO_H_

void servo_init(void);
int servo_move(float degrees);

#endif /* SERVO_H_ */
