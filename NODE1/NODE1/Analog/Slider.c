
#include <avr/io.h>

#include "Slider.h"
#include "ADC.h"

void slider_init(void)
{
    ADC_init();
}    

Slider_pos_t Slider_get_position(void)
{
	Slider_pos_t position;
	uint8_t ADC_data[4]; // 0-255
	
	ADC_get_conversion(ADC_data); //0-255
		
	position.L_pos = ADC_data[ADC_SLIDER_L]; //-128 +127 int8_t
	position.R_pos = ADC_data[ADC_SLIDER_R];
		
	return position;	
}

int Slider_get_button(Direction_t dir)
{
	switch(dir)
	{
		case RIGHT:
			return read_pin(PORTD,4);
		break;
		
		case LEFT:
			return read_pin(PORTD,5);
		break;
		
		default:
			return 0;
		break;
	}
}
