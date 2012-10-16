#ifndef TIMER_H_
#define TIMER_H_

#include <avr/io.h>

/************************************************************************
 *	DEFINES
 */
#define TX_TIMEOUT 500	/* In millisecond */

/************************************************************************
 *	FUNCTION PROTOTYPES
 */
void		Timer_Init  (void);
void		SetTimer	(uint16_t t);
uint16_t	ChkTimer	(void);

#endif 