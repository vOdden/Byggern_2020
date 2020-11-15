


#ifndef MSG_HANDLER_H_
#define MSG_HANDLER_H_

#include "CAN.h"
#include "../utility.h"
#include <stdint.h>

typedef struct {
	Direction_t dir;
	int8_t x;
	int8_t y;
	uint8_t button;
	uint8_t slider;
	Direction_t slider_dir;
}pos_t;

typedef enum
{
	START,
	READY,
	STOP,
	GOAL	
}MSG_type;

void Send_msg(MSG_type);
void msg_handler(CAN_MESSAGE* can_msg);


#endif /* MSG_HANDLER_H_ */