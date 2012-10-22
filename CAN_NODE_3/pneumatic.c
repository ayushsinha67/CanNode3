#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>
#include "pneumatic.h"
#include "can.h"
#include "buffer.h"
#include "init.h"

#if ( TERMINAL == 1 )
#include "uart.h"
#include "terminal.h"
#endif

/************************************************************************
 *	GLOBAL VARIABLES
 */
/* Can Message */
extern CanMessage PneumaticShift, PneumaticOnline;

/* Buffer */
extern volatile CanBuffer  RxBuffer;
extern volatile CanBuffer  TxBuffer;

/* Pneumatic */
extern PneumaticState Pneum_State;
extern volatile uint16_t solenoid_ticker;

/************************************************************************
 *	CHECK SHIFT SIGNAL STATE
 */
void Pneumatic_ChkState ( void )
{
	uint16_t solenoid_ticker_cpy;
	
	ATOMIC_BLOCK( ATOMIC_RESTORESTATE ){
		solenoid_ticker_cpy = solenoid_ticker;
	}
	
	switch ( Pneum_State ){													 /* Shift State */	
	
		case PNEUM_PRESSED_UP	:	if( solenoid_ticker_cpy == 0 ){			 /* Check for TimeOut */
										CLR( PORT_SHIFT, 1<<PIN_SHIFT_UP );
										Pneum_State = PNEUM_IDLE;
									}																	
									break;
	
		case PNEUM_PRESSED_DOWN :	if( solenoid_ticker_cpy == 0 ){
										CLR( PORT_SHIFT, 1<<PIN_SHIFT_DOWN );
										Pneum_State = PNEUM_IDLE;
									}									
									break;
									
						default :	break;
	}
	
	if( CHK( PIN_READ_ONLINE_IND, 1<<PIN_ONLINE_IND ) )						/* On line Indicator State */
		PneumaticOnline.data[0] = PNEUM_ONLINE;
	else
		PneumaticOnline.data[0] = PNEUM_OFFLINE;
	
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
		SET( PORT_SHIFT, 1<<PIN_SHIFT_UP );									/* Opposite Solenoid Command */
		Pneum_State = PNEUM_PRESSED_UP;								
		solenoid_ticker = SHIFT_TIMEOUT;
	}	
	
	/* DOWNSHIFT */
	else if ( ( m->data[0] == PNEUM_MSG_DOWNSHIFT ) && ( Pneum_State == PNEUM_IDLE ) ){					

#if ( TERMINAL == 1 )
		UART_TxStr_p( PSTR("DOWN\n") );	
#endif							
		SET( PORT_SHIFT, 1<<PIN_SHIFT_DOWN );								/* Opposite Solenoid Command */
		Pneum_State = PNEUM_PRESSED_DOWN;		
		solenoid_ticker = SHIFT_TIMEOUT;					
	}	
	
	/* Parse to LCD */
}
