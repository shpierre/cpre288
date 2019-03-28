/**
 * main.c
 *
 * @author aidans & apierce1
 * @date Mar 10, 2019
 */
#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"
#include "ping.h"


/**
 * main.c
 */
extern volatile unsigned current_time;
extern volatile unsigned last_time;
extern volatile int update;
volatile int overflows = 0;
volatile int CHECKPOINT = 3;

int main(void)
{
    //Inits
    lcd_init();
    ping_init();
    //Constant polling
    while (true)
    {
        if (CHECKPOINT == 1)
            //Send trigger pulse for checkpoint 1
            sendPulse();
        if (CHECKPOINT >= 2)
            //Read data from the Ping sensor
            ping_read();
        //Wait 300 ms
        timer_waitMillis(300);
    }
    return 0;
}
