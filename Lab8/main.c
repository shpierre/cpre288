/**
 * main.c
 *
 * @author aidans & apierce1
 * @date Mar 27, 2019
 */
#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"
#include "servo.h"
#include "button.h"

int main(void)
{
    //inits
    lcd_init();
    button_init();
    init_button_interrupts();
    servo_init();
    timer_waitMillis(1000);
    //begin at 90 degrees
    servo_move(90);

    int sign = 1;
    float current = 90.0;
    char str1[80];
    //print starting position
    sprintf(str1, "Position: %.1f\nDirection: %d\n", current, sign);
    lcd_printf(str1); //display info on lcd screen
    while (1)
    {
        if (button_event == 1) //when a button is pressed
        {
            switch (button_num)
            //get the button number
            {
            default:
            case 0:
                break;
            case 1:
                //move 1 degree in the current direction
                current += sign * 1;
                if (current > 180)
                {
                    servo_move(180 - current);
                    current = 180;
                }
                else if (current < 0)
                {
                    servo_init();
                    current = 0;
                    goto print;
                }
                servo_move(1 * sign);
                timer_waitMillis(300);
                break;
            case 2:
                //move 2.5 degrees in the current direction
                current += sign * 2.5;
                if (current > 180)
                {
                    servo_move(180 - current);
                    current = 180;
                }
                else if (current < 0)
                {
                    servo_init();
                    current = 0;
                    goto print;
                }
                servo_move(2.5 * sign);
                timer_waitMillis(300);
                break;
            case 3:
                //move 5 degrees in the current direction
                current += sign * 5;
                if (current > 180)
                {
                    servo_move(180 - current);
                    current = 180;
                }
                else if (current < 0)
                {
                    servo_init();
                    current = 0;
                    goto print;
                }
                servo_move(5 * sign);
                timer_waitMillis(300);
                break;
            case 4:
                //switch direction
                sign = sign * -1;
                timer_waitMillis(300);
                break;
            case 5:
                //move to 0 degrees
                servo_init();
                current = 0;
                break;
            case 6:
                //move to 180 degrees
                servo_move(180 - current);
                current = 180;
                break;
            }
            print: sprintf(str1, "Position: %.1f\nDirection: %d\n", current,
                           sign);
            lcd_printf(str1); //display info on lcd screen
        }
    }
    return 0;
}
