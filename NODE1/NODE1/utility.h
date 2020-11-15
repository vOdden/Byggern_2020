#ifndef utility_H_
#define utility_H_

#define F_CPU 4915200UL
#include <util/delay.h>
#include <stdio.h>
#include "UART/UART.h"
#include "IO_map.h"
#define read_pin(port, pin) (port & (1 << pin))

typedef enum
{
	NEUTRAL = 0x0,
	LEFT 	= 0x1,
	RIGHT 	= 0x2,
	UP 		= 0x3,
	DOWN 	= 0x4
} Direction_t;


typedef struct
{
	int8_t x_pos;
	int8_t y_pos;
	Direction_t dir;
} Joystick_pos_t;


typedef struct
{
	uint8_t L_pos;
	uint8_t R_pos;
	uint8_t L_button;
	uint8_t R_button;
} Slider_pos_t;



void print_direction(Direction_t dir);

#endif
uint8_t R_pos;