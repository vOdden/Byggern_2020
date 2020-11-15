
#include "../memory_layout.h"
#include "SRAM.h"
#include "math.h"
#include "stdlib.h"
#include "stdio.h"

#include "SRAM.h"
#include <avr/io.h>
#include <avr/interrupt.h>


void SRAM_init(void)
{
	MCUCR |= 1 << SRE; //Enable XMEM
	SFIOR |= 1 << XMM2 | 0 << XMM1 | 0 << XMM0;  //Release PC7-4 from adress buss (JTAG).
}

//Writes a set of data to SRAM.
//adress from 0x1800 to 0x1FFF.
//return 0 if write succesfull
uint8_t SRAM_write(uint8_t *data, uint16_t adress, uint8_t size)
{
	//Check that adress is in range of external memory
	if(adress < 0x1800)
	{
		return 1;
	}
	else if (adress + size > 0x1FFF)
	{
		return 1;
	}
	
	//write data to SRAM
	//volatile uint8_t * ext_ram = ( uint8_t *) adress;
	for(uint8_t i = 0; i < size; i++)
	{
		ext_ram[i] = data[i];
	}
	return 0;
}

uint8_t SRAM_read(uint8_t *data, uint16_t adress, uint8_t size)
{
	//Check that adress is in range of external memory
	if(adress < 0x1800)
	{
		return 1;
	}
	else if (adress + size-1 > 0x1FFF)
	{
		return 1;
	}
	//read data from SRAM
	//volatile uint8_t * ext_ram = (uint8_t *) adress;
	for(uint8_t i = 0; i < size; i++)
	{
		data[i] = ext_ram[i];
	}
	return 0;
}

void SRAM_test ( void )
{
	//volatile char * ext_ram = ( char *) 0x1800; //Start address for the SRAM
	//uint16_t ext_ram_size = 0x800;
	uint16_t write_errors = 0 ;
	uint16_t retrieval_errors = 0 ;
	printf ( "Starting SRAM test...\n\n" ) ;
	// rand() stores some internal state, so calling this function in a loop  will
	// yield different seeds each time ( unless srand() is called before this function)
	uint16_t seed = rand();
	// Write phase : Immediately check that the correct value was stored
	srand(seed);
	for(uint16_t i = 0 ; i < ext_ram_size ; i++)
	{
		uint8_t some_value = rand ( ) ;
		ext_ram [ i ] = some_value ;
		uint8_t retreived_value = ext_ram [ i ] ;
		if ( retreived_value != some_value )
		{
			printf ("Write phase error : ext_ram[%4d] = %02X ( should be %02X) \n" , i , retreived_value , some_value );
			write_errors++;
		}
	}
	// Retrieval phase : Check that no values were changed during or after the - write phase
	srand ( seed ) ;
	// reset the PRNG to the state it had before the write phase
	for ( uint16_t i = 0 ; i < ext_ram_size ; i++) {
		uint8_t some_value = rand ( ) ;
		uint8_t retreived_value = ext_ram [ i ] ;
		if( retreived_value != some_value ) {
			printf("Retrieval phase error : ext ram[%4d ] = %02X ( should be %02X) \n" , i , retreived_value , some_value );
			retrieval_errors++;
		}
	}
	printf ( "SRAM test completed with \n %4d errors in write phase and \n%4d  - errors in retrieval phase \n\n" , write_errors , retrieval_errors ) ;
}
