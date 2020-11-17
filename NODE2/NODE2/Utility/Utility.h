/**
 * @file Utility.h
 * @author TTK4155 2020 Group 28
 * @date 17 nov 2020
 * @brief File containing drivers for utility functions
 *
 */

#ifndef UTILITY_H_
#define UTILITY_H_


#include "sam.h"
#include "Datatypes.h"
#include "Delay.h"
#include "printf-stdarg.h"
#include "PWM.h"
#include "Timer.h"
#include "IO_map.h"
#include "UART.h"


#define set_pin(port, pin) (port->PIO_SODR |= pin)
#define clear_pin(port, pin) (port->PIO_CODR |= pin)
#define read_pin(port, pin) (port->PIO_PDSR & pin)
#define set_bit(reg, bit) (reg |= 1 << bit)
#define clear_bit(reg, bit) (reg &= ~(1 << bit))

#endif /* UTILITY_H_ */
