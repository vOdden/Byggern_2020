/**
 * @file Solenoid.c
 * @author TTK4155 2020 Group 28
 * @date 17 nov 2020
 * @brief File containing drivers for solenoid
 *
 */

#include "Solenoid.h"
#include <stdio.h>
#include "sam.h"
#include "../Utility/Utility.h"


void Solenoid_init(void)
{
	//use PB26 on board => Pin 22 on shield
	PIOB->PIO_PER |= PIO_PER_P26; //enable PIO controll on pin 22
	PIOB->PIO_OER |= PIO_OER_P26; //enable output on pin 22
	PIOB->PIO_CODR |= PIO_CODR_P26; //set the output to 0
	PIOB->PIO_PUDR |= PIO_PUDR_P26; // disable pull-up on PB26(pin 22)
	
	TC1_init(SHOT_DURATION * 1000);
} 

void Solenoid_shoot(void)//start procedure for shooting solenoid
{
	set_pin(SOLENOID_PORT, SOLENOID_PIN);
	Timer_start(TIMER1);
}

#include "../Control/Motor.h"
void TC3_Handler(void)// stops solenoid and stops timer. 
{
	uint32_t status = REG_TC1_SR0;//read status register - this clears interrupt flags
	clear_pin(SOLENOID_PORT, SOLENOID_PIN);
	Timer_stop(TIMER1);
}
	
	
