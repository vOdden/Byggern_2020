/**
 * @file PWM.c
 * @author TTK4155 2020 Group 28
 * @date 17 nov 2020
 * @brief File containing drivers for PWM.
 */
#include "PWM.h"


/**
 * @brief Initialization function for PWM
 *
 * @param  none
 * @retval none
 */ 
void PWM_init(void)
{
	PIOC->PIO_PDR |= PIO_PDR_P19; //Disable PIO control
	PIOC->PIO_ABSR |= PIO_ABSR_P19; //PIO multiplexer set to B
	PIOC->PIO_PDR |= PIO_PDR_P18; //Disable PIO control
	PIOC->PIO_ABSR |= PIO_ABSR_P18; //PIO multiplexer set to B
	
	PWM->PWM_WPCR = PWM_WPCR_WPKEY(0x50574D)| PWM_WPCR_WPRG0 |PWM_WPCR_WPRG2|PWM_WPCR_WPRG3;//Enables writing to register group 0,2,3
	
	PWM->PWM_CLK = PWM_CLK_DIVA(28)| PWM_CLK_PREA(0); //PWM_CLK = 3MHZ
	PMC->PMC_PCER1 |= (1 << 4); //Enable PWM clock
	REG_PWM_CMR5 = 0xB | 1 << 9; //CLKA, Polatity start high
	REG_PWM_CPRD5 = 60000; // set period to 20ms. 60 000 = 20ms * 1000 * 3
	REG_PWM_CDTY5 = 4500; // set duty-cycle to 1,5 ms. 4500 = 1,5 * 1000 * 3
	REG_PWM_CMR6 = 0xB | 1 << 9; //CLKA, Polatity start high
	REG_PWM_CPRD6 = 60000; // set period to 20ms. 60 000 = 20ms * 1000 * 3
	REG_PWM_CDTY6 = 4500; // set duty-cycle to 1,5 ms. 4500 = 1,5 * 1000 * 3
	PWM->PWM_ENA |= PWM_ENA_CHID5; // enable PWM channel 5
	PWM->PWM_ENA |= PWM_ENA_CHID6; // enable PWM channel 6
}

/**
 * @brief Initialization function for the external, memory mapped, ADC.
 * Function start a PWM signal to be used as ADC clock.
 * makes sure the duty-cycle is in the range 0.9ms to 2,1 ms
 * @param  us, channel
 * @retval none
 */ 
void PWM_set_duty_cycle(uint16_t us, uint8_t channel) 
{
	if(us < 900)
	{
		us = 900; //Lowest duty-cycle
	}
	else if(us > 2100)
	{
		us = 2100; //highest duty-cycle
	}
	if (channel == 5)
	{
		REG_PWM_CDTY5 = 3*us; 
	}
	else if(channel == 6)
	{
		REG_PWM_CDTY6 = 3*us; 
	}
}
