#include "lcd.h"
#include "open_interface.h"
#include "timer.h"
#include "movement.h"

/**
 * main.c
 * Simple program to drive the roomba
 * @author aidans & apierce1
 * @date Jan 31, 2019
 */

int main(void){
    oi_t *sensor_data = oi_alloc(); //initialize program
	oi_init(sensor_data);
	
	move_forward(sensor_data,100); //checkpoint 1


	/*move_forward(sensor_data,50); //checkpoint 2
	turn_right(sensor_data,90);
	move_forward(sensor_data,50);
	turn_right(sensor_data,90);
	move_forward(sensor_data,50);
	turn_right(sensor_data,90);
	move_forward(sensor_data,50);
	turn_right(sensor_data,90);*/

	//checkpoint 3
	/*int dist = 0; // distance traveled
	int dist_to_travel = 200; // distance to travel
	float modifier = 0.67; // Modifier to account for the speed of the robot and the stopping distance
	while(dist < (dist_to_travel * modifier)){ // while we still have distance to travel
	    if(sensor_data->bumpLeft){ // if the left bumper is hit
	        move_backward(sensor_data, 15);//move back 15 cm
	        dist -= 13;
	        turn_right(sensor_data,85);//turn right
	        move_forward(sensor_data,25);//move forward 15 cm
	        turn_left(sensor_data,90);//turn left
	    }else if(sensor_data->bumpRight){ // if the right bumper is hit
	        move_backward(sensor_data, 15);//move back 15 cm
	        dist -= 13;
	        turn_left(sensor_data,90);//turn left
	        move_forward(sensor_data,25);//move forward 25
	        turn_right(sensor_data,85);//turn right
	    }else{//while the bumpers are not hit
	        move_forward(sensor_data,1);//move forward 1 cm at a time until we reach dist
	        dist++;//increment distance traveled
	    }
	}*/

	oi_free(sensor_data); //free program memory
    return 0;
}
