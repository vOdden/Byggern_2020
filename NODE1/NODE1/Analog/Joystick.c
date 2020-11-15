#include "Joystick.h"
#include "ADC.h"
#include "avr/io.h"
#include "avr/interrupt.h"
#include "stdlib.h"
#include "../bit_macros.h"
#include "math.h"
#include "../IO_map.h"
//#include "../utility.h"

#define DIRECTION_THRESHOLD     80
#define NEUTRAL_THRESHOLD   	50

static uint8_t center_x;
static uint8_t center_y;

static volatile uint8_t Joystick_button = 0;

ISR(INT2_vect)
{
	if(Joystick_button == 0)
	{
		Joystick_button++;
	}
}	

void Joystick_init(void)
{
    ADC_init();
    Joystick_Calibrate();

	//joystick == PE0, input med pullup
	DDRE &= ~(1 << JOYSTICK_PIN);
	PORTE |= 1 << JOYSTICK_PIN;
	
	cli();
	
	//interrupt on falling edge. PE0
	set_bit(MCUCR, ISC01);
	clear_bit(MCUCR, ISC00);
	//Interrupt enable PE0.
	set_bit(GICR, INT2);
	
	sei();
}

void Joystick_Calibrate(void)
{
	uint8_t ADC_data[4];
	ADC_get_conversion(ADC_data);
	
    center_x = ADC_data[ADC_JOYSTICK_X];
    center_y = ADC_data[ADC_JOYSTICK_Y];
}

Joystick_pos_t  Joystick_get_position(void)
{
	uint8_t ADC_data[4];
	ADC_get_conversion(ADC_data);
	
	uint8_t x_pos16 = (ADC_data[ADC_JOYSTICK_X]);//-center_x);
    uint8_t y_pos16 = (ADC_data[ADC_JOYSTICK_Y]);//-center_y);

	
	Joystick_pos_t pos;
	static Direction_t previous_dir = NEUTRAL;

	pos.x_pos = x_pos16 - 128;
	pos.y_pos = y_pos16 - 128;
	
	uint8_t x_pos_abs = abs(pos.x_pos);
	uint8_t y_pos_abs = abs(pos.y_pos);	
	
	
	//Høyre blir opp
	//Opp blir nøytral
	//Venstre og ned er rikting.
	//Nøytral er riktig
	
//	printf("x_pos: %d", pos.x_pos);
//	printf("y_pos: %d \n", pos.y_pos);
	
	if(x_pos_abs > y_pos_abs) // Horistontal
	{
		if(x_pos_abs > DIRECTION_THRESHOLD)
		{
			if(pos.x_pos > 0)
			{
				previous_dir = RIGHT; //Nei
			}
			else
			{
				previous_dir = LEFT; //
			}
		}
		else if(x_pos_abs < NEUTRAL_THRESHOLD)
		{
			previous_dir = NEUTRAL;
		}
	}
	else //vertikal
	{
		if(y_pos_abs > DIRECTION_THRESHOLD)
		{
			if(pos.y_pos > 0)
			{
				previous_dir = UP; //
			}
			else
			{
				previous_dir = DOWN;
			}
		}
		else if(y_pos_abs < NEUTRAL_THRESHOLD)
		{
			previous_dir = NEUTRAL;
		}

	}
    pos.dir = previous_dir;
	
//	print_direction(pos.dir);

	return pos;
}


Direction_t Joystick_get_direction(void)
{
    static Direction_t previous_dir = NEUTRAL;
    
    Joystick_pos_t pos = Joystick_get_position();
            
	uint8_t x_pos_abs = abs(pos.x_pos);
	uint8_t y_pos_abs = abs(pos.y_pos);
	
	if(x_pos_abs > y_pos_abs)
	{
		if(x_pos_abs > DIRECTION_THRESHOLD)
		{
			if(pos.x_pos > 0)
			{
				previous_dir = RIGHT;
			}
			else
			{
				previous_dir = LEFT;
			}
		}
		else if(x_pos_abs < NEUTRAL_THRESHOLD)
		{
			previous_dir = NEUTRAL;
		}
	}
	else
	{
		if(y_pos_abs > DIRECTION_THRESHOLD)
		{
			if(pos.y_pos > 0)
			{
				previous_dir = UP;
			}
			else
			{
				previous_dir = DOWN;
			}
		}
		else if(y_pos_abs < NEUTRAL_THRESHOLD)
		{
			previous_dir = NEUTRAL;
		}

	}	
	return previous_dir;
}

volatile int8_t Joystick_read_button(void)
{
	volatile int8_t retval = Joystick_button;
	Joystick_button = 0;
    return retval;
}

