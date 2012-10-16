/*
 * CAN_NODE_3.c
 *
 * Created: 01-09-2012 AM 9:18:14
 *  Author: Admin
 */ 


#define  F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>
#include <avr/wdt.h>
#include "global.h"
#include "timer.h"
#include "uart.h"
#include "lcd_hd44780.h"
#include "spi.h"
#include "adc.h"
#include "terminal.h"
#include "uart.h"
#include "mcp2515.h"
#include "can.h"

/************************************************************************
 *	GLOBAL VARIABLES
 */
CanMessage gps, brake, pressure;
volatile CanMessage temp;
volatile enum TERM_STATE	state	= TERM_DISABLE;
volatile enum CFG_STATE		cfg		= CFG_NORMAL;
volatile enum MSGSTRM_STATE	strm	= MS_DISABLE; 
enum TERM_STATE		state_copy;
enum MSGSTRM_STATE  strm_copy;
enum MCP2515_STATUS res;

/************************************************************************
 *	MAIN
 */
int main(void)
{	
	ExtINT_Init();										/* Initializations */
	Timer_Init();	
	UART_Init();			
	SPI_Init();
	WDT_Init();	
	Init_ADC();
	sei();			
	
	res = CAN_Init(CAN_250KBPS);						/* Start CAN */				
	
#ifdef TERMINAL		
	UART_TxStr_p( PSTR("NODE MECHANIC (c) Ayush Sinha\n") );
	if( res == OK ){ UART_TxStr_p( PSTR("\nCAN Initialized\n") ); }
    else           { UART_TxStr_p( PSTR("\nCAN Initialization Failed\n") ); }
	term_Commands();
#endif
	
	Msg_Init();											/* Construct Data to be sent */

	while(1){
		
		wdt_enable(WDTO_1S);							/* Enable Watchdog Timer for 1 second */
		
		uint16_t val = ADC_read(0);
		pressure.data[0] = (uint8_t) ( val >> 8 );
		pressure.data[1] = (uint8_t) val;
		
		//CAN_SendMsg(&brake);		
		CAN_SendMsg(&pressure);
		CAN_SendMsg(&gps);
		
		
		
		
		
		
#ifdef TERMINAL											/* TERMINAL FOR DEBUGGIN */

	ATOMIC_BLOCK( ATOMIC_FORCEON ){						/* Read terminal state */
		state_copy = state;
		strm_copy  = strm;
	}
		
	switch( state_copy ) {
		
		case TERM_CTRLREG	: term_CtrlReg();    break;
		case TERM_RXSTAT	: term_RxStatus();	 break;
		case TERM_READSTAT	: term_ReadStatus(); break;
		case TERM_INTFLAG	: term_IntFlag();	 break;
		case TERM_ERRFLAG	: term_ErrorFlag();	 break;
		case TERM_TXBUF		: term_TxBuffer();	 break;
		case TERM_RXBUF		: term_RxBuffer();	 break;
		case TERM_MSGSTREAM	: strm_copy = (strm_copy == MS_DISABLE) ? MS_STREAM : MS_DISABLE;	 
						      break;
		case TERM_LOOPBACK  : if( cfg == CFG_NORMAL ){ 
									mcp2515_SetMode( MODE_LOOPBACK ); 
									cfg = CFG_LOOPBACK;	
							  }
							  else { 
									mcp2515_SetMode( MODE_NORMAL );
									 cfg = CFG_NORMAL; 
							  } 
							  break;
		case TERM_HELP		: term_Commands();   break;	
					default	: break;
		}	
																				
	ATOMIC_BLOCK( ATOMIC_FORCEON ){							/* Copy back */
		state = TERM_DISABLE;
		strm  = strm_copy;
	}	
		
#endif
		wdt_reset();										/* Reset if Execution time < Watchdog time */
		_delay_ms(100);										/* DELAY */
    }
	return 0;
}