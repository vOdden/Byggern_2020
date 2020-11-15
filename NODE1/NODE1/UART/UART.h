/*
 * UART.h
 *
 * Created: 22.10.2020 08:51:11
 *  Author: Sigurdod
 */ 


#ifndef UART_H_
#define UART_H_
#include <stdint.h>

#define FOSC (uint32_t)4950000
#define BAUD (uint32_t)9600
#define UBRR (uint32_t)((FOSC/(16*BAUD) - 1))


void UART_init(uint32_t ubrr);
void UART_puts(char *data, uint8_t data_size);
void UART_putc(char data);
char UART_recieve(void);


#endif /* UART_H_ */