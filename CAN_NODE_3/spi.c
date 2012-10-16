#include <avr/io.h>
#include "spi.h"

/************************************************************************
 *	SPI INITIALIZE
 */
void SPI_Init(void)
{
	SPI_DDR = (1<<MOSI) | (1<<SCK) | (1<<CAN_SS);					// Set MOSI , SCK , and SS output
    SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0) | (1<<SPR1) ;									// Enable SPI, Master, set clock rate F_CPU/4, MODE 0	
}

/************************************************************************
 *	SPI READ
 */
uint8_t SPI_Read(void)
{
    SPDR = 0xFF;				 // Transmit dummy byte
	while(!(SPSR & (1<<SPIF)));  // Wait for receive complete
    return SPDR;                 // Return Data
}

/************************************************************************
 *	SPI WRITE
 */
void SPI_Write( uint8_t DataOut)
{
    SPDR = DataOut;              // Load Value
    while(!(SPSR & (1<<SPIF)));  // Wait for transmission complete
}