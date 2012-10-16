#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "timer.h"
#include "lcd_hd44780.h"

/************************************************************************
 *	GLOBAL VARIABLES
 */
volatile uint16_t ms_tick = 0;
volatile uint16_t p = 0;

/************************************************************************
 *	INITIALIZE TIMER
 *  
 *  TIMER0 - CTC MODE
 *  F_CPU: 16Mhz
 *  PS: 64
 *  Frequency: 1 KHz
 *  Period: 1 ms 
 */
void Timer_Init(void)
{
    TCCR0 |= (1<<WGM01) | (1<<CS01) | (1<<CS00);    // CTC, PS: 64 
    TIMSK |= (1<<OCIE0);                            // Enable Interrupt
    OCR0 = 250;
}

/************************************************************************
 *	CHECK TIMER 
 */
uint16_t ChkTimer	(void)
{
	uint8_t tick_copy;

	ATOMIC_BLOCK( ATOMIC_FORCEON ){
		tick_copy = ms_tick;
	}
	return tick_copy;
}

/************************************************************************
 *	SET TIMER
 */
void SetTimer (uint16_t t)
{
	ATOMIC_BLOCK( ATOMIC_FORCEON ){
		ms_tick = t;
	}
}

/************************************************************************
 *	TIMER INTERRUPT 
 */ 
 ISR(TIMER0_COMP_vect)
{
	if( ms_tick > 0 )	
		ms_tick--;
}
