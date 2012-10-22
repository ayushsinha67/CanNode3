#include <avr/io.h>
#include <avr/interrupt.h>
#include "init.h"
#include "pneumatic.h"

/************************************************************************
 *	INITIALIZE GPIO
 */
void GPIO_Init (void)
{
	/* Shift Signals OUT */
	DDR_SHIFT |= ( 1<<PIN_SHIFT_UP ) | ( 1<<PIN_SHIFT_DOWN );
	
	/* Pneumatic Indicator IN */
	DDR_ONLINE_IND &= ~( 1<<PIN_ONLINE_IND );
	
	CLR( PORT_SHIFT, 1<<PIN_SHIFT_UP );
	CLR( PORT_SHIFT, 1<<PIN_SHIFT_DOWN );					/* Keep output low initially*/
}

/************************************************************************
 *	INITIALZE EXTERNAL INTERRUPT
 *
 * Trigger INT0 on Low Level
 * INT1 and INT2 on rising edge
 */
void ExtINT_Init(void)
{	
	MCUCR = ( ( 1<<ISC10 ) | ( 1<<ISC11) );					/* INT0 - low level; INT1 - rising edge */
	MCUCSR = ( 1<<ISC2 );									/* INT2 - rising edge */ 	
	GICR = ( ( 1<<INT0 ) | ( 1<<INT1 ) | ( 1<<INT2 ) );		/* Enable interrupts */						
}

/************************************************************************
 *	INITIALIZE TIMER
 *  
 *  TIMER0 - CTC MODE, F_CPU: 16Mhz, PS: 64, Frequency: 1 KHz, Period: 1 ms 
 *  TIMER2 - CTC MODE, F_CPU: 16Mhz, PS: 64, Frequency: 1 KHz, Period: 1 ms 
 */
void Timer_Init(void)
{
    /* TIMER0 */
	TCCR0 |= (1<<WGM01) | (1<<CS01) | (1<<CS00);			/* CTC, PS: 64 */
    TIMSK |= (1<<OCIE0);									/* Enable Interrupt */
    OCR0 = 249;
	
	/* TIMER2 */
	TCCR2 |= ( 1<<WGM21 ) | ( 1<<CS22 );					/* CTC, PS: 64 */
    TIMSK |= ( 1<<OCIE2 );									/* Enable Interrupt */
    OCR2 = 249;
}