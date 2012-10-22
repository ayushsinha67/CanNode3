#include <avr/io.h>
#include "message.h"
#include "messagedef.h"
#include "mcp2515.h"
#include "can.h"
#include "init.h"
#include "adc.h"
#include "buffer.h"
#include "pneumatic.h"

/************************************************************************
 *	GLOBAL VARIABLES
 */
/* CAN Messages */
extern CanMessage PneumaticShift, PneumaticOnline;

/* Buffer */
extern volatile CanBuffer  RxBuffer;
extern volatile CanBuffer  TxBuffer;

/************************************************************************
 *	INITIALIZE MESSAGE TO BE TRANSMITTED
 */
void Msg_Init( void )
{
	/* Pneumatic Gear Shift */
	PneumaticShift.ext = CAN_MODE;
	PneumaticShift.id  = PNEUM_SHIFT_ID;
	PneumaticShift.dlc = PNEUM_SHIFT_DLC;
	PneumaticShift.data[0] = 127;
	
	/* Pneumatic On line Indicator */
	PneumaticOnline.ext = CAN_MODE;
	PneumaticOnline.id = PNEUM_ONLINE_ID;
	PneumaticOnline.dlc = PNEUM_ONLINE_DLC;
	PneumaticOnline.data[0] = 127;
}

/************************************************************************
 *	CHECK MESSAGES RECEIVED
 */
void Msg_Chk( CanMessage *msg )
{
	switch( msg->id ){
		
		case PNEUM_SHIFT_ID		: Pneumatic_RecMsg( msg ); break;
						default : break;
			  
	}
}


