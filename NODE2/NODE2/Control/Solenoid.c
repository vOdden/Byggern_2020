
#include "Solenoid.h"
#include <stdio.h>
#include "sam.h"
#include "../Utility/Utility.h"


void Solenoid_init(void)
{
	//use PB26 on board => Pin 22
	PIOB->PIO_PER |= PIO_PER_P26; //enable PIO controll on pin 22
	PIOB->PIO_OER |= PIO_OER_P26; //enable output on pin 22
	PIOB->PIO_CODR |= PIO_CODR_P26; //set the output to 0
	PIOB->PIO_PUDR |= PIO_PUDR_P26; // disable pull-up on PB26(pin 22)
	
	TC1_init(SHOT_DURATION * 1000);
} 

void Solenoid_shoot(void)
{
	set_pin(SOLENOID_PORT, SOLENOID_PIN);
	Timer_start(TIMER1);
}


/*			TC0 TC1 TC2 TC3	TC4 TC5 TC6	 				MCU -> NVIC
//  TC1		CH0 CH1 CH2									PERIPHERAL
//  TC2                 CH0 CH1 CH2						PERIPHERAL
//  TC3                               CH0

*/
#include "../Control/Motor.h"
void TC3_Handler(void)
{
	uint32_t status = REG_TC1_SR0;//read status register - this clears interrupt flags
	clear_pin(SOLENOID_PORT, SOLENOID_PIN);
	Timer_stop(TIMER1);
}
	
	