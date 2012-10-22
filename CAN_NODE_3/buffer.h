#ifndef BUFFER_H_
#define BUFFER_H_

#include <avr/io.h>
#include "mcp2515.h"
#include "can.h"

/************************************************************************
 *	DEFINES
 */
#define CAN_TX_BUFFER_SIZE		8	
#define CAN_RX_BUFFER_SIZE		8	

/************************************************************************
 *	DATA TYPES
 */
typedef struct
{
	CanMessage	*buffer;
	uint8_t		size;
	uint8_t     head;
	uint8_t     tail;
	uint8_t		count;

} CanBuffer;

/************************************************************************
 *	ENUMERATIONS
 */
typedef enum 
{
	BUFFER_FULL = 0,
	BUFFER_MID,
	BUFFER_EMPTY
	
} BufferState;

/************************************************************************
 *	FUNCTIONS
 */
void		    CAN_BufInit ( volatile CanBuffer *buf, uint8_t size );
void    		CAN_BufEnq  ( volatile CanBuffer *buf, volatile CanMessage *msg  );
void	        CAN_BufDeq  ( volatile CanBuffer *buf, volatile CanMessage *msg );
BufferState     CAN_BufState( volatile CanBuffer *buf  );

#endif 