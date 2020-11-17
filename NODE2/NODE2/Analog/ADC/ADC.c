/**
 * @file ADC.c
 * @author TTK4155 2020 Group 28
 * @date 17 nov 2020
 * @brief File containing drivers for external, memory mapped, ADC.
 *
 */

#include "sam.h"
#include <stdio.h>
#include "ADC.h"
// AD0 = PA2 = A7 on arduino shield


//ADC max clock frequency == 22MHz => prescaler minimum 4 (21MHz)

/**
 * @brief Initialization function for the ADC
 *
 * @param  channel
 * @retval none
 */ 
void ADC_init(uint8_t channel)
{
	PMC->PMC_PCER1 |= (1<<5);			// ENABLE ADC CLOCK
	ADC->ADC_MR = ADC_MR_PRESCAL(10) | ADC_MR_FREERUN_ON;	// ADC CLOCK = MCK / (prescal+1)*2 => 21MHz
	ADC->ADC_CHER = channel;		//ENABLE ADC channels vi bruker kanal 0!
	ADC->ADC_CR = ADC_CR_START;
}
/**
 * @brief Receiving data from the ADC
 *
 * @param  channel
 * @retval ADC
 */ 
uint16_t ADC_read(uint8_t channel)
{			
	return ADC->ADC_CDR[0]; //READ DATA REGISTER
}
