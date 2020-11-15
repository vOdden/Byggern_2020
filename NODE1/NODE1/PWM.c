
#include "PWM.h"
#include "avr/io.h"


void PWM_init(void)
{
	TCCR0 = 1 << 6 | 1 << 3;
	TCCR0 |= 3 << COM00;
	TCCR0 |= 1;
	
	TCNT0 = 20;
	OCR0 = 10;
}