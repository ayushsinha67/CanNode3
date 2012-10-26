#ifndef UART_BUFFER_H_
#define UART_BUFFER_H_

#include <avr/io.h>
#include "uart.h"

/************************************************************************
 *	DEFINES
 */
#define UART_RX_BUFFER_SIZE		20	

/************************************************************************
 *	DATA TYPES
 */
typedef struct
{
	uint8_t 	*buffer;
	uint8_t		size;
	uint8_t     head;
	uint8_t     tail;
	uint8_t		count;

} UartBuffer;

/************************************************************************
 *	ENUMERATIONS
 */
typedef enum 
{
	UART_BUFFER_FULL = 0,
	UART_BUFFER_MID,
	UART_BUFFER_EMPTY
	
} UartBufferState;

/************************************************************************
 *	FUNCTIONS
 */
void		    UART_BufInit ( volatile UartBuffer *buf, uint8_t size );
void    		UART_BufEnq  ( volatile UartBuffer *buf, volatile uint8_t msg );
uint8_t	        UART_BufDeq  ( volatile UartBuffer *buf );
UartBufferState UART_BufState( volatile UartBuffer *buf  );

#endif /* UART_BUFFER_H_ */