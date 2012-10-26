#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>
#include "mcp2515.h"
#include "mcp2515reg.h"
#include "can.h"
#include "spi.h"

#if ( TERMINAL == 1 )
#include "terminal.h"
#endif

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
void mcp2515_ReadRegs (const uint8_t address, uint8_t *data, const uint8_t n)
{
	MCP2515_SELECT();
	SPI_Write(READ);
	SPI_Write(address);
	
	for ( uint8_t i=0; i<n; i++) {									/* Auto-increment of address-pointer */
		data[i] = SPI_Read();	
	}
	MCP2515_DESELECT();	
}

/************************************************************************
 *	WRITE SINGLE REGISTER
 */
void mcp2515_Write ( const uint8_t address, const uint8_t data )
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
void mcp2515_WriteRegs (const uint8_t address, const uint8_t *data, const uint8_t n)
{	
	MCP2515_SELECT();
	SPI_Write(WRITE);
	SPI_Write(address);
	
	for ( uint8_t i=0; i<n; i++) {									/* auto-increment of address-pointer */
		SPI_Write(data[i]);
	}
	MCP2515_DESELECT();
}

/************************************************************************
 *	BITMODIFY
 */
void mcp2515_BitModify ( const uint8_t address, const uint8_t mask, const uint8_t data )
{
	MCP2515_SELECT();
	SPI_Write(BITMOD);
	SPI_Write(address);
	SPI_Write(mask);
	SPI_Write(data);
	MCP2515_DESELECT();
}

/************************************************************************
 *	READ/STORE RECEIVE BUFFER
 */
void mcp2515_StrRXBUF ( const uint8_t buffer, uint8_t *data, const uint8_t n )
{
	MCP2515_SELECT();
	SPI_Write(buffer);
	
	for ( uint8_t i=0; i<n; i++) {									/* Auto-increment of address-pointer */
		data[i] = SPI_Read();	
	}
	MCP2515_DESELECT(); 
}

/************************************************************************
 *	LOAD TRANSMIT BUFFER
 */
void mcp2515_LoadTXBUF ( const uint8_t buffer, const uint8_t *data, const uint8_t n )
{
	MCP2515_SELECT();
	SPI_Write(buffer);
	
	for ( uint8_t i=0; i<n; i++) {									/* Auto-increment of address-pointer */
		SPI_Write(data[i]);
	}
	
	MCP2515_DESELECT();
}

/************************************************************************
 *	REQUEST TO SEND
 */
void mcp2515_RTS ( const uint8_t buffer )
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
mcp2515Status mcp2515_SetMode (const uint8_t mode)
{
	uint8_t i;
	
	mcp2515_BitModify( CANCTRL, MODE_MASK, mode);
	
	i = mcp2515_Read(CANCTRL);										/* Recheck */
	i &= MODE_MASK;
	
	if ( i == mode ) 
		return MCP2515_OK; 
	else 
		return MCP2515_FAILED;
}

/************************************************************************
 *	CONFIGURATION RATE
 */
mcp2515Status mcp2515_ConfigRate ( const uint8_t can_rate )
{
	uint8_t propseg, phseg1, phseg2, BRP, sjw;
	
	switch( can_rate ){
		
		case CAN_20KBPS : BRP = 24; propseg = 0x01; phseg1 = 0x30; phseg2 = 0x05; sjw = SJW2;	break;		
		case CAN_25KBPS	: BRP = 19; propseg = 0x01; phseg1 = 0x30; phseg2 = 0x05; sjw = SJW2;	break;	
		case CAN_50KBPS	: BRP = 9;  propseg = 0x01; phseg1 = 0x30; phseg2 = 0x05; sjw = SJW2;	break;	
		case CAN_100KBPS: BRP = 4;  propseg = 0x01; phseg1 = 0x30; phseg2 = 0x05; sjw = SJW2;	break;		
		case CAN_125KBPS: BRP = 3;  propseg = 0x01; phseg1 = 0x30; phseg2 = 0x05; sjw = SJW2;	break;		
		case CAN_250KBPS: BRP = 1;  propseg = 0x01; phseg1 = 0x30; phseg2 = 0x05; sjw = SJW2;	break;		
		case CAN_500KBPS: BRP = 0;  propseg = 0x01; phseg1 = 0x30; phseg2 = 0x05; sjw = SJW2;	break;			  	
		case CAN_1MBPS	: BRP = 0;  propseg = 0x01; phseg1 = 0x80; phseg2 = 0x01; sjw = SJW1;	break;
						  
		default:		  return MCP2515_FAILED; 
						  break;		
	}
	
	mcp2515_Write( CNF1, BRP | sjw );
	mcp2515_Write( CNF2, BTLMODE | SAMPLE_1X | propseg | phseg1 );
	mcp2515_Write( CNF3, phseg2 | SOF_ENABLE );
	
	return MCP2515_OK;
}

/************************************************************************
 *	SET BUFFERS
 */
void mcp2515_ClrBuffers	(void)
{
	uint8_t data[14];
	
	for( uint8_t i = 0; i < 14; i++ )								/* Write zeros */
		data[i] = 0;
	
	mcp2515_WriteRegs( TXB0SIDH, data, 14 );						/* Erase Buffers */
	mcp2515_WriteRegs( TXB1SIDH, data, 14 );
	mcp2515_WriteRegs( TXB2SIDH, data, 14 );
	mcp2515_WriteRegs( RXB0SIDH, data, 14 );
	mcp2515_WriteRegs( RXB1SIDH, data, 14 );
}

/************************************************************************
 *	PIN CONFIGURATIONS
 */
void mcp2515_ConfigPins  (void)
{
	/* RXB0PIN: Output, RXB1PIN: Receive Interrupt*/
	mcp2515_Write( BFPCTRL, RXB0_PIN_ENABLE | RXB1_PIN_ENABLE | RXB1_INTERRUPT	);					
	
	/* Deactivate RTS Transmit Pins */
	mcp2515_BitModify( TXRTSCTRL, 0x07, TXB_RTS_SETALLINPUT );			
}

/************************************************************************
 *	WRITE MASKS
 */
void mcp2515_WriteMaskFilt ( const uint8_t addr, const uint32_t m, const uint8_t ext )
{
	uint8_t buf[4];
	
	if( ext == 0 ){													/* Standard Identifier */
		buf[0] = (uint8_t) ( m >> 3 );
		buf[1] = ( (uint8_t) m << 5 );
		buf[2] = 0x00;
		buf[3] = 0x00;		
	}
	else{															/* Extended Identifier */	
		buf[0] = (uint8_t) ( m >> 21 );
		buf[1] = (uint8_t) ( ( ( (uint8_t)( m >> 16) ) & 0x03 ) | ( ( (uint8_t)( m >> 13 ) ) & 0xE0 ) | TXB_EXIDE );
		buf[2] = (uint8_t) ( m >> 8 );
		buf[3] = (uint8_t) ( m );			
	}
	
	mcp2515_WriteRegs( addr, buf, 4 );								/* Write to Mask/Filter */
}

/************************************************************************
 *	CONFIGURE FILTERS
 */
void mcp2515_ConfigFilt ( const uint8_t mode )
{
	/* Write Masks in Configuration mode */
	mcp2515_WriteMaskFilt( RXM0SIDH, MASK0, mode );					/* MASK 1 */
	mcp2515_WriteMaskFilt( RXM1SIDH, MASK1, mode );					/* MASK 2 */

	/* Write Filters in Configuration Mode */
	mcp2515_WriteMaskFilt( RXF0SIDH, FILTER0, mode );				/* Filter 0 */
	mcp2515_WriteMaskFilt( RXF1SIDH, FILTER1, mode );				/* Filter 1 */
	mcp2515_WriteMaskFilt( RXF2SIDH, FILTER2, mode );				/* Filter 2 */
	mcp2515_WriteMaskFilt( RXF3SIDH, FILTER3, mode );				/* Filter 3 */
	mcp2515_WriteMaskFilt( RXF4SIDH, FILTER4, mode );				/* Filter 4 */
	mcp2515_WriteMaskFilt( RXF5SIDH, FILTER5, mode );				/* Filter 5 */
}
/************************************************************************
 *	WRITE TRANSMIT BUFFER
 */
void mcp2515_WriteTxBuf ( const CanMessage *m, uint8_t addr )
{
	uint8_t buf[13];
	
	if( m->ext == 0 ){												/* Standard Identifier */
		buf[0] = (uint8_t) ( m->id >> 3 );
		buf[1] = ( (uint8_t) m->id << 5 );
		buf[2] = 0;
		buf[3] = 0;		
	}
	else{															/* Extended Identifier */	
		buf[0] = (uint8_t) ( m->id >> 21 );
		buf[1] = (uint8_t) ( ( ( (uint8_t)( m->id >> 16) ) & 0x03 ) | ( ( (uint8_t)( m->id >> 13 ) ) & 0xE0 ) | TXB_EXIDE );
		buf[2] = (uint8_t) ( m->id >> 8 );
		buf[3] = (uint8_t) ( m->id );			
	}

	if( m->rtr == 1 ){												/* Write RTR */
		buf[4] = (uint8_t) (TXB_RTR | m->dlc) ;
	}	
	else{	
		buf[4] = m->dlc;											/* Write DLC and Data */
		for( uint8_t i = 0; (i < m->dlc ) && (i < 8); i++ )
			buf[5+i] = (uint8_t) m->data[i];
	}	
	
	mcp2515_LoadTXBUF( addr, buf, 5 + m->dlc );						/* Write to Buffer */	
}

/************************************************************************
 *	READ RECEIVE BUFFER
 */
void mcp2515_ReadRxBuf ( volatile CanMessage *m, uint8_t addr )
{
	uint32_t temp;
	uint8_t  buf[13];
	
	mcp2515_StrRXBUF( addr, buf, 13 );								/* Read Buffer */
	
	if( buf[1] & RXB_REC_EXIDE ){									/* Extended Identifier */
			
			temp =  ( (uint32_t) buf[0] << 21 );    
			temp |= ( (uint32_t) ( buf[1] & 0xE0 ) << 13 );
			temp |= ( (uint32_t) ( buf[1] & 0x03 ) << 16 );
			temp |= (uint32_t) ( buf[2] << 8 );
			temp |= (uint32_t) ( buf[3] );
			m->ext = 1;
	}
	else {															/* Standard Identifier */
		temp =  ( (uint32_t) buf[0] << 3 ) | ( (uint32_t) buf[1] >> 5 ) ;
		m->ext = 0;	
	}
		
	m->id = temp;													/* Identifier */
		
	if( buf[1] & RXB_REC_SRR )										/* Remote Transmission Request */
		m->rtr = 1;
	else
		m->rtr = 0;
		
	m->dlc = buf[4];												/* Data length code	*/ 
		 
	for( uint8_t i = 5; (i < (5 + buf[4])) && (i < 13); i++ )			
		m->data[i - 5] = buf[i];									/* Data */
}

/************************************************************************
 *	CHECK FREE TRANSMIT BUFFER
 */
mcp2515Status mcp2515_ChkFreeTxBuf ( uint8_t *addr )
{
	/* [0] : load addr , [1] = RTS addr */
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
		return MCP2515_FAILED;		
	}
	return MCP2515_OK;
}

/************************************************************************
 *	MCP2515 INITIALIZATION
 */
mcp2515Status mcp2515_Init ( const uint8_t can_rate )
{
	mcp2515_Reset();												/* Reset */ 
	_delay_ms(1);													/* Settling time */
	
	if( mcp2515_SetMode ( MODE_CONFIG ) == MCP2515_FAILED )			/* Configuration Mode */
		return MCP2515_FAILED;	
		
	if( mcp2515_ConfigRate( can_rate ) == MCP2515_FAILED )  		/* Set CAN Speed */
		return MCP2515_FAILED;
	
	mcp2515_ClrBuffers();											/* Clear Rx/Tx Buffers */
	mcp2515_ConfigPins();											/* Set Pins */
	mcp2515_Write( CANINTE, RX0IE | RX1IE );						/* Activate Interrupts */
	
#if ( CAN_FILTER_ENABLE == 1 )										/* Filter Enabled */
	mcp2515_ConfigFilt( CAN_MODE );									/* Set Filters */
	mcp2515_Write( RXB0CTRL, RXB0_REC_ALL_VALID );					/* RXB0 Disable Rollover, Receive Filter Match */
	mcp2515_Write( RXB1CTRL, RXB1_REC_ALL_VALID );					/* RXB1 Receive Filter Match */											
#else																/* Filter Disabled */
	mcp2515_Write( RXB0CTRL, RXB0_REC_ANY );						/* No RXB0 Enable Rollover, Receive All */
	mcp2515_Write( RXB1CTRL, RXB1_REC_ANY );						/* RXB1 Receive All */
#endif

	if ( mcp2515_SetMode( MODE_NORMAL ) == MCP2515_FAILED)			/* Normal Mode */
		return MCP2515_FAILED;

	return MCP2515_OK;
}