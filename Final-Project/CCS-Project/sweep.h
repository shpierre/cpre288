/*
 * sweep.h
 *
 *  Created on: Apr 3, 2019
 *      Author: zchiggs
 */

#ifndef SWEEP_H_
#define SWEEP_H_
extern volatile int degrees;
#define N 20

void sweep(void);
void sweep_half(void);
void detect_object(void);
float CentimeterConv(int ADCValue);
extern volatile float IR_Distance;
extern volatile int degrees_start_IR[N];
extern volatile int degrees_end_IR[N];
extern volatile int distance[N];
extern volatile int num_object;
extern volatile int detected;
#endif /* SWEEP_H_ */

