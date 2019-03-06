/**
 * main.c
 *
 * @author aidans & apierce1
 * @date Mar 5, 2019
 */
#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"
#include "button.h"
#include "uart.h"

extern volatile char buffer[21]; // create our line buffer
extern volatile char cur; // initialize current character to null
extern volatile int len; // number of chars printed this line

/**
 * main.c
 */
int main(void)
{

}
