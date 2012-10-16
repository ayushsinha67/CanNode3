#ifndef _CAN_H
#define _CAN_H

#include <avr/io.h>

/************************************************************************
 *	DEFINES AND CONSTANTS
 */


/************************************************************************
 *	DATA TYPES
 */
typedef struct {
	uint32_t id; 
	uint8_t  ext;
	uint8_t  rtr; 
	uint8_t  dlc;
	uint8_t  data[8];
} CanMessage;

/************************************************************************
 *	FUNCTION PROTOTYPES
 */
enum MCP2515_STATUS CAN_Init			( uint8_t can_rate );
enum MCP2515_STATUS	CAN_SendMsg			( const	   CanMessage *msg );
enum MCP2515_STATUS CAN_ReadMsg			( volatile CanMessage *msg );

#endif