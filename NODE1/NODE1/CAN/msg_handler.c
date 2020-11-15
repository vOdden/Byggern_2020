
#include <stdio.h>
#include "msg_handler.h"
#include "CAN.h"
#include "../Menu/Menu.h"

volatile uint8_t ready		= 0;
volatile uint8_t start		= 0;
volatile uint8_t Goal		= 0;
volatile uint8_t Heartbeat	= 0;
volatile pos_t position;

extern int8_t difficulty_grade;

void msg_handler(CAN_MESSAGE* can_msg)
{	
	switch(can_msg->ID)
	{
		case 'P':
			position.button		= can_msg->data[0];  //uint8_t data[]
			position.dir		= can_msg->data[1];
			position.x			= can_msg->data[2];
			position.y			= can_msg->data[3];
			position.slider		= can_msg->data[4];
			//position.slider_dir	= can_msg->data[5];
			//printf("button: %x", position.button);
			//printf("direction: %x", can_msg->data[1]);
			//printf("slider: %x \r", can_msg->data[4]);
		break;
		
		case 'S':
			ready = can_msg->data[0];
			start = can_msg->data[1];
		break;
	
		case 'M':
			Goal = can_msg->data[0];
		break;
		
		case 'H':
			Heartbeat = can_msg->data[0];
		break;
#ifdef EX		
		case '5':
			printf("EX5 recieved \n");
			printf("Data: %s \n", can_msg->data);
		break;
		
		case '6':
			printf("EX6 recieved \n");
			int8_t i = can_msg->data[0];
			if(i > 127)
			{
				i *= -1;
			}
			printf("Joystick data: %d \n", i);
		break;
		
		case '7':
			printf("EX7 recieved \n");
			int8_t n = can_msg->data[0];
			if(n > 127)
			{
				n *= -1;
			}
 			Servo_set_position(n , 0);
		break;
#endif		
	}
}

void Send_msg(MSG_type type)
{
	
	CAN_MESSAGE msg;
	
	switch(type)
	{
		case START:
			msg.ID = 'S';
			msg.data_length = 2;
			msg.data[0] = 0xFF; // ready
			if(difficulty_grade)
			{
				msg.data[1] = 0xFF; //START = 0xnF     HARD = 0xFn    NORMAL = 0x0n
			}
			else
			{	
				msg.data[1] = 0x0F; //START = 0xnF     HARD = 0xFn    NORMAL = 0x0n
			}
		break;

		case READY:
			msg.ID = 'S';
			msg.data_length = 2;
			msg.data[0] = 0xFF;
			msg.data[1] = 0x00;
		break;
		
		case STOP:	
			msg.ID = 'S';
			msg. data_length = 2;
			msg.data[0] = 0xFF;
			msg.data[1] = 0x00;
		break;
		
		case GOAL:
			msg.ID = 'M';
			msg. data_length = 1;
			msg.data[0] = 1;
		break;
		
		
		default:
			return;
		
	}
	
	CAN_send(&msg);
}
