
#ifndef SRAM_H_
#define SRAM_H_


#include <avr/io.h>
#include <avr/interrupt.h>


void SRAM_init(void);
uint8_t SRAM_write(uint8_t *data, uint16_t adress, uint8_t size);
uint8_t SRAM_read(uint8_t *data, uint16_t adress, uint8_t size);
void SRAM_test(void);


#endif /*SRAM_H_*/