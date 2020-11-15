
#ifndef Joystick_H_
#define Joystick_H_


#include <stdint.h>
#include "../utility.h"

/*
typedef enum
{
	NEUTRAL = 0x0,
	LEFT 	= 0x1,
	RIGHT 	= 0x2,
	UP 		= 0x3,
	DOWN 	= 0x4
} Direction_t;
*/

void Joystick_init(void);
void Joystick_Calibrate(void);
Joystick_pos_t Joystick_get_position(void);
Direction_t Joystick_get_direction(void);
volatile int8_t Joystick_read_button(void);


#endif /* Joystick_H_ */