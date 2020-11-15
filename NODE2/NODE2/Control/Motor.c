
#include "sam.h"

#include "motor.h"

#include "../Utility/Utility.h"
#include "../Analog/DAC/DAC.h"
#include <stdio.h>
#include <math.h>

#define motor_vel_max 0.7

float saturate(float f, float min, float max)
{
	if(f > max)
	{
		return max;
	}
	else if(f < min)
	{
		return min;
	}
	else
	{
		return f;
	}	
}

void Motor_enable(uint8_t enable)
{
    if(enable)
	{
        set_pin(MOTOR_EN_PORT, MOTOR_EN_PIN);
    }
	else
	{    
        clear_pin(MOTOR_EN_PORT, MOTOR_EN_PIN);
	}
}

void Encoder_output_enable(uint8_t enable)
{
    if(enable)
	{
		clear_pin(ENCODER_OE_PORT, ENCODER_OE_PIN);
	}
	else
	{
		clear_pin(ENCODER_OE_PORT, ENCODER_OE_PIN);
	}
}

void Encoder_reset(void)
{
    clear_pin(ENCODER_RST_PORT, ENCODER_RST_PIN);
    Delay_ms(20);
    set_pin(ENCODER_RST_PORT, ENCODER_RST_PIN);
	Delay_ms(20);
}

void Motor_init(void)
{
    PMC->PMC_PCER0 |= PMC_PCER0_PID13 | PMC_PCER0_PID14; 
	PIOC->PIO_PER |= 0x1FE; //enable PC1-PC8
	PIOC->PIO_ODR |= 0x1FE; //sets PC1-PC8 to input
	PIOD->PIO_PER |= PIO_PER_P0 | PIO_PER_P1 | PIO_PER_P2 | PIO_PER_P9 | PIO_PER_P10;	//enable PD9 and PD10
	PIOD->PIO_OER |= PIO_OER_P0 | PIO_OER_P1 | PIO_OER_P2 | PIO_OER_P9 | PIO_PER_P10;	//enable output on PD9 and PD10
	PIOD->PIO_CODR |=  PIO_SODR_P9; // sets enable low
	PIOD->PIO_SODR |= PIO_SODR_P0 | PIO_SODR_P1;	// sets reset and OE pin high
	
    Encoder_output_enable(1);
    Encoder_reset();
    Motor_set_speed(0);
}


void Motor_set_direction(Direction_t dir)
{
    switch(dir)
	{
		case LEFT:  
			clear_pin(MOTOR_DIR_PORT, MOTOR_DIR_PIN);   
		break;
		
		case RIGHT: 
			set_pin(MOTOR_DIR_PORT, MOTOR_DIR_PIN);
		break;
		
		default: 
		
		break;
    }
}

void Motor_set_speed(uint16_t speed)
{
    DAC_write(speed);
}


void Motor_velocity(float vel)
{
//    vel = saturate(vel, -motor_vel_max, motor_vel_max);
//    motor_speed((uint8_t)(fabs(vel) * 250.0));
//    motor_direction(vel < 0 ? left : right);
}


int16_t Encoder_read(void)
{
  	int16_t Data;
  	
	set_pin(ENCODER_RST_PORT, ENCODER_RST_PIN);
	  
  	clear_pin(ENCODER_OE_PORT, ENCODER_OE_PIN); //Set OE low
  	
	clear_pin(ENCODER_SEL_PORT, ENCODER_SEL_PIN);
	
	Delay_us(30);
	
	Data = read_pin(ENCODER_OUTPUT_PORT, ENCODER_OUTPUT_MASK) << 7;
	  
//	printf("%x \r", (uint16_t)Data); // ff ff ff 00 
	  
	set_pin(ENCODER_SEL_PORT, ENCODER_SEL_PIN);	
	
	Delay_us(30);

  	Data |= read_pin(ENCODER_OUTPUT_PORT, ENCODER_OUTPUT_MASK) >> 1; //0xFF
//	printf("%x \r", (uint16_t)Data); // -1
	    
  	PIOD->PIO_SODR = PIO_SODR_P0; //set OE HIGH
	
	Encoder_reset();
 
 // 	printf("encoder data: %x \r", Data);
  	
	return Data;
}



//Right == negativ direction
//Left == Positive direction
int16_t Encoder_limit(void)
{
	Motor_set_direction(RIGHT);
	Motor_set_speed(1500);
	Motor_enable(ENABLE);
	Delay(1000);
	
	Encoder_reset();
	
	Motor_set_direction(LEFT);
	
	Delay(1000);
	
	int16_t data = Encoder_read();
	
	printf("%d \r", data);
	
	Motor_enable(DISABLE);
	Motor_set_speed(0);

	return data;
}