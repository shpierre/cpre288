/*
 * movement.c
 *
 *  Created on: Feb 6, 2019
 *      Author: Conor Albinger, Adis Osmankic
 */
#include "open_interface.h"
#include "movement.h"
#include "lcd.h"
#include "uart.h"

#define TURN_SPEED 50
#define STRAIGHT_SPEED 100
#define BUMP_DIST 15
#define BUMP_TURN_RIGHT 15
#define BUMP_TURN_LEFT 30
#define CLIFF_BACKUP_DIST 10
#define CLIFF_TURN_RIGHT 15
#define CLIFF_TURN_LEFT 30
#define TAPE_TURN_RIGHT 60
#define TAPE_TURN_LEFT 45

volatile int correction;
volatile int back=10;

void move_forward(oi_t *sensor, int centimeters)
{

    sensor = oi_alloc();

    oi_init(sensor);
    int sum = 0;
    oi_setWheels(STRAIGHT_SPEED, STRAIGHT_SPEED); //move forward full speed

    int milim = 10 * centimeters;

    int F_left = 0;
    int F_right = 0;
    int left = 0;
    int right = 0;

    char str[100];

    while (sum < milim)
    {
        oi_update(sensor);

        F_left = sensor->cliffFrontLeftSignal;
        F_right = sensor->cliffFrontRightSignal;
        left = sensor->cliffLeftSignal;
        right = sensor->cliffRightSignal;

        if (F_left <= 700 || F_right <= 700 || left <= 700 || right <= 700)
        {

            oi_setWheels(0, 0);
			back=sum/10;
            sprintf(str, "move(%d)\n\r", sum / 10);
            uart_sendStr(str);
            uart_sendStr("cliff()\n\r");
            move_cliff(sensor, F_left, F_right, left, right);
            return;
        }

        if (F_left >= 2700 || F_right >= 2700 || left >= 2700 || right >= 2700)
        {

            oi_setWheels(0, 0);
			back=sum/10;
            sprintf(str, "move(%d)\n\r", sum / 10);
            uart_sendStr(str);
            uart_sendStr("edge()\n\r");
            move_tape(sensor, F_left, F_right, left, right);
            oi_setWheels(0, 0);
            return;
        }

        if (sensor->bumpLeft && sensor->bumpRight)
        {

            oi_setWheels(0, 0);
			back=sum/10;
            sprintf(str, "move(%d)\n\r", sum / 10);
            uart_sendStr(str);
            uart_sendStr("bump(2)");
            move_backward(sensor, back);
//            turn_right(sensor, BUMP_TURN_RIGHT);
            oi_setWheels(0, 0);
            return;
        }

        if (sensor->bumpLeft)
        {

            oi_setWheels(0, 0);
			back=sum/10;
            sprintf(str, "move(%d)\n\r", sum / 10);
            uart_sendStr(str);
            uart_sendStr("bump(0)");
            move_backward(sensor, back);
//            turn_right(sensor, BUMP_TURN_RIGHT);
            oi_setWheels(0, 0);
            return;
        }
        else if (sensor->bumpRight)
        {

            oi_setWheels(0, 0);
			back=sum/10;
            sprintf(str, "move(%d)\n\r", sum / 10);
            uart_sendStr(str);
            uart_sendStr("bump(1)");
            move_backward(sensor, back);
//            turn_left(sensor, BUMP_TURN_LEFT);
            oi_setWheels(0, 0);
            return;
        }

        sum += sensor->distance;

    }
    sprintf(str, "move(%d)\n\r", centimeters);
    uart_sendStr(str);
    oi_setWheels(0, 0);
}

void turn_left(oi_t *sensor_data, int degrees)
{
    char str[100];
    sprintf(str, "turn(%d)", degrees);
    uart_sendStr(str);

    oi_init(sensor_data);
    int ang = 0;
    oi_setWheels(TURN_SPEED, -TURN_SPEED); //turn left
    if (degrees == 90)
    {
        correction = 26;
    }
    if (degrees == 45)
    {
        correction = 15;
    }
    if (degrees == 30)
    {
        correction = 11;
    }
    if (degrees == 15)
    {
        correction = 8;
    }
    while (ang < degrees - correction)
    { //correction of 10 deg while angel is less than ~90 deg
        oi_update(sensor_data);
        ang += sensor_data->angle;
        //lcd_printf("Turned: %d",  ang ); //print out the angle
    }

    oi_update(sensor_data);
    oi_setWheels(0, 0); //stop
}

void turn_right(oi_t *sensor_data, int degrees)
{
    char str[100];
    sprintf(str, "turn(%d)", -degrees);
    uart_sendStr(str);

    oi_init(sensor_data);
    int ang = 0;
    oi_setWheels(-TURN_SPEED, TURN_SPEED); //turn to the right
    if (degrees == 90)
    {
        correction = 26;
    }
    if (degrees == 45)
    {
        correction = 15;
    }
    if (degrees == 30)
    {
        correction = 11;
    }
    if (degrees == 15)
    {
        correction = 8;
    }
    while (ang > correction - degrees)
    { //loop until we reach 90 deg with a 10 deg offset to correct the robot
        oi_update(sensor_data);
        ang += sensor_data->angle;
        //lcd_printf("Turned: %d",  ang );
    }

    oi_update(sensor_data);
    oi_setWheels(0, 0); //stop

}

//moves the roomba backwards to the desired amount
void move_backward(oi_t *sensor, int centimeters)
{
    sensor = oi_alloc();
    centimeters -= 2 * centimeters; //converts centimeters to negative
    char str[100];
    sprintf(str, "move(%d)", centimeters);
    uart_sendStr(str);
    oi_init(sensor);
    int sum = 0; //keeps track of the distance traveled so far
    oi_setWheels(-STRAIGHT_SPEED, -STRAIGHT_SPEED); //sets wheels to backwards
    while (sum > 10 * centimeters)
    {

        oi_update(sensor);
        sum += sensor->distance; //same logic as move
    }
    oi_setWheels(0, 0); //stop
}

void move_cliff(oi_t *sensor, int F_left, int F_right, int left, int right)
{

    sensor = oi_alloc();
    oi_init(sensor);
    move_backward(sensor, back);
//    if (F_left <= 700 || left <= 700)
//    {
//        turn_right(sensor, CLIFF_TURN_RIGHT);
//    }
//    else
//    {
//        turn_left(sensor, CLIFF_TURN_LEFT);
//    }
}

void move_tape(oi_t *sensor, int F_left, int F_right, int left, int right)
{

    sensor = oi_alloc();
    oi_init(sensor);
//    if (left >= 2650 || F_left >= 2650)
//    {
//        turn_right(sensor, TAPE_TURN_RIGHT);
//    }
//    else
//    {
//        turn_left(sensor, TAPE_TURN_LEFT);
//    }
}
void stop_bot()
{
    oi_setWheels(0, 0);
}

