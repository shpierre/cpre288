/**
 * main.c
 *
 * @author aidans
 * @date Apr 2, 2019
 */
#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"
#include "servo.h"
#include "button.h"

extern volatile unsigned current_time;
extern volatile unsigned last_time;
extern volatile int update;
extern volatile double dist;
/*
 extern volatile char buffer[21]; // create our line buffer
 extern volatile char cur; // initialize current character to null
 extern volatile int len; // number of chars printed this line
 */
volatile int overflows = 0;

#define MAX_OBJ 10;

int getIRDist(int i) // MAY NEED TO BE RECALIBRATED!
{
    int d;
    // calibrate our IR distance to our line of best fit from our test data
    d = 240321 * pow(i, -1.284) + 1;
    return d;
}

int closeEnough(int ir, int sonar)
{
    int diff = abs(ir - sonar);
    return diff <= 2 ? 1 : 0;
}

struct obj
{
    int distance;
    int angle;
    int radialSize;
    int width;
};

int main(void)
{
    //inits
    adc_init();
    lcd_init();
    uart_init();
    button_init();
    init_button_interrupts();
    ping_init();
    servo_init();

    //wait 1s after initializing
    timer_waitMillis(1000);

    uint32_t ir[91]; // Array of ir distances
    uint32_t sonar[91]; // Array of sonar distances
    uint32_t objCount = 0; // num detected objects
    uint32_t objWidth = 0; // object width
    uint8_t smallestObj = 0; // index of smallest obj in our objects array
    int onObj = 0; // flag for if we are still reading the same object
    int jump = 10; // jump in distance required to detect the "edge" of an obj
    struct obj objects[MAX_OBJ];
    //initialize first object in our array
    objects[smallestObj].width = 0;
    objects[smallestObj].distance = 0;

    int i;

    for(i = 0; i < 91, i++)
    {
        //get sonar distance
        send_pulse();
        ping_read();
        sonar[i] = dist;
        //get ir distance
        int fifo_result = adc_read();
        ir[i] = getIRDist(fifo_result);
        //wait 200 ms and move 2 degrees
        timer_waitMillis(200);
        servo_move(2);
    }

    uart_sendStr("Degrees\tIR Dist (cm)\tSonar Dist (cm)\n\r");
    for(i = 0; i < 91, i++)
    {
        char str1[80];
        sprintf(str1, "%d\t%d\t%d\n\r", i*2, ir[i], sonar[i]);
        lcd_printf(str1); //display info on lcd screen
        uart_sendStr(str1);// sent string to putty using uart
    }

    return 0;
}
