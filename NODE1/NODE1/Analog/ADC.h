#ifndef ADC_H_
#define ADC_H_

#define ADC_CHANNEL_0 0
#define ADC_CHANNEL_1 1
#define ADC_CHANNEL_2 2
#define ADC_CHANNEL_3 3

#define ADC_JOYSTICK_X 	ADC_CHANNEL_2
#define ADC_JOYSTICK_Y	ADC_CHANNEL_3
#define ADC_SLIDER_L 	ADC_CHANNEL_1
#define ADC_SLIDER_R 	ADC_CHANNEL_0

#include <stdint.h>

void 	ADC_init(void);
uint8_t ADC_read(uint8_t channel);
void 	ADC_get_conversion(uint8_t* data);

#endif /* ADC_H_ */