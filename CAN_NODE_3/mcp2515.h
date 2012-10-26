#ifndef MCP2515_H_
#define MCP2515_H_

#include <avr/io.h>
#include "can.h"

/************************************************************************
 *	SPI COMMANDS
 */
#define RESET				0xC0	/* Read/Write */
#define WRITE				0x02
#define READ				0x03
#define BITMOD				0x05

#define READ_RXB0_SIDH		0x90	/* Read Receive Buffer */
#define READ_RXB0_D0		0x92
#define READ_RXB1_SIDH		0x94
#define READ_RXB1_D0		0x96

#define LOAD_TXB0_SIDH		0x40	/* Load Transmit Buffer */
#define LOAD_TXB0_D0		0x41
#define LOAD_TXB1_SIDH		0x42
#define LOAD_TXB1_D0		0x43
#define LOAD_TXB2_SIDH		0x44
#define LOAD_TXB2_D0		0x45

#define RTS_TXB0			0x81	/* Request to Send */
#define RTS_TXB1			0x82
#define RTS_TXB2			0x84
#define RTS_ALL				0x87

#define READ_STATUS			0xA0	/* Read Status */
#define RX_STATUS			0xB0

/************************************************************************
 *	INTERRUPTS
 */
#define ALL_TX_INT			0x1C	/* Enable all transmit interrupts */
#define TXB01_INT			0x0C	/* Enable TXB0 and TXB1 interrupts */
#define RX_INT				0x03	/* Enable receive interrupts */
#define NO_INT				0x00	/* Disable all interrupts */

/************************************************************************
 *	MASKS
 */
#define MODE_MASK			0xE0	/* Mode Mask */

#define RX0IF_STATUS		0x01	/* Read Status */
#define RX1IF_STATUS		0x02
#define TXB0_TXREQ_STATUS	0x04
#define TX0IF_STATUS		0x08
#define TXB1_TXREQ_STATUS	0x10
#define TX1IF_STATUS		0x20
#define TXB2_TXREQ_STATUS	0x40
#define TX2IF_STATUS		0x80

#define MSG_BUF_MASK		0xC0	/* Receive Status Masks */
#define MSG_TYPE_MASK		0x18
#define FILTER_MASK			0x07

/************************************************************************
 *	CAN SPEED
 *
 *	PropSeg + PHS1		>=	PHS2
 *	PropSeg + PHS1		>=	TDELAY
 *	PS2					>	SJW
 *  Propogation Segment =	(PRSEG + 1)  * TQ
 *  Phase Segment 1		=	(PHSEG1 + 1) * TQ
 *  Phase Segment 2		=	(PHSEG2 + 1) * TQ     [Minimum valid setting for PS2 is 2*TQ ]
 *  TQ					=	(2 * (BRP + 1) ) / F_CAN
 *  SJW					=	2 (normally)
 *  SJW					=	1 ( 1 MBPS )
 */
#define CAN_20KBPS		1
#define CAN_25KBPS		2
#define CAN_50KBPS		3
#define CAN_100KBPS		4
#define CAN_125KBPS		5
#define CAN_250KBPS		6
#define CAN_500KBPS		7
#define CAN_1MBPS		8

/************************************************************************
 *  ENUMERATIONS
 */
typedef enum 
{
	MCP2515_OK = 0,
	MCP2515_FAILED
	
} mcp2515Status;

/************************************************************************
 *	FUNCTION PROTOTYPES
 */

/* SPI Driver */
void	mcp2515_Reset			( void );
uint8_t mcp2515_Read			( const uint8_t address );
void	mcp2515_ReadRegs		( const uint8_t address, uint8_t *data, const uint8_t n );
void	mcp2515_Write			( const uint8_t address, const uint8_t data );
void	mcp2515_WriteRegs		( const uint8_t address, const uint8_t *data, const uint8_t n );
void    mcp2515_BitModify		( const uint8_t address, const uint8_t mask, const uint8_t data );
void	mcp2515_StrRXBUF		( const uint8_t buffer, uint8_t *data, const uint8_t n );
void	mcp2515_LoadTXBUF		( const uint8_t buffer, const uint8_t *data, const uint8_t n );
void    mcp2515_RTS				( const uint8_t buffer );
uint8_t	mcp2515_ReadStatus		( void );
uint8_t	mcp2515_RXStatus		( void );

/* Configuration */
mcp2515Status	mcp2515_Init		( const uint8_t can_rate );
mcp2515Status	mcp2515_ConfigRate	( const uint8_t can_rate );
mcp2515Status	mcp2515_SetMode		( const uint8_t mode );
void			mcp2515_ConfigFilt  ( const uint8_t mode );
void			mcp2515_ClrBuffers	( void );
void			mcp2515_ConfigPins  ( void );


/* Buffer Message Access */
void			mcp2515_WriteMaskFilt	( const uint8_t addr, const uint32_t m, const uint8_t ext );
void			mcp2515_ReadRxBuf		( volatile CanMessage *m, uint8_t addr );
void			mcp2515_WriteTxBuf		( const CanMessage *m, uint8_t addr );
mcp2515Status   mcp2515_ChkFreeTxBuf	( uint8_t *addr );

#endif