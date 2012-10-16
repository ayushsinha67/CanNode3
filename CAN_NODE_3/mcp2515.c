#include <avr/io.h>
#include <util/delay.h>
#include "mcp2515.h"
#include "spi.h"

extern char data;

/************************************************************************
 *	RESET MCP2515
 */
void mcp2515_Reset (void)
{
	MCP2515_SELECT();
	SPI_Write(RESET);
	MCP2515_DESELECT();
	_delay_ms(10);
}

/************************************************************************
 *	READ SINGLE REGISTER
 */
uint8_t mcp2515_Read ( const uint8_t address )
{
	uint8_t data;
	
	MCP2515_SELECT();
	SPI_Write(READ);
	SPI_Write(address);
	data = SPI_Read();
	MCP2515_DESELECT();
	
	return data;
}


/************************************************************************
 *	READ MULTIPLE REGISTERS
 */
void mcp2515_ReadRegs (const uint8_t address, uint8_t data[], const uint8_t n)
{
	MCP2515_SELECT();
	SPI_Write(READ);
	SPI_Write(address);
	
	for ( uint8_t i=0; i<n; i++) {		/* Auto-increment of address-pointer */
		data[i] = SPI_Read();	
	}
	MCP2515_DESELECT();	
}

/************************************************************************
 *	WRITE SINGLE REGISTER
 */
void	mcp2515_Write ( const uint8_t address, const uint8_t data )
{
	MCP2515_SELECT();
	SPI_Write(WRITE);
	SPI_Write(address);
	SPI_Write(data);
	MCP2515_DESELECT();
}

/************************************************************************
 *	WRITE MULTIPLE REGISTER
 */
void	mcp2515_WriteRegs (const uint8_t address, const uint8_t data[], const uint8_t n)
{	
	MCP2515_SELECT();
	SPI_Write(WRITE);
	SPI_Write(address);
	
	for ( uint8_t i=0; i<n; i++) {		/* auto-increment of address-pointer */
		SPI_Write(data[i]);
	}
	MCP2515_DESELECT();
}

/************************************************************************
 *	BITMODIFY
 */
void    mcp2515_BitModify ( const uint8_t address, const uint8_t mask, const uint8_t data )
{
	MCP2515_SELECT();
	SPI_Write(BITMOD);
	SPI_Write(address);
	SPI_Write(mask);
	SPI_Write(data);
	MCP2515_DESELECT();
}

/************************************************************************
 *	READ RECEIVE BUFFER
 */
void mcp2515_StrRXBUF ( const uint8_t buffer, uint8_t data[], const uint8_t n )
{
	MCP2515_SELECT();
	SPI_Write(buffer);
	
	for ( uint8_t i=0; i<n; i++) {		/* Auto-increment of address-pointer */
		data[i] = SPI_Read();	
	}
	MCP2515_DESELECT(); 
}

/************************************************************************
 *	LOAD TRANSMIT BUFFER
 */
void mcp2515_LoadTXBUF ( const uint8_t buffer, const uint8_t data[], const uint8_t n )
{
	MCP2515_SELECT();
	SPI_Write(buffer);
	
	for ( uint8_t i=0; i<n; i++) {		/* Auto-increment of address-pointer */
		SPI_Write(data[i]);
	}
	
	MCP2515_DESELECT();
}

/************************************************************************
 *	REQUEST TO SEND
 */
void    mcp2515_RTS	( const uint8_t buffer )
{
	MCP2515_SELECT();
	SPI_Write(buffer);
	MCP2515_DESELECT();
}

/************************************************************************
 *	READ STATUS
 */
uint8_t	mcp2515_ReadStatus (void)
{
	uint8_t data;
	
	MCP2515_SELECT();
	SPI_Write(READ_STATUS);
	data = SPI_Read();
	MCP2515_DESELECT();
	
	return data;
}

/************************************************************************
 *	RECEIVE STATUS
 */
uint8_t	mcp2515_RXStatus (void)
{
	uint8_t data;
	
	MCP2515_SELECT();
	SPI_Write(RX_STATUS);
	data = SPI_Read();
	MCP2515_DESELECT();
	
	return data;
}

/************************************************************************
 *	SET MODE
 */
enum MCP2515_STATUS mcp2515_SetMode (const uint8_t mode)
{
	uint8_t i;
	
	mcp2515_BitModify( CANCTRL, MODE_MASK, mode);
	
	i = mcp2515_Read(CANCTRL);								/* Recheck */
	i &= MODE_MASK;
	
	if ( i == mode ) 
		return OK; 
	else 
		return FAILED;
}

/************************************************************************
 *	CONFIGURATION RATE
 */
enum MCP2515_STATUS mcp2515_ConfigRate (const uint16_t can_rate)
{
	uint8_t BRP = ( F_OSC / ( 2 * 16 * can_rate ) ) - 1 ;	/* Baud Rate Prescalar */
	
	switch( can_rate ){
		
		case CAN_20KBPS :		
		case CAN_25KBPS	:	
		case CAN_50KBPS	:	
		case CAN_100KBPS:		
		case CAN_125KBPS:		
		case CAN_250KBPS:		
		case CAN_500KBPS:		
		case CAN_750KBPS:		
		case CAN_1MBPS	: break;
		default:		  return FAILED;		
	}
	
	if( can_rate == CAN_1MBPS ){							/* 1 MBPS Setting */
		mcp2515_Write( CNF1, SJW1 );
		mcp2515_Write( CNF2, ( BTLMODE | SAMPLE_1X | PROPSEG_1MBPS | PHSEG1_1MBPS ) );
		mcp2515_Write( CNF3, ( PHSEG2_1MBPS | SOF_ENABLE) );
	}
	
	else{													/* Write Configurations */
		mcp2515_Write( CNF1, ( BRP | SJW2 ) );				
		mcp2515_Write( CNF2, ( BTLMODE | SAMPLE_1X | PROPSEG | PHSEG1 )  );
		mcp2515_Write( CNF3, ( PHSEG2 | SOF_ENABLE ) );	
	}
	
	return OK;
}

/************************************************************************
 *	SET BUFFERS
 */
void mcp2515_ClrBuffers	(void)
{
	/* Erase Buffers */
	mcp2515_Write( TXB0CTRL, 0x00 );	
	mcp2515_Write( TXB1CTRL, 0x00 );
	mcp2515_Write( TXB2CTRL, 0x00 );
	mcp2515_Write( RXB0CTRL, 0x00 );
	mcp2515_Write( RXB1CTRL, 0x00 );	
	
	mcp2515_Write( TXB0DLC,  0x00 );
	mcp2515_Write( TXB0SIDH, 0x00 );
	mcp2515_Write( TXB0SIDL, 0x00 );
	mcp2515_Write( TXB0EID8, 0x00 );
	mcp2515_Write( TXB0EID0, 0x00 );
	
	mcp2515_Write( TXB1DLC,  0x00 );
	mcp2515_Write( TXB1SIDH, 0x00 );
	mcp2515_Write( TXB1SIDL, 0x00 );
	mcp2515_Write( TXB1EID8, 0x00 );
	mcp2515_Write( TXB1EID0, 0x00 );
	
	mcp2515_Write( TXB2DLC,  0x00 );
	mcp2515_Write( TXB2SIDH, 0x00 );
	mcp2515_Write( TXB2SIDL, 0x00 );
	mcp2515_Write( TXB2EID8, 0x00 );
	mcp2515_Write( TXB2EID0, 0x00 );
	
}

/************************************************************************
 *	PIN CONFIGURATIONS
 */
void mcp2515_ConfigPins  (void)
{
	/* Activate Receive Buffer pins in Interrupt Mode */
	mcp2515_Write( BFPCTRL, RXB0_PIN_ENABLE | RXB1_PIN_ENABLE | RXB0_INTERRUPT | RXB1_INTERRUPT	);						
	
	/* Deactivate RTS Transmit Pins */
	mcp2515_BitModify( TXRTSCTRL, 0x07, 0x00 );			
}

/************************************************************************
 *	CONFIGURE FILTERS
 */
void mcp2515_ConfigFilt (void)
{
	/* Write Masks */
	
	
	/* Write Filters */
}
/************************************************************************
 *	WRITE TRANSMIT BUFFER
 */
void mcp2515_WriteTxBuf ( const CanMessage *m, uint8_t addr )
{
	uint8_t buf[13];
	
	if( m->ext == 0 ){									/* Standard Identifier */
		buf[0] = (uint8_t) ( m->id >> 3 );
		buf[1] = ( (uint8_t) m->id << 5 );
		buf[2] = 0x00;
		buf[3] = 0x00;		
	}
	else{												/* Extended Identifier */	
		buf[0] = (uint8_t) ( m->id >> 21 );
		buf[1] = (uint8_t) ( ( ( (uint8_t)( m->id >> 16) ) & 0x03 ) | ( ( (uint8_t)( m->id >> 13 ) ) & 0xE0 ) | TXB_EXIDE );
		buf[2] = (uint8_t) ( m->id >> 8 );
		buf[3] = (uint8_t) ( m->id );			
	}

	if( m->rtr == 1 ){									/* Write RTR */
		buf[4] = (uint8_t) (TXB_RTR | m->dlc) ;
	}	
	else{	
		buf[4] = m->dlc;								/* Write DLC and Data */
		for( uint8_t i = 0; (i < m->dlc ) && (i < 8); i++ )
			buf[5+i] = (uint8_t) m->data[i];
	}	
	
	mcp2515_LoadTXBUF( addr, buf, 5 + m->dlc );			/* Write to Buffer */	
}

/************************************************************************
 *	READ RECEIVE BUFFER
 */
void mcp2515_ReadRxBuf ( CanMessage *m, uint8_t addr )
{
	uint32_t temp;
	uint8_t  buf[13];
	
	mcp2515_StrRXBUF( addr, buf, 13 );					/* Read Buffer */
	
	if( buf[1] & RXB_REC_EXIDE ){						/* Extended Identifier */
			
			temp =  ( (uint32_t) buf[0] << 21 );    
			temp |= ( (uint32_t) ( buf[1] & 0xE0 ) << 13 );
			temp |= ( (uint32_t) ( buf[1] & 0x03 ) << 16 );
			temp |= (uint32_t) ( buf[2] << 8 );
			temp |= (uint32_t) ( buf[3] );
			m->ext = 1;
		}
		else {											/* Standard Identifier */
			temp =  ( (uint32_t) buf[0] << 3 ) | ( (uint32_t) buf[1] >> 5 ) ;
			m->ext = 0;	
		}
		
		m->id = temp;									/* Identifier */
		
		if( buf[1] & RXB_REC_SRR )						/* Remote Transmission Request */
			m->rtr = 1;
		else
			m->rtr = 0;
		
		m->dlc =	buf[4];								/* Data length code	*/ 
		 
		for( uint8_t i = 5; (i < (5 + buf[4])) && (i < 13); i++ )			
			m->data[i - 5] = buf[i];					/* Data */
}

/************************************************************************
 *	CHECK FREE TRANSMIT BUFFER
 */
enum MCP2515_STATUS mcp2515_ChkFreeTxBuf ( uint8_t addr[] )
{
	/* [0] : load addr , [1] = rts addr */
	uint8_t status = mcp2515_ReadStatus();
	
	if	( ( status & TXB0_TXREQ_STATUS ) == 0 ) {						
		addr[0] = LOAD_TXB0_SIDH;
		addr[1] = RTS_TXB0; 											
	}
	else if ( ( status & TXB1_TXREQ_STATUS ) == 0 ) { 
		addr[0] = LOAD_TXB1_SIDH;
		addr[1] = RTS_TXB1;
	} 
	else if ( ( status & TXB2_TXREQ_STATUS ) == 0 ) { 
		addr[0] = LOAD_TXB2_SIDH;
		addr[1] = RTS_TXB2;
	}
	else {	
		return FAILED;		
	}
	return OK;
}

/************************************************************************
 *	MCP2515 INITIALIZATION
 */
enum MCP2515_STATUS mcp2515_Init (const uint8_t can_rate)
{
	mcp2515_Reset();												/* Reset */ 
	_delay_ms(1);													/* Settling time */
	
	if( mcp2515_SetMode ( MODE_CONFIG ) == FAILED )					/* Configuration Mode */
		return FAILED;	
		
	if( mcp2515_ConfigRate( can_rate ) == FAILED )  				/* Set CAN Speed */
		return FAILED;
	
	mcp2515_ClrBuffers();											/* Clear Rx/Tx Buffers */
	mcp2515_ConfigFilt();											/* Set Filters */
	mcp2515_ConfigPins();											/* Set Pins */
	mcp2515_Write( CANINTE, RX0IE | RX1IE );						/* Activate Interrupts */
	mcp2515_Write( RXB0CTRL, RXB0_ROLLOVER | RXB0_REC_ANY );		/* RXB0 Enable Rollover, Receive All */
	mcp2515_Write( RXB1CTRL, RXB1_REC_ANY );						/* RXB1 Receive All */											
	
	if ( mcp2515_SetMode( MODE_NORMAL ) == FAILED)					/* Normal Mode */
		return FAILED;

	return OK;
}