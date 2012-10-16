#include <avr/io.h>
#include "spi.h"
#include "mcp2515.h"
#include "can.h"
#include "uart.h"
#include "timer.h"

/************************************************************************
 *	INITIALIZE CAN
 */
enum MCP2515_STATUS CAN_Init ( uint8_t can_rate )
{	
	MCP2515_DESELECT();
	if( mcp2515_Init( can_rate ) )
		return FAILED;
	
	return OK;
}

/************************************************************************
 *	SEND MESSAGE
 */

enum MCP2515_STATUS CAN_SendMsg( const CanMessage *msg )
{
	uint8_t addr[2];
	
	if( mcp2515_ChkFreeTxBuf(addr) == OK ){
		
		mcp2515_WriteTxBuf( msg, addr[0] );
		mcp2515_RTS(addr[1]);
		return OK;
	}
	return FAILED;
}

/************************************************************************
 *	RECEIVE MESSAGE
 */
enum MCP2515_STATUS CAN_ReadMsg ( volatile CanMessage *msg )
{
	uint8_t stat, res;
	
	stat = mcp2515_ReadStatus();
	
	if ( stat & RX0IF_STATUS ) {					/* Message in Buffer 0 */				
		
		mcp2515_ReadRxBuf( msg, READ_RXB0_SIDH );	/* Read Receive Buffer	*/
		mcp2515_BitModify( CANINTF, RX0IF, 0);		/* Clear Interrupt Flag */
		res = OK;
	}
	else if ( stat & RX1IF_STATUS ) {				/* Message in Buffer 1 */
	
		mcp2515_ReadRxBuf( msg, READ_RXB1_SIDH );	/* Read Receive Buffer	*/
		mcp2515_BitModify( CANINTF, RX1IF, 0);		/* Clear Interrupt Flag */
		res = OK;
	}
	else {
		res = FAILED;
	}	
	
	return res;
}