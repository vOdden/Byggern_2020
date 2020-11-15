
#include "sam.h"
#include <stdio.h>
#include "ADC.h"
// AD0 = PA2 = A7 on arduino shield


//ADC max clock frequency == 22MHz => prescaler minimum 4 (21MHz)
void ADC_init(uint8_t channel)
{
	PMC->PMC_PCER1 |= (1<<5);			// ENABLE ADC CLOCK
	ADC->ADC_MR = ADC_MR_PRESCAL(10) | ADC_MR_FREERUN_ON;	// ADC CLOCK = MCK / (prescal+1)*2 => 21MHz
	ADC->ADC_CHER = channel;		//ENABLE ADC channels vi bruker kanal 0!
	ADC->ADC_CR = ADC_CR_START;
}

uint16_t ADC_read(uint8_t channel)
{			
	return ADC->ADC_CDR[0]; //READ DATA REGISTER
}
