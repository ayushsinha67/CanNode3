#ifndef PNEUMATIC_H_
#define PNEUMATIC_H_

#include "can.h"

/************************************************************************
 *	DEFINES
 */
/* Solenoid Pulse Time */
#define SHIFT_TIMEOUT		500

/* Output Signal to Pneumatic Controller */
#define DDR_SHIFT			DDRA
#define PORT_SHIFT			PORTA
#define PIN_SHIFT_UP		PA0
#define PIN_SHIFT_DOWN		PA1

/* System ONLINE Indicator - Input */
#define DDR_ONLINE_IND		DDRA
#define PIN_READ_ONLINE_IND	PINA
#define PORT_ONLINE_IND		PORTA
#define PIN_ONLINE_IND		PA2

/************************************************************************
 *	ENUMERATIONS
 */
typedef enum
{	
	PNEUM_IDLE = 0,
	PNEUM_PRESSED_UP,
	PNEUM_PRESSED_DOWN,
				
} PneumaticState;

/************************************************************************
 *	FUNCTIONS
 */
void Pneumatic_ChkState ( void );
void Pneumatic_RecMsg	( CanMessage *m );

#endif /* PNEUMATIC_H_ */