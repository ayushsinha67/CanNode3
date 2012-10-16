#include <avr/io.h>
#include <avr/pgmspace.h>
#include "terminal.h"
#include "uart.h"
#include "mcp2515.h"
#include "can.h"

/************************************************************************
 *	DISPLAY COMMAND REGISTERS
 */
void term_Commands	(void)
{
	UART_TxStr_p( PSTR("\nCOMMANDS \nc : \tControl Registers\n") );
	UART_TxStr_p( PSTR("x : \tRX Status\n") );
	UART_TxStr_p( PSTR("s : \tRead Status\n") );
	UART_TxStr_p( PSTR("i : \tInterrupt Flags\n") );
	UART_TxStr_p( PSTR("e : \tError Flags\n") );
	UART_TxStr_p( PSTR("t : \tTransmit Buffer\n") );
	UART_TxStr_p( PSTR("r : \tReceive Buffer\n") );
	UART_TxStr_p( PSTR("q : \tMessage Stream\n") );
	UART_TxStr_p( PSTR("l : \tLoopback\n") );
	UART_TxStr_p( PSTR("h : \tHelp\n") );
}

/************************************************************************
 *	DISPLAY CONTROL REGISTERS 
 */
void term_CtrlReg(void)
{
	uint8_t stat = mcp2515_Read( CANSTAT);
	
	UART_TxStr_p( PSTR("\nCONTROL REGISTERS -----------------------------------\n\n") );	
	UART_TxStr_p( PSTR("MODE:\t\t") );		UART_TxHex( stat >> 5 );	
	
	switch( stat >> 5 ){
		case 0: UART_TxStr_p( PSTR(" - NORMAL") );		break;
		case 1: UART_TxStr_p( PSTR(" - SLEEP") );		break;
		case 2: UART_TxStr_p( PSTR(" - LOOPBACK") );	break;
		case 3: UART_TxStr_p( PSTR(" - LISTEN-ONLY") ); break;
		case 4: UART_TxStr_p( PSTR(" - CONFIG") );		break;
	   default: UART_TxStr_p( PSTR(" - NORMAL") );		break;
	};
	UART_TxChar('\n');
	
	UART_TxStr_p( PSTR("ICOD:\t\t") );		UART_TxHex( ( mcp2515_Read( CANSTAT ) >> 1 ) & 0x07 );	UART_TxChar('\n');
	UART_TxStr_p( PSTR("CANCTRL:\t") );		UART_TxHex( mcp2515_Read( CANCTRL) );					UART_TxChar('\n');
	UART_TxStr_p( PSTR("CANSTAT:\t") );		UART_TxHex( mcp2515_Read( CANSTAT) );					UART_TxChar('\n');
	UART_TxStr_p( PSTR("CNF1:\t\t") );		UART_TxHex( mcp2515_Read( CNF1) );						UART_TxChar('\n');
	UART_TxStr_p( PSTR("CNF2:\t\t") );		UART_TxHex( mcp2515_Read( CNF2) );						UART_TxChar('\n');
	UART_TxStr_p( PSTR("CNF3:\t\t") );		UART_TxHex( mcp2515_Read( CNF3) );						UART_TxChar('\n');
	UART_TxStr_p( PSTR("BFPCTRL:\t") );		UART_TxHex( mcp2515_Read( BFPCTRL) );					UART_TxChar('\n');
	UART_TxStr_p( PSTR("TXRTSCTRL:\t") );	UART_TxHex( mcp2515_Read( TXRTSCTRL ) );				UART_TxChar('\n');
	UART_TxStr_p( PSTR("CANINTE:\t") );		UART_TxHex( mcp2515_Read( CANINTE ) );					UART_TxChar('\n');
	UART_TxStr_p( PSTR("CANINTF:\t") );		UART_TxHex( mcp2515_Read( CANINTF ) );					UART_TxChar('\n');
	UART_TxStr_p( PSTR("EFLG:\t\t") );		UART_TxHex( mcp2515_Read( EFLG ) );						UART_TxChar('\n');
	UART_TxStr_p( PSTR("TXB0CTRL:\t") );	UART_TxHex( mcp2515_Read( TXB0CTRL ) );					UART_TxChar('\n');
	UART_TxStr_p( PSTR("TXB1CTRL:\t") );	UART_TxHex( mcp2515_Read( TXB1CTRL ) );					UART_TxChar('\n');
	UART_TxStr_p( PSTR("TXB2CTRL:\t") );	UART_TxHex( mcp2515_Read( TXB2CTRL ) );					UART_TxChar('\n');
	UART_TxStr_p( PSTR("RXB0CTRL:\t") );	UART_TxHex( mcp2515_Read( RXB0CTRL ) );					UART_TxChar('\n');
	UART_TxStr_p( PSTR("RXB1CTRL:\t") );	UART_TxHex( mcp2515_Read( RXB1CTRL ) );					UART_TxChar('\n');
	UART_TxChar('\n');
}

/************************************************************************
 *	DISPLAY READ STATUS
 */

void term_ReadStatus(void)
{
	uint8_t data = mcp2515_ReadStatus();
	
	UART_TxStr_p ( PSTR("\nREAD STATUS -----------------------------------\n\n") );
	
	UART_TxStr_p( PSTR("Register:\t") ); UART_TxHex( data );	UART_TxChar('\n');
	UART_TxStr_p( PSTR("Status:\n") );
	
	if( (data & 0x01) == 0x01 ){ UART_TxStr_p( PSTR("\t\tCANINTF.RX0IF\n") ); }
	if( (data & 0x02) == 0x02 ){ UART_TxStr_p( PSTR("\t\tCANINTF.RX1IF\n") ); }
	if( (data & 0x04) == 0x04 ){ UART_TxStr_p( PSTR("\t\tTXB0CNTRL.TXREQ\n") ); }
	if( (data & 0x08) == 0x08 ){ UART_TxStr_p( PSTR("\t\tCANINTF.TX0IF\n") ); }
	if( (data & 0x10) == 0x10 ){ UART_TxStr_p( PSTR("\t\tTXB1CNTRL.TXREQ\n") ); }	
	if( (data & 0x20) == 0x20 ){ UART_TxStr_p( PSTR("\t\tCANINTF.TX1IF\n") ); }	
	if( (data & 0x40) == 0x40 ){ UART_TxStr_p( PSTR("\t\tTXB2CNTRL.TXREQ\t\n") ); }	
	if( (data & 0x80) == 0x80 ){ UART_TxStr_p( PSTR("\t\tCANINTF.TX2IF\n") ); }				
}

/************************************************************************
 *	DISPLAY RECEIVE STATUS
 */
void term_RxStatus(void)
{
	uint8_t data = mcp2515_RXStatus();
	
	UART_TxStr_p ( PSTR("\nRECEIVE STATUS -----------------------------------\n\n") );
	
	UART_TxStr_p( PSTR("Register:\t")); UART_TxHex( data );	UART_TxChar('\n');
	
	UART_TxStr_p( PSTR("RX Message:\t") );
	
	switch( ( data & 0xC0 ) >> 6 ){						/* Check Message Received */	
		case 0: UART_TxStr_p( PSTR("NO MSG\n") );		break;
		case 1: UART_TxStr_p( PSTR("RXB0\n") );			break;	
		case 2: UART_TxStr_p( PSTR("RXB1\n") );			break;	
		case 3: UART_TxStr_p( PSTR("BOTH BUFFERS\n") );	break;
	}
	
	UART_TxStr_p( PSTR("Msg Type:\t") );
	
	switch( ( data & 0x18 ) >> 3 ){						/* TYPE of  Message Received */	
		case 0: UART_TxStr_p( PSTR("Standard, DATA\n") );		break;
		case 1: UART_TxStr_p( PSTR("Standard, REMOTE") );		break;	
		case 2: UART_TxStr_p( PSTR("Extended, DATA\n") );		break;	
		case 3: UART_TxStr_p( PSTR("Extended, REMOTE\n") );		break;
	}
	
	UART_TxStr_p( PSTR("Filter Match:\t") );
	switch( data & 0x07 ){								/* Filter Match */	
		case 0: UART_TxStr_p( PSTR("RXF0\n") );							break;
		case 1: UART_TxStr_p( PSTR("RXF1\n") );							break;	
		case 2: UART_TxStr_p( PSTR("RXF2\n") );							break;	
		case 3: UART_TxStr_p( PSTR("RXF3\n") );							break;
		case 4: UART_TxStr_p( PSTR("RXF4\n") );							break;
		case 5: UART_TxStr_p( PSTR("RXF5\n") );							break;
		case 6: UART_TxStr_p( PSTR("RXF0 Rollover to RXB1\n") );		break;
		case 7: UART_TxStr_p( PSTR("RXF1 Rollover to RXB1\n") );		break;
	}
}

/************************************************************************
 *	DISPLAY INTERRUPT FLAGS
 */

void term_IntFlag(void)
{
	uint8_t data = mcp2515_Read( CANINTF );
	
	UART_TxStr_p ( PSTR("\nINTERRUPT FLAGS -----------------------------------\n\n") );
	
	UART_TxStr_p( PSTR("CANINTF:\t") ); UART_TxHex( data );	UART_TxChar('\n');
	UART_TxStr_p( PSTR("Status:\n") );
	
	if( (data & 0x01) == 0x01 ){ UART_TxStr_p( PSTR("\t\tReceive Buffer 0 Full\n") ); }
	if( (data & 0x02) == 0x02 ){ UART_TxStr_p( PSTR("\t\tReceive Buffer 1 Full\n") ); }
	if( (data & 0x04) == 0x04 ){ UART_TxStr_p( PSTR("\t\tTransmit Buffer 0 Empty\n") ); }
	if( (data & 0x08) == 0x08 ){ UART_TxStr_p( PSTR("\t\tTransmit Buffer 1 Empty\n") ); }
	if( (data & 0x10) == 0x10 ){ UART_TxStr_p( PSTR("\t\tTransmit Buffer 2 Empty\n") ); }	
	if( (data & 0x20) == 0x20 ){ UART_TxStr_p( PSTR("\t\tError\n") ); }	
	if( (data & 0x40) == 0x40 ){ UART_TxStr_p( PSTR("\t\tWakeup\t\n") ); }	
	if( (data & 0x80) == 0x80 ){ UART_TxStr_p( PSTR("\t\tMessage Error\n") ); }
}

/************************************************************************
 *	DISPLAY ERROR FLAGS
 */
void term_ErrorFlag(void)
{
	uint8_t data = mcp2515_Read( EFLG );
	
	UART_TxStr_p( PSTR("\nERROR FLAGS -----------------------------------\n\n") );
	
	UART_TxStr_p( PSTR("EFLG:\t\t") ); UART_TxHex( data );				UART_TxChar('\n');
	UART_TxStr_p( PSTR("TEC:\t\t") );  UART_TxHex( mcp2515_Read(TEC) );	UART_TxChar('\n');
	UART_TxStr_p( PSTR("REC:\t\t") );  UART_TxHex( mcp2515_Read(REC) );	UART_TxChar('\n');
	UART_TxStr_p( PSTR("Status:\n") ); 
	
	if( (data & 0x01) == 0x01 ){ UART_TxStr_p( PSTR("\t\tTEC or REC >= 96\n") ); }
	if( (data & 0x02) == 0x02 ){ UART_TxStr_p( PSTR("\t\tREC >= 96\n") ); }
	if( (data & 0x04) == 0x04 ){ UART_TxStr_p( PSTR("\t\tTEC >= 96\n") ); }
	if( (data & 0x08) == 0x08 ){ UART_TxStr_p( PSTR("\t\tError Passive - REC >= 128") ); }
	if( (data & 0x10) == 0x10 ){ UART_TxStr_p( PSTR("\t\tError Passive - TEC >= 128\n") ); }	
	if( (data & 0x20) == 0x20 ){ UART_TxStr_p( PSTR("\t\tBus Off - TEC = 255\n") ); }	
	if( (data & 0x40) == 0x40 ){ UART_TxStr_p( PSTR("\t\tRx0 Overflow\n") ); }	
	if( (data & 0x80) == 0x80 ){ UART_TxStr_p( PSTR("\t\tRx1 Overflow\n") ); }
}

/************************************************************************
 *	DISPLAY TRANSMIT BUFFER
 */
void term_TxBuffer(void)
{
	uint8_t  data[14];
	
	UART_TxStr_p( PSTR("\nTRANSMIT BUFFER -----------------------------------\n\n") );
	UART_TxStr_p( PSTR("\tSID\tEXIDE\tDLC\tCAN Data\n\n"));
	
	// ------------- TXB0
	UART_TxStr_p( PSTR("TXB0:\t") ); 
	mcp2515_ReadRegs( TXB0CTRL, data, 14 );
	term_BufTab( data );
	
	// --------------- TXB1
	UART_TxStr_p( PSTR("TXB1:\t") );
	mcp2515_ReadRegs( TXB1CTRL, data, 14 );
	term_BufTab( data );
	
	// --------------- TXB2
	UART_TxStr_p( PSTR("TXB2:\t") );
	mcp2515_ReadRegs( TXB2CTRL, data, 14 );
	term_BufTab( data );	
}


/************************************************************************
 *	DISPLAY RECEIVE BUFFER
 */
void term_RxBuffer (void)
{
	uint8_t  data[14];
	
	UART_TxStr_p( PSTR("\nRECEIVE BUFFER -----------------------------------\n\n") );
	UART_TxStr_p( PSTR("\tSID\tEXIDE\tDLC\tCAN Data\n\n"));
	
	// ------------- RXB0
	UART_TxStr_p( PSTR("RXB0:\t") ); 
	mcp2515_ReadRegs( RXB0CTRL, data, 14 );
	term_BufTab( data );
	
	// --------------- RXB1
	UART_TxStr_p( PSTR("RXB1:\t") );
	mcp2515_ReadRegs( RXB1CTRL, data, 14 );
	term_BufTab( data );
}

/************************************************************************
 *	TABULATE BUFFER DATA
 */
void term_BufTab( uint8_t *data)
{
	if( ( data[2] & TXB_EXIDE) == TXB_EXIDE  ){					// EID
		UART_TxHex( data[1] >> 3 );
		UART_TxHex( ( (data[1] & 0x07) << 5 ) | (data[2] & 0x03 ) | ( (data[2] & 0xE0) >>3 )  );	UART_TxChar('\t');	
		UART_TxHex( data[3] );									// EID8 
		UART_TxHex( data[4] );	UART_TxChar('\t');				// EID0	
	}	
	else{	
		UART_TxHex( data[1] >> 5 );								// SID
		UART_TxHex( (data[1] << 3) | ( data[2] >> 5) );	UART_TxChar('\t');
		UART_TxHex( 0x00 );									
		UART_TxHex( 0x00 );		UART_TxChar('\t');				
	}					
	
	UART_TxHex( data[5] );	UART_TxChar('\t');					// DLC
	
	for( uint8_t i = 6; (i < (6 + data[5])) && (i < 14); i++ ){	// Data
		UART_TxHex(data[i]);
		UART_TxChar(' ');
	}
	UART_TxChar('\n');	
}

/************************************************************************
 *	DISPLAY FILTERS
 */
void term_Filter (void)
{
	
}

/************************************************************************
 *	DISPLAY MASKS
 */
void term_Mask (void)
{
	
}

/************************************************************************
 *	DISPLAY RECEIVED MESSAGE
 */
void term_RxMsg	(volatile CanMessage *msg)
{
	UART_TxStr_p( PSTR("Rcvd:\t") );
	if( msg->ext == 0 ){
		UART_TxHex( (uint8_t) ( msg->id >> 8 ) );
		UART_TxHex( (uint8_t) ( msg->id ) );		UART_TxChar('\t');
		UART_TxHex( (uint8_t) ( msg->id >> 24 ) );
		UART_TxHex( (uint8_t) ( msg->id >> 16 ) );	UART_TxChar('\t');
	} 
	else{
		UART_TxHex( (uint8_t) ( msg->id >> 24 ) );
		UART_TxHex( (uint8_t) ( msg->id >> 16 ) );	UART_TxChar('\t');
		UART_TxHex( (uint8_t) ( msg->id >> 8 ) );
		UART_TxHex( (uint8_t) ( msg->id ) );		UART_TxChar('\t');
	}
	
	UART_TxHex( msg->dlc );						UART_TxChar('\t');
	
	for( uint8_t i = 0; i < msg->dlc; i++ ){
		UART_TxHex( msg->data[i] );				UART_TxChar(' ');	
	}
	UART_TxChar('\n');
}

