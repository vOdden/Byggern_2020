/**
 * @file IR.c
 * @author TTK4155 2020 Group 28
 * @date 17 nov 2020
 * @brief File containing drivers for IR
 *
 */
#include "IR.h"
#include "ADC/ADC.h"

static uint16_t obstruct_threshold = 10;

void IR_obstructThreshold(uint16_t t)
{
    obstruct_threshold = t;
}

uint8_t IR_obstructed(void)
{
    #define FILTER_ORDER 4
    #if FILTER_ORDER == 1
        return ADC_read(ADC_CH_IR) < obstruct_threshold;
    #else
        static uint16_t filter[FILTER_ORDER-1];

        uint16_t val = ADC_read(ADC_CH_IR);
        uint16_t avg = val;

        for(uint8_t i = 0; i < FILTER_ORDER-2; i++){
            avg += filter[i];
            filter[i] = filter[i+1];
        }
        avg += filter[FILTER_ORDER-2];
        filter[FILTER_ORDER-2] = val;

        return avg/FILTER_ORDER < obstruct_threshold;
    #endif
}



