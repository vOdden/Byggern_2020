#ifndef ADC_H_
#define ADC_H_

#include "stdint.h"

#define ADC_CH0  1 << 0x00
#define ADC_CH1  1 << 0x01
#define ADC_CH2  1 << 0x02
#define ADC_CH3  1 << 0x03
#define ADC_CH4  1 << 0x04
#define ADC_CH5  1 << 0x05
#define ADC_CH6  1 << 0x06
#define ADC_CH7  1 << 0x07
#define ADC_CH8  1 << 0x08
#define ADC_CH9  1 << 0x09
#define ADC_CH10 1 << 0x0A
#define ADC_CH11 1 << 0x0B
#define ADC_CH12 1 << 0x0C
#define ADC_CH13 1 << 0x0D
#define ADC_CH14 1 << 0x0E
#define ADC_CH15 1 << 0x0F

#define ADC_CH_IR ADC_CH0

void ADC_init(uint8_t channel);
uint16_t ADC_read(uint8_t channel);


#endif /* ADC_DRIVER_H_ */