#ifndef GLOBAL_H
#define GLOBAL_H

#include <avr/io.h>

/************************************************************************
 *	DEFINES AND CONSTANTS
 */
#define TERMINAL	/* ENABLE DEBUGGING */

/************************************************************************
 *	GLOBAL FUNCTIONS
 */
void ExtINT_Init (void);
void Msg_Init    (void);
void WDT_Init	 (void);

#endif

