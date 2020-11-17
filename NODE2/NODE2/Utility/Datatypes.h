/**
 * @file Datatypes.h
 * @author TTK4155 2020 Group 28
 * @date 17 nov 2020
 * @brief File containing drivers for datatypes
 *
 */

#ifndef Datatypes_H_
#define Datatypes_H_

typedef enum
{
	NEUTRAL = 0x0,
	LEFT = 0x1,
	RIGHT = 0x2,
	UP = 0x3,
	DOWN = 0x4
} Direction_t;

typedef struct {
	Direction_t dir;
	int8_t x;
	int8_t y;
	uint8_t button;
	uint8_t slider;
	Direction_t slider_dir;
}pos_t;




#endif /* Datatypes_H_ */
