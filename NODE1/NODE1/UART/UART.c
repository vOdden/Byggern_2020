/*
 * UART.c
 *
 * Created: 22.10.2020 08:51:19
 *  Author: Sigurdod
 */ 
#include "UART.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "stdio.h"


void UART_init(uint32_t ubrr)
{
	//Set baud rate
	UBRR0H = ubrr >> 8;
	UBRR0L = ubrr;
	//enable TX/RX
	UCSR0B = 1 << RXEN0 | 1<< TXEN0;
	//Set frame format
	UCSR0C = 1 << URSEL0 | 1 << USBS0 | 3 << UCSZ00; //
		
	fdevopen(UART_putc, UART_recieve);
}

void UART_puts(char *data, uint8_t data_size)
{
	for(int i = 0; i < data_size - 1; i++)
	{
		while(!(UCSR0A & (1<<UDRE0)))
		{
		}
		UDR0 = data[i];
	}
}

void UART_putc(char data)
{
	while(!(UCSR0A & (1<<UDRE0)))
	{
	}
	UDR0 = data;
}

char UART_recieve(void)
{
	//wait for data to be recieved
	while(! (UCSR0A & (1<<RXC0)))
	{
	}
	return UDR0;
}
