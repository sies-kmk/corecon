/*******************************************************************************
 * File Name	: macro.h
 * Project	: Keyboard
 * Date		: 2014/03/17
 * Version	: 1.0
 * Target MCU	: AVR
 * Tool Chain	: Atmel AVR Studio 4.19 730, avr-gcc, avr-libc
 * Author	: "Detlef Mueller" <detlef@gmail.com>
 * Release Notes:
 *
 * $Id$
 ******************************************************************************/

#ifndef	__macro_h__
#define	__macro_h__

//------------------------------------------------------------------------------

#define	M_ACTIVE		0x80		/* Macro active flag */

#define REP_KB			0x0000		/* Keyboard */
#define REP_M			0x1000		/* Macro */
#define REP_SC			0x2000		/* System Control */
#define REP_CC			0x4000		/* Consumer Control */
#define	REP_DEL			0x8000		/* Delay */

#define uKB( n )		(n)
#define uM( n )			(REP_M   | (n))
#define	uSC( n )		(REP_SC  | (n))
#define	uCC( n )		(REP_CC  | (n))
#define	uDms( n )		(REP_DEL | (n * 2))

//------------------------------------------------------------------------------

extern uint8_t
    play_macro( uint8_t idx ) ;

//------------------------------------------------------------------------------

#endif // __macro_h__
