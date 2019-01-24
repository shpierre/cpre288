#include "lcd.h"

/**
 * main.c
 *
 * Simple 'Hello, world program'.
 * This program prints "Hello, world" to the LCD screen
 * @author
 * @date
 */

int main(void){
    //Initialize the LCD. This also clears the screen
    lcd_init();

    lcd_rotatingBanner("Microcontrollers are lots of fun!");

    return 0;
}
