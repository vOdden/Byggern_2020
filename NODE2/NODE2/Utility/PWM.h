#ifndef PWM_H_
#define PWM_H_

#include "sam.h"

void PWM_init(void);
void PWM_set_duty_cycle(uint16_t us, uint8_t channel);

#endif