
#include <avr/io.h>

#include "ADC.h"
#include "../utility.h"
#include "../PWM.h"

#define F_CPU 4915200UL
#include <util/delay.h>


void ADC_init(void)
{
	PWM_init(); //Start klokke
}

uint8_t ADC_read(uint8_t channel)
{
	uint8_t data[4];
	ADC_get_conversion(data);

	return data[channel];
}

void ADC_get_conversion(uint8_t* data)
{
	//start write-sequence of ADC. Toggle WR to start conversion
	volatile uint8_t* ext_ADC = (uint8_t *) 0x1400;
	*ext_ADC = 0;
	
	//_delay_us(10);
	
	while(!read_pin(PINB, PB1))
	{
	}

	for(uint8_t i = 0; i < 4; i++)
	{
		data[i] = ext_ADC[0];
	}
}
