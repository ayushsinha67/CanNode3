#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "global.h"
#include "terminal.h"
#include "can.h"
#include "mcp2515.h"
#include "lcd_hd44780.h"
#include "uart.h"


/************************************************************************
 *	GLOBAL VARIABLES
 */
extern CanMessage gps, brake, pressure;
extern volatile CanMessage temp;
extern volatile enum TERM_STATE		state;
extern volatile enum CFG_STATE		cfg;
extern volatile enum MSGSTRM_STATE	strm; 

/************************************************************************
 *	INITIALIZE MESSAGE TO BE SENT
 */
void Msg_Init(void)
{
	gps.ext = 0;
	gps.dlc = 8;
	gps.id  = 0x07;
	gps.data[0] = 0x12;
	gps.data[1] = 0xAB;
	gps.data[2] = 0x34;
	gps.data[3] = 0x23;
	gps.data[4] = 0xDF;
	gps.data[5] = 0x49;
	gps.data[6] = 0x45;
	gps.data[7] = 0x33;
	
	brake.ext = 0;
	brake.dlc = 8;
	brake.id = 0x0C;
	brake.data[0] = 0x65;
	brake.data[1] = 0x91;
	brake.data[2] = 0x44;
	brake.data[3] = 0x52;
	brake.data[4] = 0xFA;
	brake.data[5] = 0xCE;
	brake.data[6] = 0x1C;
	brake.data[7] = 0xBA;
	
	pressure.ext = 0;
	pressure.dlc = 8;
	pressure.id = 0x08;
	pressure.data[0] = 0x00;
	pressure.data[1] = 0x00;
	pressure.data[2] = 0x00;
	pressure.data[3] = 0x00;
	pressure.data[4] = 0x00;
	pressure.data[5] = 0x00;
	pressure.data[6] = 0x00;
	pressure.data[7] = 0x00;
	

}
/************************************************************************
 *	EXTERNAL INTERRUPT
 */

ISR(INT0_vect)
{
	if( !CAN_ReadMsg(&temp) ){
			
		if( strm == MS_STREAM ) 
			term_RxMsg(&temp);
	}
}


/************************************************************************
 *	INITIALZE EXTERNAL INTERRUPT
 */
void ExtINT_Init(void)
{	
	GICR = (1 << INT0); 
	MCUCR = 0x00;		/*Trigger INT0 on Low */
}

/************************************************************************
 *	RECEIVE INTERRUPT
 */
ISR(USART_RXC_vect)
{
	uint8_t data = UDR; 
	
	switch( data ) {
		
		case 'c': state = TERM_CTRLREG;  break;
		case 'x': state = TERM_RXSTAT;	 break;
		case 's': state = TERM_READSTAT; break;
		case 'i': state = TERM_INTFLAG;	 break;
		case 'e': state = TERM_ERRFLAG;	 break;
		case 't': state = TERM_TXBUF;	 break;
		case 'r': state = TERM_RXBUF;	 break;
		case 'q': state = TERM_MSGSTREAM;break;
		case 'l': state = TERM_LOOPBACK; break;
		case 'h': state = TERM_HELP;	 break; 	
		default : state = TERM_DISABLE;	 break;	
		}		
}

/************************************************************************
 *	WATCHDOG TIMER
 * 
 * Disable interrupts during WDT Operations
 * wdt_reset() 
 * wdt_disable()
 * wdt_enable(value)
	  WDTO_15MS 
	  WDTO_30MS 
	  WDTO_60MS 
	  WDTO_120MS 
	  WDTO_250MS 
	  WDTO_500MS 
	  WDTO_1S 
	  WDTO_2S 
 */
void WDT_Init( void )
{	
	uint8_t reset = MCUCSR;						/* Read MCU Control and Status Register */
	
	if( (reset & 0x08) == 0x08)					/* Check WDRF Flag */
		UART_TxChar('w');	
	
	MCUCSR = 0x00;								/* Reinitialize Reset Register */
}
