#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>
#include "pneumatic.h"
#include "can.h"
#include "init.h"

/************************************************************************
 *	GLOBAL VARIABLES
 */
/* Can Message */
extern CanMessage PneumaticOnline;

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
