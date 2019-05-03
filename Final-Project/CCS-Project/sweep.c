#include "servo.h"
#include "uart.h"
#include "ping.h"
#include "timer.h"
#include "button.h"
#include "adc.h"
#include "sweep.h"
/*
 * sweep.c
 *
 *  Created on: Apr 3, 2019
 *      Author: zchiggs
 */

#define N 20
volatile int degrees;
volatile float IR_Distance;
volatile int degrees_start_IR[N];
volatile int degrees_end_IR[N];
volatile int distance[N];
volatile int num_object;
volatile int detected;

//bot 9
float CentimeterConv(int ADCValue)
{
    float value = 0;
    value = 59789 * pow(ADCValue, -1.086);
    return value;

}
void sweep(void)
{
    detected = 0;
    int AdcResult = 0; //Variable to hold raw output from the ADC
    int dist_before_avg = 0;
    //Varialbe to hold distance in CM
    int i = 0;
    int z = 0;
    char str[50];
    num_object = 0;
    for (i = 0; i < 90; i++)
    {

        send_pulse(); // possibly need a wait
        if (!(ADC0_ACTSS_R & 0x00010000))
        { //If the ADC is not busy start conversion
            ADC0_PSSI_R |= 0x1;
        }

        if (ADC0_RIS_R & 0x00000001)
        { //If conversion is complete Store result
            AdcResult = ADC0_SSFIFO0_R;
            ADC0_ISC_R |= 0x1;
        }

        IR_Distance = CentimeterConv(AdcResult);

        degrees = ((TIMER1_TBMATCHR_R - MIN) * pow((MAX - MIN), -1)) * 180; // calculate degrees from current Match Value
        sprintf(str, "%-20d%-20f%-20f\r\n", degrees, IR_Distance,
                Sonar_Distance);
        uart_sendStr(str);
        timer_waitMillis(30);
        if ((IR_Distance < 80) && (detected == 0))
        {
            num_object++;
            degrees_start_IR[num_object] = degrees;
            detected = 1;
            dist_before_avg = 0;

        }
        if ((IR_Distance > 100) && (detected == 1))
        {
            degrees_end_IR[num_object] = degrees;
            detected = 0;
        }

        if (detected == 1)
        {
            dist_before_avg = dist_before_avg + Sonar_Distance;
            z++;
        }
        if (detected == 0 && z > 0)
        {
            distance[num_object] = (dist_before_avg * pow(z, -1));
            z = 0;
        }

        servo_move(2);

    }
    num_object = 0;
    for (i = 0; i < 90; i++)
    {

        send_pulse(); // possibly need a wait
        if (!(ADC0_ACTSS_R & 0x00010000))
        { //If the ADC is not busy start conversion
            ADC0_PSSI_R |= 0x1;
        }

        if (ADC0_RIS_R & 0x00000001)
        { //If conversion is complete Store result
            AdcResult = ADC0_SSFIFO0_R;
            ADC0_ISC_R |= 0x1;
        }

        IR_Distance = CentimeterConv(AdcResult);

        degrees = ((TIMER1_TBMATCHR_R - MIN) * pow((MAX - MIN), -1)) * 180; // calculate degrees from current Match Value
        sprintf(str, "%-20d%-20f%-20f\r\n", degrees, IR_Distance,
                Sonar_Distance);
        uart_sendStr(str);
        timer_waitMillis(30);
        if ((IR_Distance < 80) && (detected == 0))
        {
            num_object++;
            degrees_start_IR[num_object] = degrees;
            detected = 1;
            dist_before_avg = 0;

        }
        if ((IR_Distance > 100) && (detected == 1))
        {
            degrees_end_IR[num_object] = degrees;
            detected = 0;
        }

        if (detected == 1)
        {
            dist_before_avg = dist_before_avg + Sonar_Distance;
            z++;
        }
        if (detected == 0 && z > 0)
        {
            distance[num_object] = (dist_before_avg * pow(z, -1));
            z = 0;
        }
        servo_move(-2);

    }
    timer_waitMillis(20);

}
void sweep_half(void)
{ //set up for data collection
    int AdcResult = 0; //Variable to hold raw output from the ADC
    int dist_before_avg = 0;
    //Varialbe to hold distance in CM
    int i = 0;
    int z = 0;
    num_object = 0;
    detected = 0;
    for (i = 0; i < 90; i++)
    {

        send_pulse(); // possibly need a wait
        if (!(ADC0_ACTSS_R & 0x00010000))
        { //If the ADC is not busy start conversion
            ADC0_PSSI_R |= 0x1;
        }

        if (ADC0_RIS_R & 0x00000001)
        { //If conversion is complete Store result
            AdcResult = ADC0_SSFIFO0_R;
            ADC0_ISC_R |= 0x1;

        }

        IR_Distance = CentimeterConv(AdcResult);

        degrees = ((TIMER1_TBMATCHR_R - MIN) * pow((MAX - MIN), -1)) * 180; // calculate degrees from current Match Value

        timer_waitMillis(30);

        if ((abs(IR_Distance - Sonar_Distance) <= 6) && (detected == 0))
        {
            num_object++;
            degrees_start_IR[num_object] = degrees;
            detected = 1;
            dist_before_avg = 0;
        }
        if ((abs(IR_Distance - Sonar_Distance) > 6) && (detected == 1))
        {
            detected = 0;
            degrees_end_IR[num_object] = degrees - 2; //don't include last one
        }
        if (detected == 1)
        { //collect distance while object is detected
            dist_before_avg = dist_before_avg + Sonar_Distance;
            z++;
        }
        if (detected == 0 && z > 0)
        { //stop collecting distance and average
            distance[num_object] = (dist_before_avg / z);
            z = 0;
        }
        timer_waitMillis(75);
        servo_move(2); //move 2 degrees
    }
    servo_set(0);

}
void detect_object(void)
{

    char str[50];
    overflow = 0;
    sign = 1;
    int i = 0;
    int v = 0;
    int x = 0;
    float w[N];
    int dist2[N];
    int start2[N];
    int end2[N];
    int rad2[N];
    float width[N];
    float corrected_angle[N];
    float angle_b[N];
    int radial_distance[N];
    int m = 0;
    int type[N];
    servo_set(90);
    x = 0;
    v = 0;

    servo_set(0);
    sweep_half(); //run sweep and gather data
    for (i = 1; i <= num_object; i++)
    { //iterate through and error check objects
        radial_distance[i] = abs(degrees_start_IR[i] - degrees_end_IR[i]); //subtract initial and final degree values
        if ((radial_distance[i] >= 5 && radial_distance[i] < 100)
                && ((distance[i] > 0) && (distance[i] < 50)))
        { //error check (not too big or small)
            x = i - v; //object #
            m++;
            corrected_angle[i] = (radial_distance[i]) * 3.1415926535
                    * pow(180, -1); //put angle in radians
            angle_b[i] = (180 - radial_distance[i]) * pow(2, -1) * 3.1415926535
                    * pow(180, -1); //calculate iscololes triangle angles
            width[i] = (distance[i]) * sin(corrected_angle[i])
                    * pow(sin(angle_b[i]), -1); //calculate width by law of sines
                    //width[i] = (distance[i])*corrected_angle[i];  //can use if issues
            w[x] = width[i]; //make arrays of values for actual object
            dist2[x] = distance[i];
            start2[x] = degrees_start_IR[i];
            end2[x] = degrees_end_IR[i];
            rad2[x] = radial_distance[i];

        }
        else
        { //count # errors
            v++;
        }
    }
    //}

    for (i = 1; i <= x; i++)
    { //iterate to find smallest width & save object #
        if (w[i] < 7.5)
        {
            type[i] = 0;
        }
        else
        {

            type[i] = 1;

        }
        sprintf(str, "object(%d,%d,%d)\n\r", dist2[i], ((start2[i]+end2[i])/2),
                type[i]);
        uart_sendStr(str);
    } // 1 is a "large object". 0 is a "small" object

}

