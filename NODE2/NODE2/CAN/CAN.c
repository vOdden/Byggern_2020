/**
 * @file CAN.c
 * @author TTK4155 2020 Group 28
 * @date 17 nov 2020
 * @brief File containing top level drivers for MCU based CAN bus communication.
 * Recieved messages is put into a rolling, FIFO register of CAN messages.
 * Messages are also handled at recieve time, to update relevant global datastructures.
 * See msg_handler.h
 */


#include "CAN.h"
#include "sam.h"
#include "../Utility/printf-stdarg.h"
#include "can_controller.h"
#include "can_interrupt.h"
#include "../Utility/Utility.h"
/*
//MCK == 84MHz
//CAN BR:
//b24 == sampling mode 0 = once, 1 = three times.
//b16-22 == BRP Baud rate prescaler. t_can_system_clock == TQ == (BRP+1)/MCK. MCK = 8MHz BRP == 32. TQ = 1us = 83
//b12-13 == SJW. Re-synchronication jump width.
//b8-10 == PROPAG. progamming time segment. t = t_can_system_clock X(PROPAG+1)
//b4-6 == PHASE1. Phase 1 segment. tPH1 == t_can_system_clock X(PHASE1+1)
//b0-2 == PHASE2. Phase 2 segment. tPH2 == t_can_system_clock X(PHASE2+1)
*/

#define CAN_BRP ( 83 << 16) //CAN system clock = 242.4kHz => Tq = 4.125
#define CAN_SMP (0 << 24)	//Sample once per bit
#define CAN_PROPAG (1 << 8)	//Propagation delay = TQ x2
#define CAN_SJW (2 << 12)	//tPH1 =
#define CAN_PHASE1 (6 << 4)
#define CAN_PHASE2 (6 << 0)
#define can_br (CAN_BRP | CAN_SMP | CAN_PROPAG | CAN_SJW | CAN_PHASE1 | CAN_PHASE2)

volatile CAN_MESSAGE CAN_INPUT_BUFFER[8]; ///< @brief Rolling CAN input message buffer 
volatile uint8_t CAN_BUFFER_CTR = 0; ///< @brief CAN input message buffer counter
volatile uint8_t CAN_first_rx = 0; ///< @brief First valid CAN_input message flag
volatile uint8_t valid_messages = 0; ///< @brief Number of valid CAN messages in input buffer

static volatile int flag = 0;
/**
 * @brief Read data from input buffer
 *
 * @param  RX_packet: pointer to data destination
 * @retval int8_t: fail/success
 */ 
int8_t CAN_read(CAN_MESSAGE* RX_packet)
{
	
	if(valid_messages > 0)
	{
		valid_messages--;
		*RX_packet = CAN_INPUT_BUFFER[CAN_first_rx];
		
		if(++CAN_first_rx >= 8 )
		{
			CAN_first_rx = 0;
		}
		return 0;
	}
	else
	{
		return -1;
	}
}

int CAN_interrupt()
{
	if(flag)
	{
		flag = 0;
		return 1;
	}
	return 0;
}

/**
 * @brief Initialization function for the top layer CAN interface.
 * Configures relevant peripherals and goes through initialization-
 * sequence
 *
 * @param  mode: MCP2515 mode
 * @retval none
 */ 
void CAN_init(uint8_t mode)
{
	for(uint8_t i = 0; i < 8; i++) //Initialize all CAN input buffer IDs to zero
	{
		CAN_INPUT_BUFFER[i].ID = 0;
	}
	
	can_init_def_tx_rx_mb(can_br); 
	
	TC2_init(750000); //Timer for ready-pinging of NODE1
	Timer_start(TIMER2);
}

void CAN_send(CAN_MESSAGE* TX_packet)
{
	can_send(TX_packet, 0);
}
