#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>
#include "terminal.h"
#include "uart.h"
#include "uart_buffer.h"
#include "mcp2515reg.h"
#include "mcp2515.h"

/************************************************************************
 *	GLOBAL VARIABLES
 */
volatile	TERM_STATE			state	= TERM_DISABLE;
volatile	CFG_STATE			cfg		= CFG_NORMAL;
volatile	MSGSTRM_STATE		strm	= MS_DISABLE; 

extern volatile UartBuffer UART_RxBuffer;

/************************************************************************
 *	START SCREEN
 */
void term_Start( CanStatus res )
{
	UART_TxStr_p( PSTR("\nCAN NodeView v1.0 (c) Ayush Sinha\n") );
	
	if( res == CAN_OK )
		UART_TxStr_p( PSTR("\nCAN Initialized\n") ); 
    else 
		UART_TxStr_p( PSTR("\nCAN Initialization Failed\n") ); 
	
	term_Commands();										/* Show commands */
}

/************************************************************************
 *	GET STATE
 */
TERM_STATE	term_GetState	( uint8_t s )
{
	switch(s) {
		
		case 'i': return TERM_CANINIT;	break;
		case 'c': return TERM_CTRLREG;  break;
		case 'x': return TERM_RXSTAT;	break;
		case 's': return TERM_READSTAT; break;
		case 'f': return TERM_INTFLAG;	break;
		case 'e': return TERM_ERRFLAG;	break;
		case 't': return TERM_TXBUF;	break;
		case 'r': return TERM_RXBUF;	break;
		case 'k': return TERM_MASK;		break;
		case 'g': return TERM_FILT;		break;
		case 'm': return TERM_MSGSTREAM;break;
		case 'l': return TERM_LOOPBACK; break;
		case 'h': return TERM_HELP;		break;
		default : return TERM_DISABLE;	break;
	}
}

/************************************************************************
 *	MAIN FUNCTION ROUTINE
 */
void term_Main( void )
{
	if( UART_BufState( &UART_RxBuffer ) != UART_BUFFER_EMPTY )
		state = UART_BufDeq( &UART_RxBuffer );				/* Dequeue */

	switch( term_GetState(state) ) {						/* terminal function call */
																
		case TERM_CANINIT	: term_Start( CAN_Init(CAN_SPEED) );	break;
		case TERM_CTRLREG	: term_CtrlReg();						break;
		case TERM_RXSTAT	: term_RxStatus();						break;
		case TERM_READSTAT	: term_ReadStatus();					break;
		case TERM_INTFLAG	: term_IntFlag();						break;
		case TERM_ERRFLAG	: term_ErrorFlag();						break;
		case TERM_TXBUF		: term_TxBuffer();						break;
		case TERM_RXBUF		: term_RxBuffer();						break;
		case TERM_MASK      : term_Mask();							break;
		case TERM_FILT		: term_Filt();							break;
		
		case TERM_MSGSTREAM	: if( strm == MS_DISABLE ){		/* Enable/Disable Streaming of Received Data */	
								strm = MS_STREAM;
								UART_TxStr_p( PSTR("STREAM\n") );					  		
							  }
							  else{
								strm = MS_DISABLE;
							  }
							  break;
							  
		case TERM_LOOPBACK  : if( cfg == CFG_NORMAL ){		/* Change to Loop-back Mode */
								mcp2515_SetMode( MODE_LOOPBACK ); 
								cfg = CFG_LOOPBACK;	
								UART_TxStr_p( PSTR("LOOPBACK MODE\n") );
							  }
							  else { 
								mcp2515_SetMode( MODE_NORMAL );
								cfg = CFG_NORMAL;
								UART_TxStr_p( PSTR("NORMAL MODE\n") );
							  } 
							  break;
							  
		case TERM_HELP		: term_Commands();   break;	
		
					default	: break;
	}	
																				
	state = TERM_DISABLE;									/* Disable multiple print */
}

/************************************************************************
 *	DISPLAY COMMAND REGISTERS
 */
void term_Commands	(void)
{
	UART_TxStr_p( PSTR("\nCOMMANDS \n") );
	UART_TxStr_p( PSTR("i : \tCAN Initialize\n") );
	UART_TxStr_p( PSTR("c : \tControl Registers\n") );
	UART_TxStr_p( PSTR("x : \tRX Status\n") );
	UART_TxStr_p( PSTR("s : \tRead Status\n") );
	UART_TxStr_p( PSTR("f : \tInterrupt Flags\n") );
	UART_TxStr_p( PSTR("e : \tError Flags\n") );
	UART_TxStr_p( PSTR("t : \tTransmit Buffer\n") );
	UART_TxStr_p( PSTR("r : \tReceive Buffer\n") );
	UART_TxStr_p( PSTR("k : \tMasks\n") );
	UART_TxStr_p( PSTR("g : \tFilters\n") );
	UART_TxStr_p( PSTR("m : \tMessage Stream\n") );
	UART_TxStr_p( PSTR("l : \tLoopback\n") );
	UART_TxStr_p( PSTR("h : \tHelp\n") );
}

/************************************************************************
 *	DISPLAY CONTROL REGISTERS 
 */
void term_CtrlReg(void)
{
	uint8_t stat = mcp2515_Read(CANSTAT);					/* Read Status */
	
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
	uint8_t  data[13];
	
	UART_TxStr_p( PSTR("\n\nTRANSMIT BUFFER -----------------------------------\n\n") );
	UART_TxStr_p( PSTR("\tSID\tEXIDE\tDLC\tCAN Data\n\n"));
	
	/* TXB0 */
	UART_TxStr_p( PSTR("TXB0:\t") ); 
	mcp2515_ReadRegs( TXB0SIDH, data, 13 );
	term_BufTab( TXB0SIDH, data );
	
	/* TXB1 */
	UART_TxStr_p( PSTR("TXB1:\t") );
	mcp2515_ReadRegs( TXB1SIDH, data, 13 );
	term_BufTab( TXB1SIDH, data );
	
	/* TXB2 */
	UART_TxStr_p( PSTR("TXB2:\t") );
	mcp2515_ReadRegs( TXB2SIDH, data, 13 );
	term_BufTab( TXB2SIDH, data );	
}

/************************************************************************
 *	DISPLAY RECEIVE BUFFER
 */
void term_RxBuffer (void)
{
	uint8_t  data[13];
	
	UART_TxStr_p( PSTR("\n\nRECEIVE BUFFER -----------------------------------\n\n") );
	UART_TxStr_p( PSTR("\tSID\tEXIDE\tDLC\tCAN Data\n\n"));
	
	/* RXB0 */
	UART_TxStr_p( PSTR("RXB0:\t") ); 
	mcp2515_ReadRegs( RXB0SIDH, data, 13 );
	term_BufTab( RXB0SIDH, data );
	
	/* RXB1 */
	UART_TxStr_p( PSTR("RXB1:\t") );
	mcp2515_ReadRegs( RXB1SIDH, data, 13 );
	term_BufTab( RXB1SIDH, data );
}


/************************************************************************
 *	DISPLAY MASKS
 */
void term_Mask (void)
{
	mcp2515_SetMode( MODE_CONFIG );									/* Only accessible in this mode */
	
	uint8_t  data[4];
	
	UART_TxStr_p( PSTR("\n\nMASKS -----------------------------------\n\n") );
	UART_TxStr_p( PSTR("\tSID\tEXIDE\n\n"));
	
	/* RXM0 */
	UART_TxStr_p( PSTR("RXM0:\t") ); 
	mcp2515_ReadRegs( RXM0SIDH, data, 4 );
	term_BufTab( RXM0SIDH, data );
	
	/* RXM1 */
	UART_TxStr_p( PSTR("\nRXM1:\t") );
	mcp2515_ReadRegs( RXM1SIDH, data, 4 );
	term_BufTab( RXM1SIDH, data );	
	
	mcp2515_SetMode( MODE_NORMAL );									/* Reset to Normal Mode */
}

/************************************************************************
 *	DISPLAY FILTERS
 */
void term_Filt (void)
{
	mcp2515_SetMode( MODE_CONFIG );									/* Only accessible in this mode */
	
	uint8_t  data[4];
	
	UART_TxStr_p( PSTR("\n\nFILTERS -----------------------------------\n\n") );
	UART_TxStr_p( PSTR("\tSID\tEXIDE\n\n"));
	
	/* FILT0 */
	UART_TxStr_p( PSTR("FILT0:\t") ); 
	mcp2515_ReadRegs( RXF0SIDH, data, 4 );
	term_BufTab( RXF0SIDH, data );
	
	/* FILT1 */
	UART_TxStr_p( PSTR("\nFILT1:\t") );
	mcp2515_ReadRegs( RXF1SIDH, data, 4 );
	term_BufTab( RXF1SIDH, data );	
	
	/* FILT2 */
	UART_TxStr_p( PSTR("\nFILT2:\t") );
	mcp2515_ReadRegs( RXF2SIDH, data, 4 );
	term_BufTab( RXF2SIDH, data );	
	
	/* FILT3 */
	UART_TxStr_p( PSTR("\nFILT3:\t") );
	mcp2515_ReadRegs( RXF3SIDH, data, 4 );
	term_BufTab( RXF3SIDH, data );	
	
	/* FILT4 */
	UART_TxStr_p( PSTR("\nFILT4:\t") );
	mcp2515_ReadRegs( RXF4SIDH, data, 4 );
	term_BufTab( RXF4SIDH, data );	
	
	/* FILT5 */
	UART_TxStr_p( PSTR("\nFILT5:\t") );
	mcp2515_ReadRegs( RXF5SIDH, data, 4 );
	term_BufTab( RXF5SIDH, data );	
	
	mcp2515_SetMode( MODE_NORMAL );									/* Reset to Normal Mode */	
}

/************************************************************************
 *	TABULATE BUFFER DATA
 */
void term_BufTab( uint8_t addr, uint8_t *data)
{
	if( ( data[1] & TXB_EXIDE) == TXB_EXIDE  ){						/* EID */
		UART_TxHex( data[0] >> 3 );
		UART_TxHex( ( (data[0] & 0x07) << 5 ) | (data[1] & 0x03 ) | ( (data[1] & 0xE0) >>3 )  );	UART_TxChar('\t');	
		UART_TxHex( data[2] );										/* EID8 */
		UART_TxHex( data[3] );	UART_TxChar('\t');					/* EID0 */	
	}	
	else{	
		UART_TxHex( data[0] >> 5 );									/* SID */
		UART_TxHex( (data[0] << 3) | ( data[1] >> 5) );	UART_TxChar('\t');
		UART_TxChar('\t');				
	}					
	
	uint8_t p;
	
	switch( addr ){													/* Check Address */
		case TXB0SIDH :
		case TXB1SIDH :
		case TXB2SIDH :
		case RXB0SIDH :
		case RXB1SIDH : p = 1; break;
			  default : p = 0; break;
	}
	
	if( p == 1 ){													/* Only for Rx/Tx Buffers */						
		
		UART_TxHex( data[4] );	UART_TxChar('\t');					/* DLC */
	
		for( uint8_t i = 5; (i < (5 + data[4])) && (i < 13); i++ ){	/* Data */
			UART_TxHex(data[i]);
			UART_TxChar(' ');
		}
		UART_TxChar('\n');		
	}
	
}

/************************************************************************
 *	DISPLAY RECEIVED MESSAGE
 */
void term_RxMsg	(CanMessage *msg)
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
	
	UART_TxHex( msg->dlc );							UART_TxChar('\t');
	
	for( uint8_t i = 0; i < msg->dlc; i++ ){
		UART_TxHex( msg->data[i] );					UART_TxChar(' ');	
	}
	UART_TxChar('\n');
}

/************************************************************************
 *	RECEIVE INTERRUPT
 */
ISR( USART_RXC_vect )
{
	uint8_t data = UDR;
	
	if( UART_BufState( &UART_RxBuffer) != UART_BUFFER_FULL )		
		UART_BufEnq( &UART_RxBuffer, data );						/* Enqueue incoming commands to buffer */
}

