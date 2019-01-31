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
    oi_setWheels(500,500); //move forward full speed, 500mm/s
	int sum = 0;
	while(sum < centimeters * 10){ //while we are still moving
		oi_update(sensor);
		sum += sensor -> distance; //add change in distance
	}
	oi_setWheels(0,0); //stop moving
    return;
}

void move_backward(oi_t *sensor, int centimeters){
    oi_setWheels(-500,-500); //move backward full speed, 500mm/s
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
    oi_setWheels(-200,200);
    int sum = 0;
    while(sum > degrees){
        oi_update(sensor);
        sum += sensor -> angle;
    }
    oi_setWheels(0,0);
    return;
}

void turn_left(oi_t *sensor, int degrees){
    oi_setWheels(200,-200);
    int sum = 0;
    while(sum > degrees){
        oi_update(sensor);
        sum += sensor -> angle;
    }
    oi_setWheels(0,0);
    return;
}