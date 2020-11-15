#include "Timer.h"

volatile uint32_t ms_tick = 0;  //Updates every 100ms
volatile uint8_t NODE2_READY_FLAG = 0;
extern volatile uint8_t ready;

//f_cpu == 49152000Hz
void Timer_init(void) //timer 1
{
	TCCR1A = 0x00;
	TCCR1B = (1 << CS10) | (1 << CS12); //Prescaler == 1024 => clk = 4.8kHz => t = 208us. Vil ha 100ms => 480 tick
	TIMSK |= 1 << TOIE1;
	TCNT1 = 65536 - 480;
	
	TCCR3A = 0x00;
	TCCR3B = (1 << CS30) | (1 << CS32); //Prescaler == 1024 => clk = 4.8kHz => t = 208us. Vil ha 100ms => 480 tick
	ETIMSK |= 1 << TOIE3;
	TCNT3 = 65536 - 4800; //every second
	
	sei();
}

void Timer1_start(void) //timer 1
{
	ms_tick = 0;
	TCCR1B = (1 << CS10) | (1 << CS12); //Prescaler == 1024 => clk = 4.8kHz => t = 208us. Vil ha 100ms => 480 tick
}

void Timer1_stop(void) //timer 3
{
	TCCR1B &= ~(1 << CS10) | (1 << CS12); //Prescaler == 1024 => clk = 4.8kHz => t = 208us. Vil ha 100ms => 480 tick
}

void Timer3_start(void) //timer 3
{
	TCCR3B = (1 << CS30) | (1 << CS32); //Prescaler == 1024 => clk = 4.8kHz => t = 208us. Vil ha 100ms => 480 tick
}

void Timer3_stop(void) //timer 3
{
	TCCR3B &= ~(1 << CS30) | (1 << CS32); //Prescaler == 1024 => clk = 4.8kHz => t = 208us. Vil ha 100ms => 480 tick
}


ISR(TIMER1_OVF_vect)
{
		ms_tick++;
		TCNT1 = 65536 - 480;
}

ISR(TIMER3_OVF_vect)
{
	if(ready)
	{
		ready = 0;
		NODE2_READY_FLAG = 1;
	}
	else
	{
		NODE2_READY_FLAG = 0;
	}
	
	//printf("READY IRQ \n");
	
	TCNT3 = 65536 - 4800;
}

