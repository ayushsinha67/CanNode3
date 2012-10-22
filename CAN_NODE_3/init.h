#ifndef _INIT_H_
#define _INIT_H_

/************************************************************************
 *	DEFINES
 */
/* Bit Manipulation */
#define CHK(x,b) (x&b)
#define CLR(x,b) (x&=~b)
#define SET(x,b) (x|=b)
#define TOG(a,b) (a^=b)

/* Terminal Debugging */
#define TERMINAL		1	

/* Timer */
#define TX_TIMEOUT		100						

/************************************************************************
 *	FUNCTIONS
 */
void GPIO_Init	 (void);
void ExtINT_Init (void);
void Timer_Init  (void);

#endif 