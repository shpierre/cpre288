/**
 * main.c
 *
 * @author aidans & apierce1
 * @date Feb 20, 2019
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
 * Prints the button that has been pressed
 */
int main(void)
{
    // required initializations
    uart_init();
    lcd_init();

    while(true);
    // part 1 & 2
    /*char buffer[21]; // create our line buffer
     char cur = '\0'; // initialize current character to null
     int len = 0; // number of chars printed this line
     while (true)
     {
     cur = uart_receive(); // update character received
     buffer[len] = cur; // add cur to buffer
     len++; // increment length
     buffer[len] = '\0'; // append null character
     if (cur == '\r') // if we get an ENTER
     {
     uart_sendChar('\n');
     buffer[len - 1] = '\0'; // set last character to null byte
     lcd_printf("%s", buffer); // print buffer
     len = 0; // reset length
     buffer[0] = '\0'; // reset buffer;
     }
     else if (len >= 20) // if we are at the end of the line
     {
     uart_sendChar(cur);
     lcd_printf("%s", buffer); // print buffer
     len = 0; // reset length
     buffer[0] = '\0'; // reset buffer;
     }
     else // otherwise
     {
     uart_sendChar(cur);
     lcd_printf("Index: %d\nCharacter: %c\n", len, cur); // print current character and its index
     }
     }*/

}
