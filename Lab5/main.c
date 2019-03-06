/**
 * main.c
 *
 * @author aidans & apierce1
 * @date Feb 27, 2019
 */
#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"
#include "button.h"
#include "uart.h"
#include "lab5_scan_data.h"

extern volatile char buffer[21]; // create our line buffer
extern volatile char cur; // initialize current character to null
extern volatile int len; // number of chars printed this line

/**
 * main.c
 */
int main(void)
{

    uart_init();
    struct reading readings[181];
    get_reading("dataset3.csv", readings); // read in data from csv

    int degrees;
    uart_sendStr("\rDegrees \tIT Distance (cm) \tSonar Distance (cm)\n\r"); // send line to putty
    char str[80];
    for(degrees = 0; degrees < 181; degrees++){
        sprintf(str, "%d \t\t%.6f \t\t%.6f\n\r", degrees, readings[degrees].ir_distance, readings[degrees].sonar_distance); // scan data to string
        uart_sendStr(str); // sent string to putty using uart
    }
}
