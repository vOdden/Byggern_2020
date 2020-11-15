
#ifndef Slider_H_
#define Slider_H_


#include <stdint.h>
#include "../utility.h"



void slider_init(void);
Slider_pos_t Slider_get_position(void);
int Slider_get_button(Direction_t dir);


#endif /* Slider_H_*/