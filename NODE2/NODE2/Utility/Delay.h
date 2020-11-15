#ifndef Delay_H_
#define Delay_H_

#include "stdint.h"

//Delay uses SysTick!
void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);
void Delay(uint32_t t);

#endif /*Delay_h */