/**
 * main.c
 *
 * @author aidans & apierce1
 * @date Feb 13, 2019
 */

#include "lcd.h"
#include "button.h"

/**
 * main.c
 * Prints the button that has been pressed
 */
int main(void) {
    // Hint: You may need to use pointers to return the button that has been pressed
    lcd_init();
    button_init();
    init_button_interrupts();

    //checkpoint 1
    /*while(1==1){
        lcd_printf("%d", button_getButton());
    }*/

    //checkpoint 3
    while(1==1){
        if(button_event == 1)
            lcd_printf("%d", button_num);
    }
}
