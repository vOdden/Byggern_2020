
#include "sam.h"
#include "DAC.h"

//Utgang på shield skal være DAC1

void DAC_init(void)
{
	PMC->PMC_PCER1 |= (1<<6);			// ENABLE DACC CLOCK
	DACC->DACC_CHER = DACC_CHER_CH1; //Enable DAC1
	DACC->DACC_MR =  DACC_MR_USER_SEL_CHANNEL1;
}

void DAC_write(uint16_t data)
{
	DACC->DACC_CDR = data; //Send data to DAC
}