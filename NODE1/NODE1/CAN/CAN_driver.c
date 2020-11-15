
#include "CAN_driver.h"
#include "../bit_macros.h"

#include <avr/io.h>
#define F_CPU 4915200UL
#include <util/delay.h>
#include "stdio.h"

void mcp_activate()
{
	/* Activate Chip Select */
	clear_bit(PORTB, PB4);
}

void mcp_deactivate()
{
	/* Deactivate Chip Select */
	set_bit(PORTB, PB4);
}

void mcp_2515_set_mode(uint8_t mode)
{
	mcp_2515_write(MCP_CANCTRL, mode);
}

uint8_t mcp_2515_init(uint8_t mode)
{	
	uint8_t val;
	SPI_init();
	
	mcp_2515_reset();
	
	val = mcp_2515_read(MCP_CANSTAT);
	uint8_t mode_bits = (val & MODE_MASK);
	if(mode_bits != MODE_CONFIG)
	{
		printf("MCP2515 is NOT in Configuration mode after reset! Its config bits are %x\n", mode_bits);
		return 1;
	}
	
	mcp_2515_set_mode(mode);
	
	val = mcp_2515_read(MCP_CANSTAT);
	mode_bits = (val & MODE_MASK);
	
	if(mode_bits != mode)
	{	
		printf("MCP2515 is NOT in correct mode after reset! Its config bits are %x\n", mode_bits);
		printf("\n!\n");
		return 1;
	}
	
	
	return 0;
}

void mcp_2515_reset()
{
	mcp_activate();
	SPI_write(MCP_RESET);
	mcp_deactivate();
	_delay_ms(10);	//a small delay after mcp reset
}

uint8_t mcp_2515_read(uint8_t address)
{
	mcp_activate();
	
	uint8_t data;
	
	SPI_write(MCP_READ);
	SPI_write(address);
	data = SPI_read();
	
	mcp_deactivate();
	
	return data;
}


void mcp_2515_write(uint8_t address, uint8_t data) 
{
	mcp_activate();
	
	SPI_write(MCP_WRITE);
	SPI_write(address);
	SPI_write(data);
	
	mcp_deactivate();
}

void mcp_2515_request_to_send(uint8_t buffer) 
{
	mcp_activate();
	
	SPI_write(buffer);
	
	mcp_deactivate();
}

void mcp_2515_bit_modify(uint8_t adress, uint8_t mask, uint8_t data) 
{
	mcp_activate();
	
	SPI_write(MCP_BITMOD);
	SPI_write(adress);
	SPI_write(mask);
	SPI_write(data);
	
	mcp_deactivate();
}

uint8_t mcp_2515_read_status() 
{
	mcp_activate();
	
	SPI_write(MCP_READ_STATUS);
	uint8_t status = SPI_read();
	
	mcp_deactivate();
	
	return status;
}