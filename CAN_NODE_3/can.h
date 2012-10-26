#ifndef CAN_H_
#define CAN_H_

#include <avr/io.h>
#include "messagedef.h"

/************************************************************************
 *	DEFINES
 */
/* CAN Initializations */
#define CAN_SPEED			CAN_1MBPS				/* Bus Speed */
#define CAN_MODE			CAN_2A					/* CAN Mode */
#define CAN_2A				0						/* Standard Identifier	- 11 bit */
#define CAN_2B				1						/* Extended Identifier  - 29 bit */
#define CAN_FILTER_ENABLE	1						/* Enable: 1, Disable: 0 */

/* Filter MASKS */
#define MASK0				0x7FF
#define MASK1				0x7FF

/* Filters */
#define FILTER0				PNEUM_SHIFT_ID
#define FILTER1				PNEUM_ONLINE_ID
#define FILTER2				0x7FF
#define FILTER3				0x7FF
#define FILTER4				0x7FF
#define FILTER5				0x7FF

/************************************************************************
 *	DATA TYPES
 */
typedef struct 
{
	uint32_t id; 
	uint8_t  ext;									/* 0 = SID, 1 = EXIDE */
	uint8_t  rtr;									/* 0 = NO RTR, 1 = RTR */
	uint8_t  dlc;
	uint8_t  data[8];	
	
} CanMessage;

/************************************************************************
 *	ENUMERATIONS
 */
typedef enum
{
	CAN_OK = 0,
	CAN_FAILED

} CanStatus;

/************************************************************************
 *	FUNCTION PROTOTYPES
 */
CanStatus CAN_Init			( const uint8_t can_rate );
CanStatus CAN_SendMsg		( const CanMessage *msg );
CanStatus CAN_ReadMsg		( volatile CanMessage *msg );

#endif