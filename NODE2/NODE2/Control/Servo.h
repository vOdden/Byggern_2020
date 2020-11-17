/**
 * @file Servo.h
 * @author TTK4155 2020 Group 28
 * @date 17 nov 2020
 * @brief File containing driver for Servo
 */
#ifndef Servo_H_
#define Servo_H_

#include <stdint.h>

#define SERVO_PWM_CHANNEL 6

void Servo_init(void);
void Servo_move(uint8_t pos, uint8_t channel);
uint16_t Servo_calc_D(int8_t x, int8_t y);
void Servo_set_position(int8_t x, int8_t y);
int8_t Servo_get_D(uint8_t channel);

#endif
