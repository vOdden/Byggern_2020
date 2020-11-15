#include "sam.h"
#include "Delay.h"

uint32_t us_tick = 0;
uint32_t ms_tick = 0;




void Delay_us(uint32_t us)
{	
	if(us > 199728) //Max us @ 84MHz
	{
		us = 199728;
	}
	SysTick->LOAD = 84 * us;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk; //Enable Systick, 	Clock = MCK = 84MHz => 84 tick = 1us. Kan telle til 16 777 216/84 = 199 728us
	while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
	{
		//Wait for systick count flag to reach zero
	}
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //Disable Systick	
}

void Delay_ms(uint32_t ms)
{	
	if(ms > 159) //Max ms @ 10.5MHz
	{
		for(int i = 0; i < ms; i++)
		{
			Delay_ms(1);	
		}
		return;	
	}
	SysTick->LOAD = 10500 * ms;
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk; //Enable Systick, 	Clock = MCK = 10.5MHz => 10.5 tick = 1us => 10500 = ms. Kan telle til 16 777 216/10500 = 
	while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
	{
		//Wait for systick count flag to reach zero
	}	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //Disable Systick		
	
	
}

void Delay(uint32_t t)
{
	Delay_ms(t);
}

