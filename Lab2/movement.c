/*
 * movement.c
 *
 *  Created on: Jan 31, 2019
 *      Author: aidans
 */

#include "lcd.h"
#include "open_interface.h"
#include "timer.h"

void move_forward(oi_t *sensor, int centimeters){
    oi_setWheels(300,300); //move forward full speed, 500mm/s
	int sum = 0;
	while(sum < centimeters * 10){ //while we are still moving
		oi_update(sensor);
		sum += sensor -> distance; //add change in distance
	}
	oi_setWheels(0,0); //stop moving
    return;
}

void move_backward(oi_t *sensor, int centimeters){
    oi_setWheels(-300,-300); //move backward
	int sum = centimeters * 10;
	while(sum > 0){ //while we are still moving
		oi_update(sensor);
		sum += sensor -> distance; //add change in distance
	}
	oi_setWheels(0,0); //stop moving
    return;
}

void turn_right(oi_t *sensor, int degrees){
	degrees = -1 * degrees;
    oi_setWheels(-150,150);//turn right
    int sum = 0;
    while(sum > degrees){//while we have not turned degrees
        oi_update(sensor);
        sum += sensor -> angle;//update sum with amount we have turned
    }
    oi_setWheels(0,0);//stop moving
    return;
}

void turn_left(oi_t *sensor, int degrees){
    oi_setWheels(150,-150);//turn left
    int sum = 0;
    while(sum < degrees){//while we have not turned degrees
        oi_update(sensor);
        sum += sensor -> angle;//update sum with amount we have turned
    }
    oi_setWheels(0,0);
    return;//stop moving
}
