#include "lcd.h"
#include "open_interface.h"
#include "timer.h"
#include "movement.h"

/**
 * main.c
 * Simple program to drive the roomba
 * @author aidans
 * @date Jan 31, 2019
 */

int main(void){
    oi_t *sensor_data = oi_alloc(); //initialize program
	oi_init(sensor_data);
	
	move_forward(sensor_data,100); //checkpoint 1

/*
	move_forward(sensor_data,50); //checkpoint 2
	turn_right(sensor_data,90);
	move_forward(sensor_data,50);
	turn_right(sensor_data,90);
	move_forward(sensor_data,50);
	turn_right(sensor_data,90);
	move_forward(sensor_data,50);
	turn_right(sensor_data,90);
*/

	oi_free(sensor_data); //free program memory
    return 0;
}
