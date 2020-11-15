/*
 * CAN.c
 *
 * Created: 01.10.2020 09:28:27
 *  Author: Sigurdod
 */ 


#include "CAN.h"
#include "msg_handler.h"

CAN_MESSAGE CAN_INPUT_BUFFER[8];
uint8_t CAN_BUFFER_CTR = 0;
uint8_t CAN_first_rx = 0;
uint8_t valid_messages = 0;

static volatile int flag = 0;


// Ved interrupt fra CAN -> Sjekk om det er RX.complete.
//Hvis pakke er mottatt, les til CAN buffer.
ISR(INT0_vect)
{
	flag = 1;
	
	//Sjekk interrupt-flagget
	//volatile uint8_t interrupt_flag = mcp_2515_read(MCP_CANINTF);
	//Hvis RX -> ta i mot pakke i buffer
//	printf("ISR: ");
//	printf("flag: %d \n", mcp_2515_read(MCP_CANINTF));
	if( mcp_2515_read(MCP_CANINTF) & MCP_RX0IF)
	{
		CAN_receive(0, &(CAN_INPUT_BUFFER[CAN_BUFFER_CTR]));
		valid_messages++;
		mcp_2515_bit_modify(MCP_CANINTF, MCP_RX0IF, 0);
//		printf("Message received in CAN buffer 0 \n");
	//	printf("Message: %s \n",CAN_INPUT_BUFFER[CAN_BUFFER_CTR].data);
		if(++CAN_BUFFER_CTR >= 8)
		{
			CAN_BUFFER_CTR = 0;
		}
	}
	if( mcp_2515_read(MCP_CANINTF) & MCP_RX1IF)
	{
		CAN_receive(1, &(CAN_INPUT_BUFFER[CAN_BUFFER_CTR]));
		valid_messages++;
		mcp_2515_bit_modify(MCP_CANINTF , MCP_RX1IF, 0);
//		printf("Message received in CAN buffer 1 \n");
		if(++CAN_BUFFER_CTR >= 8)
		{
			CAN_BUFFER_CTR = 0;
		}
	}
	mcp_2515_write(MCP_CANINTF, 0);
}

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


int CAN_RXcmplt(void)
{
	if(flag == 0)
	{
		return 0;
	}
	uint8_t interrupt_flag = mcp_2515_read(MCP_CANINTF);
	interrupt_flag |= interrupt_flag & (MCP_RX0IF | MCP_RX1IF); //RX0 complete
//	vector[1] = interrupt_flag & MCP_RX1IF; //RX1 complete
	return interrupt_flag;
}


void CAN_init(uint8_t mode)
{
	for(uint8_t i = 0; i < 8; i++)
	{
		CAN_INPUT_BUFFER[i].ID = 0;
	}

	mcp_2515_init(MODE_CONFIG);

	mcp_2515_write(MCP_CANINTE, MCP_RX_INT);
	
	printf("CANINTE: %x \n", mcp_2515_read(MCP_CANINTE));
	
	// Sjekk hvilke bit dette er
	uint8_t _BRP = 7; //Baud rate prescaler. TQ = 2 (BRP+1)/Fosc. 7 gir BAUD = 1MHz @ 16MHz(?) TQ = 1us
	uint8_t _SJW = (0x80); // Synchronization jump width(?)
	uint8_t CNF_CONF = _SJW | _BRP;
	
	mcp_2515_write(MCP_CNF1, CNF_CONF);
	
	//length of propagation segment = TQ x PRSEG. 	
	//length of PHG1 == TQ x PHSEG1
	//BTLMODE == 1 => PH2 settes av PHSEG2. Sett denne
	
	uint8_t _PRSEG = (1 << 0);
	uint8_t _SAM = (0 << 6);
	uint8_t _BTLMODE = (1 << 7);
	uint8_t _PHSEG1 = (6 << 3);
	
	CNF_CONF = _PRSEG | _SAM | _BTLMODE | _PHSEG1;
	mcp_2515_write(MCP_CNF2, CNF_CONF);

	uint8_t RXBnCTRL_CONF = 3 << 5; //Turn mask/filter off
//	 RXBnCTRL_CONF |= 1 << 3; //remote transfer request(?)

	mcp_2515_write(MCP_RXB0CTRL,RXBnCTRL_CONF);
	mcp_2515_write(MCP_RXB1CTRL,RXBnCTRL_CONF);

	//length of PHG2 == TQ x PHSEG2
	//SOF?????????????
	uint8_t _PHSEG2 = (6 << 0);
	uint8_t _WAKFIL = (0 << 6);
	uint8_t _SOF = (0 << 7);
		
	CNF_CONF = _PHSEG2 | _WAKFIL | _SOF;
	mcp_2515_write(MCP_CNF3, CNF_CONF);
	
	mcp_2515_bit_modify(MCP_CANCTRL, ~(1<<4) ,1 << 4); //Set one shot mode
	
	mcp_2515_set_mode(mode);
	
	//disable global interrupt
	cli();
	//interrupt on falling edge. PD2
	set_bit(MCUCR, ISC01);
	clear_bit(MCUCR, ISC00);
	//Interrupt enable PD2.
	set_bit(GICR, INT0);
	
	sei();
	
}

void CAN_send(CAN_MESSAGE* TX_packet)
{
	static uint8_t buffer_number = 0;
	//Sjekk at buffer er ledig. Hvis ikke, velg en annen buffer	
	while(!CAN_TXcomplete(buffer_number))
	{
		buffer_number++;
		if(buffer_number > 2)
		{
			buffer_number = 0;
		}		
	}
	
	// Define data length:
	mcp_2515_write(MCP_TXB0DLC + (buffer_number << 4), TX_packet->data_length);
	
	//Write data to transmit buffer
	for (int i = 0; i < TX_packet->data_length; i++)
	{
		mcp_2515_write(MCP_TXB0Dm + i + (buffer_number << 4), TX_packet->data[i]);
	}
	
		//ID: 0xabcd == 16 bit.		xxxx x|xxx xxxx xxxx
	//CAN ID == 11bit, SIDH		
		
	//Canbuss ID er 11 bit.
	mcp_2515_write(MCP_TXB0SIDH + (buffer_number << 4), (TX_packet->ID >> 3));
	mcp_2515_write(MCP_TXB0SIDL + (buffer_number << 4), (TX_packet->ID << 5));
		
	mcp_2515_request_to_send(MCP_RTS_TX0 + buffer_number);
}


void CAN_receive(int RX_buffer, CAN_MESSAGE* RX_packet)
{
	uint16_t RX_IDh = mcp_2515_read(MCP_RXB0SIDH + (RX_buffer << 4)) << 3;
	uint16_t RX_IDL = mcp_2515_read(MCP_RXB0SIDL + (RX_buffer << 4));
	RX_IDL = RX_IDL >> 5; //Three highest bits
	RX_packet->ID = RX_IDh | RX_IDL; 
		
	uint8_t RX_data_length = mcp_2515_read(MCP_RXB0DLC + (RX_buffer << 4));
	RX_packet->data_length = RX_data_length & 0x0F;
	
	for(int i = 0; i < RX_packet->data_length; i++)
	{
		RX_packet->data[i] = mcp_2515_read(MCP_RXB0DM + i + (RX_buffer << 4));
	}
	
	msg_handler(RX_packet);
}



int CAN_TXcomplete(uint8_t buffer_number)
{
	uint8_t transmit_flag = mcp_2515_read(MCP_CANINTF);
	uint8_t interrupts = transmit_flag &( MCP_TX0IF + (buffer_number<< 1));
	if(interrupts == (MCP_TX0IF + (buffer_number << 1)))
	{
		return 0;
	}
	return 1;
}

uint8_t CAN_error()
{
	uint8_t error = mcp_2515_read(MCP_EFLG);
	mcp_2515_bit_modify(MCP_EFLG, error, 0);	
	if(error != 0)
	{
		printf("CAN-ERROR \n");
		if(error & (1 << 0))//
		{
			printf("EWARN \n");
		}		
		if(error & (1 << 1))
		{
			printf("RXWAR \n");
		}		
		if(error & (1 << 2))
		{
			printf("TXWAR \n");
		}		
		if(error & (1 << 3))
		{
			printf("RXEP \n");
		}		
		if(error & (1 << 4))
		{
			printf("TXEP \n");
		}		
		if(error & (1 << 5))//
		{
			printf("TXBO \n");
		}		
		if(error & (1 << 6))
		{
			printf("RX0OVR \n");
		}		
		if(error & (1 << 7))
		{
			printf("RX0OVR \n");
		}
	}
	if(mcp_2515_read(MCP_CANINTF))
	{
			printf("interrupt flag: %d \n", mcp_2515_read(MCP_CANINTF));
	}

	
	return error;
}

CAN_MESSAGE CAN_handler(void)
{
	uint8_t RX_cmplt = CAN_RXcmplt();
	CAN_MESSAGE RX_packet;
	
	if(RX_cmplt & MCP_RX0IF)
	{
		CAN_receive(0, &RX_packet);
		mcp_2515_bit_modify(MCP_CANINTF, 0, 0);	
	}
	else if(RX_cmplt & MCP_RX1IF)
	{
		CAN_receive(1, &RX_packet);
		mcp_2515_bit_modify(MCP_CANINTF , 1, 0);
	}
	return RX_packet;
}