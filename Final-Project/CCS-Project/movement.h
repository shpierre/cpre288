/*
 * movement.h
 *
 *  Created on: Feb 6, 2019
 *      Author: conor
 */
#include "open_interface.h"
#ifndef CODE_FILES_MOVEMENT_H_
#define CODE_FILES_MOVEMENT_H_

extern volatile int correction;
extern volatile int back;

void move_forward(oi_t *sensor, int centimeters); //moves roomba forward a set distance
void turn_right(oi_t *sensor, int degrees); //Turns roomba right a set number of degrees
void turn_left(oi_t *sensor, int degrees); //Turns roomba left a set number of degrees
void move_backward(oi_t *sensor, int centimeters); // moves roomba backwards a set distance
void move_cliff(oi_t *sensor, int F_left, int F_right, int left, int right); //stops the robot when a cliff appears
void move_tape(oi_t *sensor, int F_left, int F_right, int left, int right);
void stop_bot(void);

#endif /* CODE_FILES_MOVEMENT_H_ */
