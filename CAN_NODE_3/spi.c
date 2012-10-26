#include <avr/io.h>
#include "spi.h"

/************************************************************************
 *	SPI INITIALIZE
 *
 * MODE		: 0
 * SPI_CLK	: F_CPU/2 = 8 MHz
 */
void SPI_Init( void )
{
	SPI_DDR = ( 1<<MOSI ) | ( 1<<SCK ) | ( 1<<CAN_SS );			/* Set MOSI , SCK , and SS output */
    SPCR = ( 1<<SPE ) | ( 1<<MSTR );							/* Enable SPI, Master, Set SPI_CLK  = F_CPU/4, MODE 0 */
	SPSR = ( 1<<SPI2X );										/* Clock doubling SPI_CLK = F_CPU/2 */
}

/************************************************************************
 *	SPI READ
 */
uint8_t SPI_Read( void )
{
    SPDR = 0xFF;												/* Transmit dummy byte */
	while ( !( SPSR & ( 1<<SPIF ) ) );							/* Wait for receive complete */
    return SPDR;												/* Return Data */
}

/************************************************************************
 *	SPI WRITE
 */
void SPI_Write( uint8_t DataOut)
{
    SPDR = DataOut;												/* Load Value */
    while( !( SPSR & ( 1<<SPIF ) ) );							/* Wait for transmission complete */
}