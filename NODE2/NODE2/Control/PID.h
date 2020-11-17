/**
 * @file PID.c
 * @author TTK4155 2020 Group 28
 * @date 17 nov 2020
 * @brief File containing drivers for the PID controller
 * @see http://nimron.net/P1000/
 */


#ifndef PID_H_
#define PID_H_

#include "../Utility/Utility.h"
#include "Motor.h"

typedef struct
{
	float Kp;
	float Ki;
	int32_t I;
	float Kd;
	float dt;
	float pre_error;
}PID_struct;


int16_t PID_encoder_init(void);
void PID_init();
int16_t PID_encoder_init(void);
void PID_stop(void);
void PID_start(void);
void PID_reg(PID_struct* pid, uint8_t ref_pos);


#endif
