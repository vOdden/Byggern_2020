/**
 * @file DAC.c
 * @author TTK4155 2020 Group 28
 * @date 17 nov 2020
 * @brief File containing drivers for the DAC.
 *
 */
#include "sam.h"
#include "DAC.h"

//Utgang på shield skal være DAC1
/**
 * @brief Initialization function for the DAC
 * 
 * @param  none
 * @retval none
 */ 
void DAC_init(void)
{
	PMC->PMC_PCER1 |= (1<<6);			// ENABLE DACC CLOCK
	DACC->DACC_CHER = DACC_CHER_CH1; //Enable DAC1
	DACC->DACC_MR =  DACC_MR_USER_SEL_CHANNEL1;
}
/**
 * @brief Send data to the DAC
 * 
 * @param  data
 * @retval none
 */ 
void DAC_write(uint16_t data)
{
	DACC->DACC_CDR = data; //Send data to DAC
}
