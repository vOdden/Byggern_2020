


#ifndef MSG_HANDLER_H_
#define MSG_HANDLER_H_

#include "can_controller.h"
#include "../Utility/Datatypes.h"
#include <stdint.h>

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