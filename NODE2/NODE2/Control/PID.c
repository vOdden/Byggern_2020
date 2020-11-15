#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "PID.h"

#define Kp_init 1
#define Ki_init 0.5
#define Kd_init 0
#define dt_init 0.05	// in sec   // 50 ms

static int16_t encoder_max = 0;
static float encoder_middle;
extern volatile pos_t position;

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




PID_struct PID;

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


int16_t PID_encoder_init(void)
{
	Motor_init();
	
	Motor_enable(1);

	encoder_max = Encoder_limit();
	
	encoder_middle = encoder_max/2;
	
	return encoder_max;
}


void PID_stop(void)
{
	Motor_enable(DISABLE);
	Timer_stop(TIMER0);
}

void PID_start(void)
{
	Motor_enable(ENABLE);
	Timer_start(TIMER0);
}

//Wanted position is a number from 0-255 (uint8_t)

//teller opp når går til venstre, ned til høyre
//null er lengst til høyre
//encoder max er lengst til venstre


//venstre													høyre 
/*---------------------------------------------------------------------*/
//encoder_max                                                          0   
//                                x
//							~-encoder_max/2
//                                                                       

int16_t current_pos()
{
	static int16_t pos = 0;
	
	pos += Encoder_read();
	
	return pos;
}


// u = Kp +Ki + Kd
void PID_regulator()
{
	int32_t pos = current_pos();
	
	int16_t e = (int16_t)position.slider + (255*pos)/encoder_max;  // e = (ønsket posisjon) - (faktisk posisjon)
	
	printf("%d \t", position.slider);
	
	static int i = 0;

	PID.I = PID.I * e * PID.dt; // alle er enig
	
	if(e < 1) 
	{
		PID.I = 0; //reset integralet hvis vi er veldig nærme ønsket
	}
	
	float D = (PID.pre_error - e) / PID.dt;
	
	int16_t u = PID.Kp * e + PID.Ki * PID.I;// + PID.Kd * D;	// Kd = 0
	
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
		Motor_set_speed(abs(u)*10 + 1000);
		//Motor_set_speed(2000);
		Motor_enable(ENABLE);
	}
	else
	{
		Motor_enable(DISABLE);
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


void TC0_Handler(void)
{
	uint32_t status = REG_TC0_SR0;//read status register - this clears interrupt flags

	PID_regulator();
}