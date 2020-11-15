#include "sam.h"
#include <stdio.h>
#include "Timer.h"


void TC0_init(uint32_t periode) //Periode i us
{
	NVIC_EnableIRQ(TC0_IRQn); // Enable TC0 IRQ
	REG_PMC_PCER0 |= PMC_PCER0_PID27; // Enable TC0 clock in PMC
	//mainclock div 8 => 10.5MHz
	REG_TC0_CMR0 |= TC_CMR_TCCLKS_TIMER_CLOCK2;
	//compare resets counter and clock
	REG_TC0_CMR0 |= TC_CMR_CPCTRG;
	//enable RC compare interrupt
	REG_TC0_IER0 |= TC_IER_CPCS;
	//enable tc clock
	REG_TC0_CCR0 |= TC_CCR_CLKEN;

	uint32_t tick = periode / 0.095;

	REG_TC0_RC0 = tick;
}

void TC1_init(uint32_t periode) //Periode i us
{
	NVIC_EnableIRQ(TC3_IRQn); // Enable TC3 IRQ
	REG_PMC_PCER0 |= PMC_PCER0_PID30; // Enable TC3 clock in PMC
	//mainclock div 8 => 10.5MHz
	REG_TC1_CMR0 |= TC_CMR_TCCLKS_TIMER_CLOCK2;
	//compare resets counter and clock
	REG_TC1_CMR0 |= TC_CMR_CPCTRG;
	//enable RC compare interrupt
	REG_TC1_IER0 |= TC_IER_CPCS;
	//enable tc clock
	REG_TC1_CCR0 |= TC_CCR_CLKEN;

	uint32_t tick = periode / 0.095; // hvert tick == 1/f sekund. Dvs => tid = antall tick / f => antall tick = tid * f
	// Vil ha 2 us. Timeren går i en periode på 0.095us
	// Da må vi vente i 2/0.0.95 tick ~= 5
	REG_TC1_RC0 = tick;
}

void TC2_init(uint32_t periode) //Periode i us
{
	NVIC_EnableIRQ(TC6_IRQn); // Enable TC3 IRQ
	REG_PMC_PCER1 |= PMC_PCER1_PID33; // Enable TC3 clock in PMC
	//mainclock div 8 => 10.5MHz
	REG_TC2_CMR0 |= TC_CMR_TCCLKS_TIMER_CLOCK2;
	//compare resets counter and clock
	REG_TC2_CMR0 |= TC_CMR_CPCTRG;
	//enable RC compare interrupt
	REG_TC2_IER0 |= TC_IER_CPCS;
	//enable tc clock
	REG_TC2_CCR0 |= TC_CCR_CLKEN;

	uint32_t tick = periode / 0.095; // hvert tick == 1/f sekund. Dvs => tid = antall tick / f => antall tick = tid * f
	// Vil ha 2 us. Timeren går i en periode på 0.095us
	// Da må vi vente i 2/0.0.95 tick ~= 5
	REG_TC2_RC0 = tick;
}

void Timer_start(int timer)
{
	switch(timer)
	{
		case TIMER0:
			REG_TC0_CCR0 = TC_CCR_CLKEN;
			REG_TC0_CCR0 |= TC_CCR_SWTRG;
		break;

		case TIMER1:
			REG_TC1_CCR0 = TC_CCR_CLKEN;
			REG_TC1_CCR0 |= TC_CCR_SWTRG;
		break;
		
		case TIMER2:
			REG_TC2_CCR0 = TC_CCR_CLKEN;
			REG_TC2_CCR0 |= TC_CCR_SWTRG;
		break;
		
	}
}

void Timer_stop(int timer)
{
	switch(timer)
	{
		case TIMER0:
			REG_TC0_CCR0 |= TC_CCR_CLKDIS;
		break;

		case TIMER1:
			REG_TC1_CCR0 |= TC_CCR_CLKDIS;
		break;

		case TIMER2:
			REG_TC2_CCR0 |= TC_CCR_CLKDIS;
		break;
	}
}
