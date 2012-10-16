#ifndef _SPI_H
#define _SPI_H

#include <avr/io.h>

/************************************************************************
 *	CONSTANTS AND DEFINES
 */
#define SPI_DDR  DDRB
#define SPI_PORT PORTB
#define CAN_SS   PB4
#define MOSI     PB5
#define MISO     PB6
#define SCK      PB7

#define MCP2515_SELECT()	SPI_PORT &= ~(1<<CAN_SS)		// CAN ASSERT
#define	MCP2515_DESELECT()	SPI_PORT |=  (1<<CAN_SS)		// CAN DEASSERT

/************************************************************************
 *	FUNCTION PROTOTYPES
 */
void	SPI_Init  (void);
uint8_t SPI_Read  (void);
void	SPI_Write (uint8_t DataOut );

#endif

