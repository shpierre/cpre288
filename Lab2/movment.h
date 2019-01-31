/*
 * movment.h
 *
 *  Created on: Jan 31, 2019
 *      Author: aidans
 */

#ifndef MOVMENT_H_
#define MOVMENT_H_

void move_forward(oi_t *sensor, int centimeters);
void move_backward(oi_t *sensor, int centimeters);
void turn_right(oi_t *sensor, int degrees);
void turn_left(oi_t *sensor, int degrees);


#endif /* MOVMENT_H_ */
