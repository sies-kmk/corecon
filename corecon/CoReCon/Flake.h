#if ! defined(__Flake_h__) && defined(FrostyFlake)
/*******************************************************************************
 * File Name	: Flake.h
 * Project	: Costar Replacement Controllers
 * Date		: 2014/05/22
 * Version	: 1.0
 * Target MCU   : ATMEGA32U2
 * Tool Chain	: Atmel AVR Studio 4.19 730, avr-gcc, avr-libc
 * Author	: "Detlef Mueller" <detlef@gmail.com>
 * Device	: The Frosty Flake
 *
 * Release Notes:
 *
 * Pinout ATmega32U2
 * =================
 *
 * PB0 --               PC0 (/XTAL2)            PD0 Col 1       i
 * PB1 R A       o      PC1 (/RESET)            PD1 Col 2       i
 * PB2 R B       o      PC2 Col 0      i        PD2 Col 6       i
 * PB3 R C       o                              PD3 --
 * PB4 R D       o      PC4 --                  PD4 Col 5       i
 * PB5 R E       o      PC5 WIN LED    o        PD5 Col 4       i
 * PB6 R F       o      PC6 SCR LED    o        PD6 Col 7       i
 * PB7 CAPS LED  o      PC7 Col 3      i        PD7 HWB     
 *
 * $Id$
 ******************************************************************************/

#define __Flake_h__

//------------------------------------------------------------------------------
// Port Setup

// Port B

#define PBPU		(_B1( PB7) | _B1( PB6) | _B1( PB5) | _B1( PB4) | \
			 _B1( PB3) | _B1( PB2) | _B1( PB1) | PU1( PB0))
#define DDB		(DDO(DDB7) | DDO(DDB6) | DDO(DDB5) | DDO(DDB4) | \
			 DDO(DDB3) | DDO(DDB2) | DDO(DDB1) | DDI(DDB0))
// Port C

#define PCPU		(PU1( PC7) | _B1( PC6) | _B1( PC5) | PU1( PC4) | \
			             PU1( PC2) | PU1( PC1) | PU0( PC0))
#define DDC		(DDI(DDC7) | DDO(DDC6) | DDO(DDC5) | DDI(DDC4) | \
			             DDI(DDC2) | DDI(DDC1) | DDI(DDC0))
// Port D

#define PDPU		(PU0( PD7) | PU1( PD6) | PU1( PD5) | PU1( PD4) | \
			 PU1( PD3) | PU1( PD2) | PU1( PD1) | PU1( PD0))
#define DDD		(DDI(DDD7) | DDI(DDD6) | DDI(DDD5) | DDI(DDD4) | \
			 DDI(DDD3) | DDI(DDD2) | DDI(DDD1) | DDI(DDD0))

//------------------------------------------------------------------------------
// Row and Column Setup

#define KEYS			88	/* 88 keys ISO */

#define	pROW			PORTB	/* Port that connects to U2/U3 */

#define	bR_A			PB1	/* A U2/U3 */
#define	bR_B			PB2	/* B U2/U3 */
#define	bR_C			PB3	/* C U2 */
#define	bR_D			PB4	/* D U2 */
#define	bR_E			PB5	/* C U3 */
#define	bR_F			PB6	/* D U3 */

#define mPB7			_B1(PB7) /* LED, disconnected by PWM */
#define mPB0			PU1(PB0) /* N/C */

#define mR(r)			((r) | mPB0 | mPB7)

//------------------------------------------------------------------------------

// Bit patterns adressing rows A-R

#define	ROW_A			mR(mR15)
#define	ROW_B			mR(mR18)
#define	ROW_C			mR(mR08)
#define	ROW_D			mR(mR07)
#define	ROW_E			mR(mR06)
#define	ROW_F			mR(mR05)
#define	ROW_G			mR(mR04)
#define	ROW_H			mR(mR02)
#define	ROW_I			mR(mR00)
#define	ROW_J			mR(mR01)
#define	ROW_K			mR(mR11)
#define	ROW_L			mR(mR10)
#define	ROW_M			mR(mR16)
#define	ROW_N			mR(mR03)
#define	ROW_O			mR(mR14)
#define	ROW_P			mR(mR09)
#define	ROW_Q			mR(mR13)
#define	ROW_R			mR(mR17)

//------------------------------------------------------------------------------

// Col info, for reference only.

#define	COL_0			{ &PORTC, _BV(PC2) }
#define	COL_1			{ &PORTD, _BV(PD0) }
#define	COL_2			{ &PORTD, _BV(PD1) }
#define	COL_3			{ &PORTC, _BV(PC7) }
#define	COL_4			{ &PORTD, _BV(PD5) }
#define	COL_5			{ &PORTD, _BV(PD4) }
#define	COL_6			{ &PORTD, _BV(PD2) }
#define	COL_7			{ &PORTD, _BV(PD6) }

#define	pCOL1			PINC
#define	pCOL2			PIND

//------------------------------------------------------------------------------
// Spacebar Row and Column Definition (K6)

#define	bCOL6			PD2	/* Space bar column */

#define	ROW_SPC			ROW_K
#define	pCOL_SPC		pCOL2
#define bCOL_SPC		bCOL6

//------------------------------------------------------------------------------
// LED Configuration

// We have LED's 0,1,2. No LED 3

#define	LED_0
#define	LED_1
#define	LED_2

// Map LED's

#define	bLED_SCR		bLED_0
#define	bLED_WIN		bLED_1
#define	bLED_CAP		bLED_2

#define	pLED_SCR		pLED_0
#define	pLED_WIN		pLED_1
#define	pLED_CAP		pLED_2

#define	mLED_SCR		_BV(bLED_SCR)
#define	mLED_WIN		_BV(bLED_WIN)
#define	mLED_CAP		_BV(bLED_CAP)

#define	mLEDS			(mLED_SCR | mLED_WIN | mLED_CAP)

#define	bLED_SLP		bLED_WIN
#define	pLED_SLP		pLED_WIN

#if defined(_QF_3)
 #undef  LED_LVL_DEF
 #define LED_LVL_DEF		40
#endif

//------------------------------------------------------------------------------
// Demo Core

#define	__BR_IDX( n )		pgm_read_byte( breathe + ((idx + (n)) % ARRSZ( breathe )) )

#define demo_leds()		__WRAP__(			\
	{							\
	    if ( fLock )					\
		set_leds_raw( mLED_WIN, pgm_read_byte( breathe + idx ) ) ;	\
	    else						\
	    if ( dir )						\
	    {							\
		set_leds_raw( mLED_CAP, pgm_read_byte( breathe + idx ) ) ;	\
		set_leds_raw( mLED_WIN, __BR_IDX(  5 ) ) ;	\
		set_leds_raw( mLED_SCR, __BR_IDX( 10 ) ) ;	\
	    }							\
	    else						\
	    {							\
		set_leds_raw( mLED_CAP, __BR_IDX( 10 ) ) ;	\
		set_leds_raw( mLED_WIN, __BR_IDX(  5 ) ) ;	\
		set_leds_raw( mLED_SCR, pgm_read_byte( breathe + idx ) ) ;	\
	    }							\
	} )

//------------------------------------------------------------------------------
// Alarm Flasher

#define al_flash()		__WRAP__(			\
	{							\
	    if ( al_pat & 0x001 )				\
		set_leds_raw( mLED_WIN, _x(100) ) ;		\
	    else						\
	    if ( al_pat & 0x100 )				\
		set_leds_raw( mLED_CAP | mLED_SCR, _x(100) ) ;	\
	} )

//------------------------------------------------------------------------------
// Handle LED's

static void
    set_leds( uint8_t leds, uint8_t lvl ) ;

static inline uint8_t
	__FA__( do_leds, __always_inline__ ) ( void )
{
    uint8_t
	_mLeds = 0 ;

    if ( fLock )
	return ( 0 ) ;

  #if defined(_QF_3)
    //          -   LN   F1   F2
    // CAPL  CAPL NUML CAPL CAPL
    // WINL  WINL WINL WINL WINL
    // SCRL  LNML LNML SCRL NUML

    if ( fFN == 1 )
    {
	if ( kbd_leds & mLED_CAPS )
	    _mLeds |= mLED_CAP ;

	if ( kbd_leds & mLED_SCRL )
	    _mLeds |= mLED_SCR ;
    }
    else
    if ( fFN == 2 )
    {
	if ( kbd_leds & mLED_CAPS )
	    _mLeds |= mLED_CAP ;

	if ( kbd_leds & mLED_NUML )
	    _mLeds |= mLED_SCR ;
    }
    else
    if ( fNumLk )
    {
	_mLeds |= mLED_SCR ;

	if ( kbd_leds & mLED_NUML )
	    _mLeds |= mLED_CAP ;
    }
  #else
    //          -   LN   F1   F2
    // CAPL  CAPL NUML CAPL NUML
    // WINL  WINL WINL WINL WINL
    // SCRL  LNML LNML NUML SCRL

    if ( fFN == 1 )
    {
	if ( kbd_leds & mLED_CAPS )
	    _mLeds |= mLED_CAP ;

	if ( kbd_leds & mLED_NUML )
	    _mLeds |= mLED_SCR ;
    }
    else
    if ( fFN == 2 )
    {
	if ( kbd_leds & mLED_NUML )
	    _mLeds |= mLED_CAP ;

	if ( kbd_leds & mLED_SCRL )
	    _mLeds |= mLED_SCR ;
    }
    else
    if ( fNumLk )
    {
	_mLeds |= mLED_SCR ;

	if ( kbd_leds & mLED_NUML )
	    _mLeds |= mLED_CAP ;
    }
  #endif
    else
    {
	if ( kbd_leds & mLED_CAPS )
	    _mLeds |= mLED_CAP ;

	if ( kbd_leds & mLED_NUML )
	    _mLeds |= mLED_SCR ;
    }

    if ( _mLeds )
	set_leds( _mLeds, led_lvl ) ;

    if ( fWinLk || fFN )
    {
	_mLeds |= mLED_WIN ;

	set_leds( mLED_WIN,	// Superimpose FN status to WIN LED
		  (fFN ? LED_LVL_MAX - led_lvl : 0) + (fWinLk ? led_lvl : 0) ) ;
    }

    return ( _mLeds ) ;
}

//------------------------------------------------------------------------------
// Read and Sort the Column Bits

static inline uint8_t
	__FA__( rd_cbits, __always_inline__ ) ( void )
{
    uint8_t
	_cb ;

    __asm__ __volatile__
    (
	"in     %0,%1"		"\n\t"
	"bst    %0,2"		"\n\t"
	"bld    %0,4"		"\n\t"
	"swap   %0"		"\n\t"
	"andi   %0,0b00001001"	"\n\t"
	"mov    __tmp_reg__,%0"	"\n\t"
	"in     %0,%2"		"\n\t"
	"lsl    %0"		"\n\t"
	"bst    %0,6"		"\n\t"
	"bld    %0,4"		"\n\t"
	"bst    %0,3"		"\n\t"
	"bld    %0,6"		"\n\t"
	"andi   %0,0b11110110"	"\n\t"
	"or     %0,__tmp_reg__"	"\n\t"
	: "=d" (_cb)
	: "I" (_SFR_IO_ADDR(pCOL1)),
	  "I" (_SFR_IO_ADDR(pCOL2))
	: "cc"
    ) ;

    return ( _cb ) ;
}

//------------------------------------------------------------------------------

#endif	// __Flake_h__
