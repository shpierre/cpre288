#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"
#include "uart.h"
#include "lab5_scan_data.h"
#include "adc.h"
#include <math.h>
#include "servo.h"
#include "cyBot_Sonar.h"
#include "button.h"
#include "open_interface.h"
#include "movement.h"
#include "sweep.h"
#include "music.h"

/**git
 * main.c
 *
 */

bool killswitch = false;
char ch[5]; // character buffer for commands
// "w100\0" = move forward 100 cm
// "d45\0"  = rotate right 45 deg
// "e\0"    = toggle manual mode

void read()
{
    int i;
    for (i = 0; i < 5; i++)
    {
        char c = uart_receive();
        if (c == '\0')
        { //If we are at the end of the command
            ch[i] = c;
            break;
        }
        ch[i] = c; //assign received char to buffer ch
    }
}

int parseBuf()
{ // parses our char array and returns the integer
// value following the command identifier
    int i;
    int val;
    char tmp[4];
    for (i = 1; i < 4; i++)
    { //copy all but the first character to a temp array
        if (ch[i] == '\0')
        { //If we are at the end of the value
            tmp[i - 1] = '\0';
            break;
        }
        tmp[i - 1] = ch[i];
    }
    sscanf(tmp, "%d", &val);
    return val;
}

int main(void)
{
    //inits
    PortB_Init(); //Initialize port B
    ADC_init(); //Initialize ADC
    lcd_init(); //Initialize LCD
    uart_init();
    servo_init();
    ping_init();
    input_edge_timer_init();
    button_init();
    init_button_interrupts();
//       detect_object();

    //init open interface
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    oi_update(sensor_data);
    oi_setWheels(0, 0);

//    detect_object();

    while (!button_event && button_num != 6)
    {
        //Robot ignition using interrupts
    }
    char str[100];
    uart_sendStr("warning()");

    detect_object();
    while (!killswitch)
    {

        read();
        int x = parseBuf();
        switch (ch[0])
        {
        case 'w':
            if(x>30||x<0)
                break;
            move_forward(sensor_data, x);
            detect_object();
            break;
        case 's':
            if(x>30||x<0)
                break;
            move_backward(sensor_data, x);
            detect_object();
            break;
        case 'd':
            if(x>90||x<0)
                break;
            turn_right(sensor_data, x);
            detect_object();
            break;
        case 'a':
            if(x>90||x<0)
                return;
            turn_left(sensor_data, x);
            detect_object();
            break;
        case 'q':   //Stop the Robot
            stop_bot();
            break;
        case 'x':   // Killswitch
            stop_bot();
            killswitch = true;
            break;
        case 'm':
            play_song(sensor_data, 5);
            break;
        case 'p':
            detect_object();
            break;
        default:
            break;
        }

    }
    oi_free(sensor_data);
}
