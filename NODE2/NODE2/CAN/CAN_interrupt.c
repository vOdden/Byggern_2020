/*
 * interrupt.c
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 

#include "can_interrupt.h"

#include <stdio.h>
#include "sam.h"
#include "msg_handler.h"
#include "../Utility/printf-stdarg.h"

#include "can_controller.h"
#include "CAN.h"

#define DEBUG_INTERRUPT 1

/**
 * \brief CAN0 Interrupt handler for RX, TX and bus error interrupts
 *
 * \param void
 *
 * \retval 
 */

extern CAN_MESSAGE CAN_INPUT_BUFFER[8];
extern uint8_t CAN_BUFFER_CTR;
extern volatile uint8_t valid_messages;

void CAN0_Handler( void )
{
	//if(DEBUG_INTERRUPT)printf("CAN0 interrupt\n\r");
	char can_sr = CAN0->CAN_SR; 
	
	//RX interrupt
	if(can_sr & (CAN_SR_MB1 | CAN_SR_MB2) )//Only mailbox 1 and 2 specified for receiving
	{
	//	CAN_MESSAGE message;
		if(can_sr & CAN_SR_MB1)  //Mailbox 1 event
		{
//			can_receive(&message, 1);
			can_receive(&CAN_INPUT_BUFFER[CAN_BUFFER_CTR], 1);
			msg_handler(&CAN_INPUT_BUFFER[CAN_BUFFER_CTR]);
//			printf("CAN0 message arrived in mailbox 1\n\r");
			
			valid_messages++;
			if(++CAN_BUFFER_CTR >= 8)
			{
				CAN_BUFFER_CTR = 0;
			}
			
		}
		else if(can_sr & CAN_SR_MB2) //Mailbox 2 event	
		{
//			can_receive(&message, 2);
			can_receive(&CAN_INPUT_BUFFER[CAN_BUFFER_CTR], 2);
			msg_handler(&CAN_INPUT_BUFFER[CAN_BUFFER_CTR]);
//			printf("CAN0 message arrived in mailbox 2\n\r");
			valid_messages++;
			if(++CAN_BUFFER_CTR >= 8)
			{
				CAN_BUFFER_CTR = 0;
			}
		}
		else
		{
	//		printf("CAN0 message arrived in non-used mailbox\n\r");
		}

//		if(DEBUG_INTERRUPT)printf("message id: %d\n\r", message.ID);
//		if(DEBUG_INTERRUPT)printf("message data length: %d\n\r", message.data_length);
		//for (int i = 0; i < message.data_length; i++)
		{
	//		if(DEBUG_INTERRUPT)printf("%d ", message.data[i]);
		}
		//if(DEBUG_INTERRUPT)printf("\n\r");
	}
	
	if(can_sr & CAN_SR_MB0)
	{
//		if(DEBUG_INTERRUPT) printf("CAN0 MB0 ready to send \n\r");
		
		//Disable interrupt
		CAN0->CAN_IDR = CAN_IER_MB0;

	}
	
	if(can_sr & CAN_SR_ERRP)
	{
//		if(DEBUG_INTERRUPT)printf("CAN0 ERRP error\n\r");
	}
	if(can_sr & CAN_SR_TOVF)
	{
	//	if(DEBUG_INTERRUPT)printf("CAN0 timer overflow\n\r");
	}
	
	NVIC_ClearPendingIRQ(ID_CAN0);
}




