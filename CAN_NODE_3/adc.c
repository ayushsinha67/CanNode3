#include <avr/io.h>
#include "adc.h"

/************************************************************************
 *	INITIALIZE ADC
 */
void ADC_Init(void)
{
    /*
        10 Bit ADC
        Prescalar  = 8
        ADC Freq: 1000000/8 = 125000 Hz
        AREF = AVcc
    */
    ADMUX  = ( 1 << REFS0 );
    ADCSRA = ( 1 << ADEN ) | ( 1 << ADPS1) | ( 1 << ADPS0 );
}

/************************************************************************
 *	READ CHANNEL
 */

uint16_t ADC_Read( uint8_t ch)
{
   ADMUX   = ( ( ADMUX & 0xE0 ) | ( ch & 0x07 ) );		/* Select ADC Channel ch must be 0-7 */
   ADCSRA |= ( 1 << ADSC );								/* Start Single conversion */
   
   while( !( ADCSRA & ( 1 << ADIF ) ) );				/* Wait for conversion to complete */
   
   ADCSRA |= ( 1 << ADIF );								/* Clear ADIF by writing one to it */

   return ADC;
}

/************************************************************************
 *	ADC DISABLE
 */

void ADC_Disable ( void )
{
	ADCSRA = 0x00;
	ADMUX  = 0x00;
}
