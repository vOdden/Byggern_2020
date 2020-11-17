/**
 * @file Servo.c
 * @author TTK4155 2020 Group 28
 * @date 17 nov 2020
 * @brief File containing diver for Servo
 */
#include "Servo.h"
#include "../Utility/Utility.h"

extern volatile pos_t position;
volatile int8_t Servo_x_CAL = 0;//offset from 0
volatile int8_t Servo_y_CAL = 0;//offset from 0

void Servo_calibrate(void)
{
	Delay(20);
	Servo_x_CAL = position.x;
	Servo_y_CAL = position.y;
}


void Servo_init(void)
{
	PWM_init();
	Servo_calibrate();
}


void Servo_move(uint8_t pos, uint8_t channel)//old function 
{
	uint16_t x = 900 + (pos * 4.72); //
	
	PWM_set_duty_cycle(x,channel);
}


uint16_t Servo_calc_D(int8_t x, int8_t y)//makes sure the value is in the range -100 to 100. Only x is used
{
	x *= -1;
	
	if(x < -100)
	{
		x = -100;
	}
	else if(x > 100)
	{
		x = 100;
	}
	
	return 1500 + x*6;
}

void Servo_set_position(int8_t x, int8_t y)//Only x is used
{
	int16_t x_16 = (x - 2 *Servo_x_CAL);
	int16_t y_16 = (y - 2 *Servo_y_CAL);
	
	
	if(x_16 < -100)
	{
		x_16 = -100;
	}
	else if (x_16 > 100)
	{
		x_16 = 100;
	}
	
	
	uint16_t t = Servo_calc_D(x_16, y_16);
	
	PWM_set_duty_cycle(t, SERVO_PWM_CHANNEL);
}



int8_t Servo_get_D(uint8_t channel)// get Duty-cycle
{
	switch(channel)
	{
		case 5:
			return ((REG_PWM_CDTY5/3) - 1500)/4.72;
		break;
		
		case 6:
			return ((REG_PWM_CDTY6/3) - 1500)/4.72;
		break;
		
		default:
			return 0;
		break;
		
	}
}
