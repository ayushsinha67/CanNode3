#include <avr/io.h>
#include <avr/pgmspace.h>
#include "message.h"
#include "messagedef.h"
#include "mcp2515.h"
#include "can.h"
#include "can_buffer.h"
#include "init.h"
#include "adc.h"
#include "pneumatic.h"

#if ( TERMINAL == 1 )
#include "uart.h"
#include "terminal.h"
#endif


/************************************************************************
 *	GLOBAL VARIABLES
 */
/* CAN Messages */
extern CanMessage PneumaticShift, PneumaticOnline;

/* Buffer */
extern volatile CanBuffer  RxBuffer;
extern volatile CanBuffer  TxBuffer;

/* Pneumatic */
extern PneumaticState Pneum_State;
extern volatile uint16_t solenoid_ticker;


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

/************************************************************************
 *	RECEIVE PNEUMATIC MESSAGE
 */
void Pneumatic_RecMsg( CanMessage *m )
{
	/* UPSHIFT */
	if( ( m->data[0] == PNEUM_MSG_UPSHIFT ) && ( Pneum_State == PNEUM_IDLE) ){							

#if ( TERMINAL == 1 )
		UART_TxStr_p( PSTR("UP\n") );
#endif	
		SET( PORT_SHIFT, 1<<PIN_SHIFT_UP );									/* Up shift Command */
		Pneum_State = PNEUM_PRESSED_UP;								
		solenoid_ticker = SHIFT_TIMEOUT;
	}	
	
	/* DOWNSHIFT */
	else if ( ( m->data[0] == PNEUM_MSG_DOWNSHIFT ) && ( Pneum_State == PNEUM_IDLE ) ){					

#if ( TERMINAL == 1 )
		UART_TxStr_p( PSTR("DOWN\n") );	
#endif							
		SET( PORT_SHIFT, 1<<PIN_SHIFT_DOWN );								/* Down shift Command */
		Pneum_State = PNEUM_PRESSED_DOWN;		
		solenoid_ticker = SHIFT_TIMEOUT;					
	}	
	
	/* Parse to LCD */
}



