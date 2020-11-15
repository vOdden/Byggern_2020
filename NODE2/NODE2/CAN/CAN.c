
#include "CAN.h"
#include "sam.h"
#include "../Utility/printf-stdarg.h"
#include "can_controller.h"
#include "can_interrupt.h"
#include "../Utility/Utility.h"
/*
//MCK ==16MHz eller 84???
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

volatile CAN_MESSAGE CAN_INPUT_BUFFER[8];
volatile uint8_t CAN_BUFFER_CTR = 0;
volatile uint8_t CAN_first_rx = 0;
volatile uint8_t valid_messages = 0;

static volatile int flag = 0;

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

void CAN_init(uint8_t mode)
{
	for(uint8_t i = 0; i < 8; i++)
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
