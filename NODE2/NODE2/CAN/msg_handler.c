/**
 * @file msg_handler.c
 * @author TTK4155 2020 Group 28
 * @date 17 nov 2020
 * @brief File containing CAN bus message handling.
 */

#include "sam.h"
#include <stdio.h>
#include "CAN.h"
#include "msg_handler.h"
#include "../Utility/Utility.h"

volatile uint8_t ready; ///< @brief Node 1 ready flag
volatile uint8_t start; ///< @brief Game start message flag
volatile uint8_t Goal;  ///< @brief Goal scored flag
volatile uint8_t Heartbeat; ///< @brief Node heartbeat @note Not implemented. Replaced by ready polling
volatile pos_t position; ///< @brief global position structure. Continously updated with user input. Shared between nodes.

/**
 * @brief CAN buss message handler.
 * Uppdates global flags and data structures based on CAN.ID
 *
 * @param  can_msg: CAN message to process
 * @retval none
 */ 
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
				//printf("start: %d", start);
		break;
	
		case 'M':
			Goal = can_msg->data[0];
		break;
		
		case 'H':
			Heartbeat = can_msg->data[0];
		break;
		
		
		case '5':
			printf("EX5 recieved \r");
			printf("Data: %s \r", can_msg->data);
		break;
		
		case '6':
			printf("EX6 recieved \r");
			int8_t i = can_msg->data[0];
			if(i > 127)
			{
				i *= -1;
			}
			printf("Joystick data: %d \r", i);
		break;
		
// 		case '7':
// 			printf("EX7 recieved \r");
// 			int8_t y = can_msg->data[0];
// 			if(y > 127)
// 			{
// 				y *= -1;
// 			}
// 			Servo_set_position(y , 0);	
// 		break;
		
		
	}
}

/**
 * @brief CAN buss standard output package constructor.
 * Assembles and sends standard CAN packages.
 *
 * @param  type: Type of message to send
 * @retval none
 */ 
void Send_msg(MSG_type type)
{
	
	CAN_MESSAGE msg;
	
	switch(type)
	{
		case START:
		msg.ID = 'S';
		msg.data_length = 2;
		msg.data[0] = 0xFF; //READY
		msg.data[1] = 0xFF; //START
		printf("Sendt start \r");
		break;

		case READY:
		msg.ID = 'S';
		msg.data_length = 2;
		msg.data[0] = 0xFF;
		msg.data[1] = 0x00;
		//printf("Sendt ready \r");
		break;
		
		case STOP:
		msg.ID = 'S';
		msg. data_length = 2;
		msg.data[0] = 0xFF;
		msg.data[1] = 0x00;
		printf("Sendt stop \r");
		break;
		
		case GOAL:
		msg.ID = 'M';
		msg. data_length = 1;
		msg.data[0] = 1;
		printf("Sendt goal \r");
		break;
		
		
		default:
		return;
		
	}
	
	CAN_send(&msg);
}


void TC6_Handler(void) //Timer interrupt handler
{	
	uint32_t status = REG_TC2_SR0;//read status register - this clears interrupt flags
	
	Send_msg(READY);
}
