/**
 * @file main.c
 * @author TTK4155 2020 Group 28
 * @date 17 nov 2020
 * @brief TTK4155 term project Node 1 main file.
 * Contains a program for controlling the motor with a PID regulator.
*/ 






#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "PID.h"
// Defining initial values for the PID controller 
#define Kp_init 1	// proportial gain
#define Ki_init 0.5	// Integral gain
#define Kd_init 0	// Derivative gain - is set to zero, for simplicity and faster system for the desired controller.
#define dt_init 0.05	// in sec   // 50 ms

static int16_t encoder_max = 0;
static float encoder_middle;
extern volatile pos_t position;

// Print out direction of the joystick
void print_direction(Direction_t dir)
{
	switch(dir)
	{
		case NEUTRAL:
			printf("NEUTRAL \r");
		break;
		
		case UP:
			printf("UP \r");
		break;
		
		case DOWN:
			printf("DOWN \r");
		break;
		
		case LEFT:
			printf("LEFT \r");
		break;
		
		case RIGHT:
			printf("RIGHT \r");
		break;

		default:
			printf("DIR ERROR \r");
		break;		
	}
}



// PID struct 
PID_struct PID;
/**
 * @brief Assigning the initial PID values to the PID struct
 *
 * @param  none
 * @retval none
 */  
void PID_init()
{
	PID.Kp = Kp_init;
	PID.Ki = Ki_init;
	PID.I = 0;
	PID.Kd = Kd_init;
	PID.dt = dt_init;
	PID.pre_error = 0;
	
	TC0_init(PID.dt * (uint32_t)1000000); //Update 250ms
}
/**
 * @brief Initializing the PID encoder
 *
 * @param  none
 * @retval none
 */  
int16_t PID_encoder_init(void)
{
	Motor_init();
	
	Motor_enable(1);

	encoder_max = Encoder_limit();
	
	encoder_middle = encoder_max/2;
	
	return encoder_max;
}

/**
 * @brief Stops the servo
 *
 * @param  none
 * @retval none
 */   
void PID_stop(void)
{
	Motor_enable(DISABLE);
	Timer_stop(TIMER0);
}
/**
 * @brief Starts the servo
 *
 * @param  none
 * @retval none
 */   
void PID_start(void)
{
	Motor_enable(ENABLE);
	Timer_start(TIMER0);
}

/**
 * @brief Receiving current position of the servo.
 *
 * @param  none
 * @retval none
 */                                                                      
int16_t current_pos()
{
	static int16_t pos = 0;
	
	pos += Encoder_read();
	
	return pos;
}


/**
 * @brief Main function for the PID regulator.
 *
 * @param  none
 * @retval none
 */ 
void PID_regulator()
{
	int32_t pos = current_pos();	// Receiving position of the servo
	
	int16_t e = (int16_t)position.slider + (255*pos)/encoder_max;  // Calculating the error, e = (Desired position) - (actual position)
	
	printf("%d \t", position.slider);	// Print out the position of the slider
	
	static int i = 0;	

	PID.I = PID.I * e * PID.dt; // Calculating the integrator part of the PID
	
	if(e < 1) 
	{
		PID.I = 0; // Reset the integral when getting close to the desired position
	}
	
	float D = (PID.pre_error - e) / PID.dt;	// Calculating the derivative part, but is set to zero.
	
	int16_t u = PID.Kp * e + PID.Ki * PID.I;// Calculating the PI regulator
	
	PID.pre_error = e;	
	
	if(abs(u) > 4)
	{
		if(u < 0)
		{
			Motor_set_direction(LEFT);	
		}
		else
		{
			Motor_set_direction(RIGHT);
		}
		
		//0 - 4095
		Motor_set_speed(abs(u)*10 + 1000);	// Sets the motor with respect to the PID controller
		//Motor_set_speed(2000);
		Motor_enable(ENABLE);	// Enabling the motor
	}
	else
	{
		Motor_enable(DISABLE);	// Disabling the motor 
	}
		

	if(i >= 5)
	{
	//	printf("%d \r", u);		
		i = 0;
	}
	else
	{
		i++;
	}

}

/**
 * @brief read status register - this clears interrupt flags
 *
 * @param  none
 * @retval none
 */  
void TC0_Handler(void)
{
	uint32_t status = REG_TC0_SR0;

	PID_regulator();
}
