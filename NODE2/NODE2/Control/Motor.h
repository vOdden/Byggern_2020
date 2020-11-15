#ifndef Motor_H_
#define Motor_H_

#include <stdint.h>
#include "../Utility/Utility.h"

#define ENABLE 1
#define DISABLE 0

#define MOTOR_DIR_PIN		PIN10
#define MOTOR_DIR_PORT		PIOD
#define MOTOR_EN_PIN		PIN9
#define MOTOR_EN_PORT		PIOD
#define ENCODER_SEL_PIN		PIN2
#define ENCODER_SEL_PORT	PIOD
#define ENCODER_OE_PIN		PIN0
#define ENCODER_OE_PORT		PIOD
#define ENCODER_RST_PIN		PIN1
#define ENCODER_RST_PORT	PIOD
#define ENCODER_OUTPUT_PORT PIOC
#define ENCODER_OUTPUT_MASK 0x1FE

float saturate(float f, float min, float max);
void Motor_enable(uint8_t enable);
void Encoder_output_enable(uint8_t enable);
void Encoder_reset(void);
void Motor_init(void);
void Motor_set_direction(Direction_t dir);
void Motor_set_speed(uint16_t speed);
void Motor_velocity(float vel);
int16_t Encoder_read(void);
int16_t Encoder_limit(void);

#endif