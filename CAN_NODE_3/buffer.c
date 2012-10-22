#include <avr/io.h>
#include <stdlib.h>
#include "buffer.h"

/************************************************************************
 *	BUFFER INITIALIZATION
 */
void CAN_BufInit ( volatile CanBuffer *buf, uint8_t size )
{
	buf->buffer = (CanMessage*) malloc( sizeof(CanMessage) * size );
	buf->head = 0;
	buf->tail = 0;
	buf->size = size;
	buf->count = 0;
}

/************************************************************************
 *	BUFFER ENQUEUE
 */
void CAN_BufEnq ( volatile CanBuffer *buf, volatile CanMessage *msg )
{
    buf->buffer[ buf->tail ] = *msg;
    buf->count++;

    if( buf->tail == ( buf->size - 1 ) )						/* Loop around */
     	buf->tail = 0;
    else
		buf->tail = buf->tail + 1;								/* Increment tail */
}


/************************************************************************
 *	BUFFER DEQUEUE
 */
void CAN_BufDeq ( volatile CanBuffer *buf, volatile CanMessage *msg )
{
	*msg = buf->buffer[ buf->head ];							/* Copy from Head */
	buf->count--;

	if( buf->head == ( buf->size -1 ) )							/* Loop around */
		buf->head = 0;
	else
		buf->head = buf->head + 1;								/* Increment head */
}

/************************************************************************
 *	BUFFER STATE 
 */
BufferState CAN_BufState( volatile CanBuffer *buf )
{
    if( buf->count == buf->size )								/* Check FULL */
        return BUFFER_FULL;

    else if ( buf->count == 0 )									/* Check EMPTY */
        return BUFFER_EMPTY;

    else
        return BUFFER_MID;
}
