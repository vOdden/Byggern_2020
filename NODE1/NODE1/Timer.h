/*
 * tim.h
 *
 * Created: 17.09.2020 13:54:19
 *  Author: Sigurdod
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include "avr/io.h"
#include "avr/interrupt.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
void Timer_init(void);
void Timer1_start(void);
void Timer1_stop(void);
void Timer3_start(void);
void Timer3_stop(void);


#endif /* TIM_H_ */