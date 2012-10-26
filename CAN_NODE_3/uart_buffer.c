#include <avr/io.h>
#include <stdlib.h>
#include "uart_buffer.h"
#include "uart.h"

/************************************************************************
 *	INITIALIZE UART BUFFER
 */
void UART_BufInit ( volatile UartBuffer *buf, uint8_t size )
{
	buf->buffer = (uint8_t *) malloc( sizeof(uint8_t) * size );
	buf->head = 0;
	buf->tail = 0;
	buf->size = size;
	buf->count = 0;
}

/************************************************************************
 *	ENQUEUE UART BUFFER
 */
void UART_BufEnq  ( volatile UartBuffer *buf, volatile uint8_t msg )
{
	buf->buffer[ buf->tail ] = msg;
	buf->count++;

	if( buf->tail == ( buf->size - 1 ) )						/* Loop around */
		buf->tail = 0;
	else
		buf->tail = buf->tail + 1;								/* Increment tail */
}

/************************************************************************
 *	DEQUEUE	UART BUFFER
 */
uint8_t UART_BufDeq  ( volatile UartBuffer *buf )
{
	uint8_t msg = buf->buffer[ buf->head ];						/* Copy from Head */
	buf->count--;

	if( buf->head == ( buf->size - 1 ) )						/* Loop around */
		buf->head = 0;
	else
		buf->head = buf->head + 1;								/* Increment head */
	
	return msg;
}

/************************************************************************
 *	UART BUFFER STATE
 */
UartBufferState UART_BufState( volatile UartBuffer *buf  )
{
	if( buf->count == buf->size )								/* Check FULL */
		return UART_BUFFER_FULL;
		
	else if ( buf->count == 0 )									/* Check EMPTY */
		return UART_BUFFER_EMPTY;
		
	else
		return UART_BUFFER_MID;
}