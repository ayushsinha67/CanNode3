#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <avr/io.h>
#include "can.h"

/************************************************************************
 *	DEFINES
 */
/* Message Sending Rate (ms) */				
#define RATE_MSG1				1
#define RATE_MSG2				50
#define RATE_MSG3				300
#define RATE_MSG4				400
#define RATE_MSG5				100
#define RATE_MSG6				100
#define RATE_MSG7				100

/************************************************************************
 *	FUNCTION PROTOTYPES
 */
void Msg_Init			( void );
void Msg_Chk			( CanMessage *msg );
void Pneumatic_RecMsg	( CanMessage *m );

#endif /* MESSAGE_H_ */