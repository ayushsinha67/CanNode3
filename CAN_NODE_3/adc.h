
#ifndef _ADC_H
#define _ADC_H

/************************************************************************
 *	FUNCTION PROTOTYPES
 */
void		ADC_Init		( void );							/* Initialize ADC */
void		ADC_Disable		( void );							/* Disable ADC */
uint16_t	ADC_Read		( uint8_t ch );                     /* Read ADC Channel */

#endif
