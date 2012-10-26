#include <avr/io.h>
#include <util/atomic.h>
#include "can.h"
#include "mcp2515reg.h"
#include "mcp2515.h"
#include "spi.h"
#include "init.h"
#include "message.h"

/************************************************************************
 *	INITIALIZE CAN
 */
 CanStatus CAN_Init ( const uint8_t can_rate )
{	
	MCP2515_DESELECT();
	if( mcp2515_Init( can_rate ) )
		return CAN_FAILED;
	
	return CAN_OK;
}

/************************************************************************
 *	SEND MESSAGE
 */

CanStatus CAN_SendMsg( const CanMessage *msg )
{
	uint8_t addr[2];								/* [0] : load addr , [1] = RTS addr */
	
	if( mcp2515_ChkFreeTxBuf(addr) == MCP2515_OK ){
													/* Flash LED */
		mcp2515_BitModify( BFPCTRL, RXB0_DIGINPSTATE, RXB0_DIGINPSTATE );
		
		mcp2515_WriteTxBuf( msg, addr[0] );	
		mcp2515_RTS(addr[1]);						/* Transmit Message */
		
		mcp2515_BitModify( BFPCTRL, RXB0_DIGINPSTATE, ~RXB0_DIGINPSTATE );
		return CAN_OK;
	}
	else
		return CAN_FAILED;
}

/************************************************************************
 *	RECEIVE MESSAGE
 */
CanStatus CAN_ReadMsg ( volatile CanMessage *msg )
{
	uint8_t stat;
	CanStatus res;
	
	stat = mcp2515_ReadStatus();
	
	if ( stat & RX0IF_STATUS ) {					/* Message in Buffer 0 */				
		
		mcp2515_ReadRxBuf( msg, READ_RXB0_SIDH );	/* Read Receive Buffer	*/
		mcp2515_BitModify( CANINTF, RX0IF, 0);		/* Clear Interrupt Flag */
		res = CAN_OK;
	}
	else if ( stat & RX1IF_STATUS ) {				/* Message in Buffer 1 */
	
		mcp2515_ReadRxBuf( msg, READ_RXB1_SIDH );	/* Read Receive Buffer	*/
		mcp2515_BitModify( CANINTF, RX1IF, 0);		/* Clear Interrupt Flag */
		res = CAN_OK;
	}
	else {
		res = CAN_FAILED;
	}	
	
	return res;
}
