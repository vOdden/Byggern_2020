
#ifndef CAN_DRIVER_H_
#define CAN_DRIVER_H_

#include <stdint.h>
#include "SPI.h"
#include "MCP2515.h"

uint8_t mcp_2515_init(uint8_t mode);

void mcp_2515_set_mode(uint8_t mode);
void mcp_2515_reset(void);

uint8_t mcp_2515_read(uint8_t address);
void mcp_2515_write(uint8_t address, uint8_t data);

void mcp_2515_request_to_send(uint8_t buffer);
void mcp_2515_bit_modify(uint8_t adress, uint8_t mask, uint8_t data);
uint8_t mcp_2515_read_status(void);

// Page 63 in the datasheet

#endif /* CAN_DRIVER_H_ */