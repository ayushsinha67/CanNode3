#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "can.h"

/************************************************************************
 *	ENUMERATIONS
 */
typedef enum									/* Terminal State */
{
	TERM_CANINIT = 0,
	TERM_CTRLREG,
	TERM_RXSTAT,
	TERM_READSTAT,
	TERM_INTFLAG,
	TERM_ERRFLAG,
	TERM_TXBUF,
	TERM_RXBUF,
	TERM_MASK,
	TERM_FILT,
	TERM_MSGSTREAM,
	TERM_LOOPBACK,
	TERM_HELP,
	TERM_DISABLE	
	
} TERM_STATE;

typedef enum									/* Mode Configuration State */
{
	CFG_NORMAL = 0,
	CFG_LOOPBACK
	
} CFG_STATE;

typedef enum									/* Message Stream State */
{		
	MS_STREAM = 0,
	MS_DISABLE
	
} MSGSTRM_STATE;

/************************************************************************
 *	FUNCTION PROTOTYPES
 */
void		term_Main			( void );
void		term_Start			( CanStatus res );
TERM_STATE	term_GetState		( uint8_t s );
void		term_Commands		( void );
void		term_CtrlReg		( void );
void		term_ReadStatus		( void );
void		term_RxStatus		( void );
void		term_IntFlag		( void );
void		term_ErrorFlag		( void );
void		term_TxBuffer		( void );
void		term_BufTab			( uint8_t addr, uint8_t *data );
void		term_RxMsg			( CanMessage *msg );
void		term_RxBuffer		( void );
void		term_Filt			( void );
void		term_Mask			( void );

#endif