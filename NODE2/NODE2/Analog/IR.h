/**
 * @file IR.h
 * @author TTK4155 2020 Group 28
 * @date 17 nov 2020
 * @brief File containing drivers for IR
 *
 */

#ifndef IR_H_
#define IR_H_

#include <stdint.h>


void IR_obstructThreshold(uint16_t t);
uint8_t IR_obstructed(void);

#endif
