#if ! defined(__Flake2_h__) && defined(FrostyFlake2)
/*******************************************************************************
 * File Name	: Flake2.h
 * Project	: Costar Replacement Controllers
 * Date		: 2014/05/24
 * Version	: 1.0
 * Target MCU   : ATMEGA32U2
 * Tool Chain	: Atmel AVR Studio 4.19 730, avr-gcc, avr-libc
 * Author	: "Detlef Mueller" <detlef@gmail.com>
 * Device	: The Frosty Flake 2
 *
 * Release Notes:
 *
 * Pinout ATmega32U2
 * =================
 *
 * PB0 Col 7     i      PC0 (/XTAL2)            PD0 R C         o
 * PB1 Col 4     i      PC1 (/RESET)            PD1 R D         o
 * PB2 Col 5     i      PC2 --                  PD2 --
 * PB3 Col 6     i                              PD3 R A         o
 * PB4 Col 2     i      PC4 --                  PD4 R B         o
 * PB5 Col 1     i      PC5 CAP LED    o        PD5 R E         o
 * PB6 Col 3     i      PC6 SCR LED    o        PD6 R F         o
 * PB7 WIN LED   o      PC7 Col 0      i        PD7 HWB     
 *
 * $Id$
 ******************************************************************************/

#define __Flake2_h__

//------------------------------------------------------------------------------
// Port Setup

// Port B

#define PBPU		(_B1( PB7) | PU1( PB6) | PU1( PB5) | PU1( PB4) | \
			 PU1( PB3) | PU1( PB2) | PU1( PB1) | PU1( PB0))
#define DDB		(DDO(DDB7) | DDI(DDB6) | DDI(DDB5) | DDI(DDB4) | \
			 DDI(DDB3) | DDI(DDB2) | DDI(DDB1) | DDI(DDB0))
// Port C

#define PCPU		(PU1( PC7) | _B1( PC6) | _B1( PC5) | PU1( PC4) | \
			             PU1( PC2) | PU1( PC1) | PU0( PC0))
#define DDC		(DDI(DDC7) | DDO(DDC6) | DDO(DDC5) | DDI(DDC4) | \
			             DDI(DDC2) | DDI(DDC1) | DDI(DDC0))
// Port D

#define PDPU		(PU0( PD7) | _B1( PD6) | _B1( PD5) | _B1( PD4) | \
			 _B1( PD3) | PU1( PD2) | _B1( PD1) | _B1( PD0))
#define DDD		(DDI(DDD7) | DDO(DDD6) | DDO(DDD5) | DDO(DDD4) | \
			 DDO(DDD3) | DDI(DDD2) | DDO(DDD1) | DDO(DDD0))

//------------------------------------------------------------------------------
// Row and Column Setup

#define KEYS			88

#define	pROW			PORTD

#define	bR_A			PD3
#define	bR_B			PD4
#define	bR_C			PD0
#define	bR_D			PD1
#define	bR_E			PD5
#define	bR_F			PD6

#define	mPD7			PU0(PD7)	/* HWB */
#define	mPD2			PU1(PD2)

#define mR(r)			((r) | mPD2 | mPD7)

//------------------------------------------------------------------------------

// Bit patterns adressing rows A-R

#define	ROW_A			mR(mR13)
#define	ROW_B			mR(mR18)
#define	ROW_C			mR(mR09)
#define	ROW_D			mR(mR07)
#define	ROW_E			mR(mR08)
#define	ROW_F			mR(mR06)
#define	ROW_G			mR(mR04)
#define	ROW_H			mR(mR02)
#define	ROW_I			mR(mR00)
#define	ROW_J			mR(mR01)
#define	ROW_K			mR(mR15)
#define	ROW_L			mR(mR16)
#define	ROW_M			mR(mR14)
#define	ROW_N			mR(mR03)
#define	ROW_O			mR(mR12)
#define	ROW_P			mR(mR05)
#define	ROW_Q			mR(mR11)
#define	ROW_R			mR(mR17)

//------------------------------------------------------------------------------

// Col info, for reference only.

#define	COL_0			{ &PORTC, _BV(PC7) }
#define	COL_1			{ &PORTB, _BV(PB5) }
#define	COL_2			{ &PORTB, _BV(PB4) }
#define	COL_3			{ &PORTB, _BV(PB6) }
#define	COL_4			{ &PORTB, _BV(PB1) }
#define	COL_5			{ &PORTB, _BV(PB2) }
#define	COL_6			{ &PORTB, _BV(PB3) }
#define	COL_7			{ &PORTB, _BV(PB0) }

#define	pCOL1			PINC
#define	pCOL2			PINB

//------------------------------------------------------------------------------
// Spacebar Row and Column Definition (K6)

#define	bCOL6			PB3	/* Space bar column */

#define	ROW_SPC			ROW_K
#define	pCOL_SPC		pCOL2
#define bCOL_SPC		bCOL6

//------------------------------------------------------------------------------
// LED Configuration

#define LED_0
#define LED_1
#define LED_2

#define	bLED_SCR		bLED_0
#define	bLED_WIN		bLED_2
#define	bLED_CAP		bLED_1

#define	pLED_SCR		pLED_0
#define	pLED_WIN		pLED_2
#define	pLED_CAP		pLED_1

#define	mLED_SCR		_BV(bLED_SCR)
#define	mLED_WIN		_BV(bLED_WIN)
#define	mLED_CAP		_BV(bLED_CAP)

#define	mLEDS			(mLED_SCR | mLED_WIN | mLED_CAP)

#define	bLED_SLP		bLED_SCR
#define	pLED_SLP		pLED_SCR

#define mLED_SLP		_BV(bLED_SLP)

#if defined(_QF_3)
 #undef  LED_LVL_DEF
 #define LED_LVL_DEF		40
#endif

//------------------------------------------------------------------------------
// Demo Core

#define	__BR_IDX( n )		pgm_read_byte( breathe + ((idx + (n)) % ARRSZ( breathe )) )

#define demo_leds()		__WRAP__(			\
	{							\
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

static inline uint8_t FA_INLINE( do_leds ) ( void )
{
    uint8_t
	_mLeds = 0 ;

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

static inline uint8_t FA_INLINE( rd_cbits ) ( void )
{
    uint8_t
	_cb ;

    __asm__ __volatile__
    (
	"clr	__tmp_reg__"	"\n\t"
	"in     %0,%1"		"\n\t"
	"bst    %0,7"		"\n\t"
	"bld    __tmp_reg__,0"	"\n\t"
	"in     %0,%2"		"\n\t"
	"lsl    %0"		"\n\t"
	"bst    %0,1"		"\n\t"
	"bld    %0,0"		"\n\t"
	"bst    %0,6"		"\n\t"
	"bld    %0,1"		"\n\t"
	"bst    %0,4"		"\n\t"
	"bld    %0,6"		"\n\t"
	"bst    %0,2"		"\n\t"
	"bld    %0,4"		"\n\t"
	"bst    %0,5"		"\n\t"
	"bld    %0,2"		"\n\t"
	"bst    %0,3"		"\n\t"
	"bld    %0,5"		"\n\t"
	"bst    %0,7"		"\n\t"
	"bld    %0,3"		"\n\t"
	"bst    %0,0"		"\n\t"
	"bld    %0,7"		"\n\t"
	"andi   %0,0b11111110"	"\n\t"
	"or     %0,__tmp_reg__"	"\n\t"
	: "=d" (_cb)
	: "I" (_SFR_IO_ADDR(pCOL1)),
	  "I" (_SFR_IO_ADDR(pCOL2))
	: "cc"
    ) ;

    return ( _cb ) ;
}

//------------------------------------------------------------------------------

#endif	// __Flake2_h__
