
#ifndef CAN_H_
#define CAN_H_

#include "CAN_driver.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../bit_macros.h"


typedef struct
{
	uint16_t ID;
	uint8_t data_length;
	uint8_t data[8];
} CAN_MESSAGE;

#include "msg_handler.h"

int CAN_RXcmplt(void);
void CAN_init(uint8_t mode);
void CAN_send(CAN_MESSAGE* TX_packet);
void CAN_receive(int RX_buffer, CAN_MESSAGE* RX_packet);
int CAN_TXcomplete(uint8_t buffer_number);
uint8_t CAN_error(void);
CAN_MESSAGE CAN_handler(void);
int8_t CAN_read(CAN_MESSAGE* RX_packet);


#endif /* CAN_H_ */