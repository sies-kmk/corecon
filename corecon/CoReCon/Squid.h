#if ! defined(__Squid_h__) && defined(GhostSquid)
/*******************************************************************************
 * File Name	: Squid.h
 * Project	: Costar Replacement Controllers
 * Date		: 2014/05/22
 * Version	: 1.0
 * Target MCU   : ATMEGA32U2
 * Tool Chain	: Atmel AVR Studio 4.19 730, avr-gcc, avr-libc
 * Author	: "Detlef Mueller" <detlef@gmail.com>
 * Device	: The Ghost Squid
 *
 * Release Notes:
 *
 * Pinout ATmega32U2
 * =================
 *
 * PB0 Col 6     i      PC0 (/XTAL2)            PD0 WIN LED     o
 * PB1 Col 0     i      PC1 (/RESET)            PD1 R F         o
 * PB2 Col 5     i      PC2 Col 1      i        PD2 R E         o
 * PB3 Col 4     i                              PD3 R A         o
 * PB4 Col 3     i      PC4 --                  PD4 R B         o
 * PB5 Col 7     i      PC5 NUM LED    o        PD5 R C         o
 * PB6 Col 2     i      PC6 CAP LED    o        PD6 R D         o
 * PB7 SCR LED   o      PC7 --                  PD7 HWB     
 *
 * $Id$
 ******************************************************************************/

#define __Squid_h__

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
			 _B1( PD3) | _B1( PD2) | _B1( PD1) | _B1( PD0))
#define DDD		(DDI(DDD7) | DDO(DDD6) | DDO(DDD5) | DDO(DDD4) | \
			 DDO(DDD3) | DDO(DDD2) | DDO(DDD1) | DDO(DDD0))

//------------------------------------------------------------------------------
// Row and Column Setup

#define KEYS			105

#define	pROW			PORTD

#define	bR_A			PD3
#define	bR_B			PD4
#define	bR_C			PD5
#define	bR_D			PD6
#define	bR_E			PD2
#define	bR_F			PD1

#define	mPD7			PU0(PD7)	/* HWB */
#define	mPD0			_B1(PD0)	/* WIN LED */

#define mR(r)			((r) | mPD0 | mPD7)

//------------------------------------------------------------------------------

// Bit patterns adressing rows A-R

#define	ROW_A			mR(mR18)
#define	ROW_B			mR(mR11)
#define	ROW_C			mR(mR15)
#define	ROW_D			mR(mR12)
#define	ROW_E			mR(mR13)
#define	ROW_F			mR(mR10)
#define	ROW_G			mR(mR16)
#define	ROW_H			mR(mR14)
#define	ROW_I			mR(mR17)
#define	ROW_J			mR(mR19)
#define	ROW_K			mR(mR06)
#define	ROW_L			mR(mR02)
#define	ROW_M			mR(mR09)
#define	ROW_N			mR(mR07)
#define	ROW_O			mR(mR03)
#define	ROW_P			mR(mR08)
#define	ROW_Q			mR(mR04)
#define	ROW_R			mR(mR05)

// Col info, for reference only.

#define	COL_0			{ &PORTB, _BV(PB1) }
#define	COL_1			{ &PORTC, _BV(PC2) }
#define	COL_2			{ &PORTB, _BV(PB6) }
#define	COL_3			{ &PORTB, _BV(PB4) }
#define	COL_4			{ &PORTB, _BV(PB3) }
#define	COL_5			{ &PORTB, _BV(PB2) }
#define	COL_6			{ &PORTB, _BV(PB0) }
#define	COL_7			{ &PORTB, _BV(PB5) }

#define	pCOL1			PINC
#define	pCOL2			PINB

//------------------------------------------------------------------------------
// Spacebar Row and Column Definition (K6)

#define	bCOL6			PB0	/* Space bar column */

#define	ROW_SPC			ROW_K
#define	pCOL_SPC		pCOL2
#define bCOL_SPC		bCOL6

//------------------------------------------------------------------------------
// LED Configuration

#define	LED_0
#define	LED_1
#define	LED_2
#define	LED_3

#define bLED_WIN		bLED_3
#define	bLED_SCR		bLED_2
#define	bLED_NUM		bLED_1
#define	bLED_CAP		bLED_0

#define pLED_WIN		pLED_3
#define	pLED_SCR		pLED_2
#define	pLED_NUM		pLED_1
#define	pLED_CAP		pLED_0

#define mLED_WIN		_BV(bLED_WIN)
#define	mLED_SCR		_BV(bLED_SCR)
#define	mLED_NUM		_BV(bLED_NUM)
#define	mLED_CAP		_BV(bLED_CAP)

#define	mLEDS			(mLED_WIN | mLED_SCR | mLED_NUM | mLED_CAP)

#define	bLED_SLP		bLED_WIN
#define	pLED_SLP		pLED_WIN

#define mLED_SLP		_BV(bLED_SLP)

#undef  LED_LVL_DEF
#define LED_LVL_DEF		40

//------------------------------------------------------------------------------
// Demo Core

#define	__BR_IDX( n )		pgm_read_byte( breathe + ((idx + (n)) % ARRSZ( breathe )) )

#define demo_leds()		__WRAP__(			\
	{							\
	    if ( dir )						\
	    {							\
		set_leds_raw( mLED_SCR, __BR_IDX( 12 ) ) ;	\
		set_leds_raw( mLED_CAP, __BR_IDX(  8 ) ) ;	\
		set_leds_raw( mLED_NUM, __BR_IDX(  4 ) ) ;	\
		set_leds_raw( mLED_WIN, pgm_read_byte( breathe + idx ) ) ;	\
	    }							\
	    else						\
	    {							\
		set_leds_raw( mLED_WIN, __BR_IDX( 12 ) ) ;	\
		set_leds_raw( mLED_NUM, __BR_IDX(  8 ) ) ;	\
		set_leds_raw( mLED_CAP, __BR_IDX(  4 ) ) ;	\
		set_leds_raw( mLED_SCR, pgm_read_byte( breathe + idx ) ) ;	\
	    }							\
	} )

//------------------------------------------------------------------------------
// Alarm Flasher

#define al_flash()		__WRAP__(			\
	{							\
	    if ( al_pat & 0x001 )				\
		set_leds_raw( mLED_WIN | mLED_CAP, _x(100) ) ;	\
	    else						\
	    if ( al_pat & 0x100 )				\
		set_leds_raw( mLED_NUM | mLED_SCR, _x(100) ) ;	\
	} )

//------------------------------------------------------------------------------
// Handle LED's

static void
    set_layer( void ),
    set_leds( uint8_t leds, uint8_t lvl ) ;

static inline uint8_t
	__FA__( do_leds, __always_inline__ ) ( void )
{
    uint8_t
	_mLeds = 0 ;

    if ( fFN || fWinLk )
    {
	_mLeds |= mLED_WIN ;

	set_leds( mLED_WIN,	// Superimpose FN status to WIN LED
		  (fFN ? LED_LVL_MAX - led_lvl : 0) + (fWinLk ? led_lvl : 0) ) ;
    }

    if ( (kbd_leds & mLED_NUML) || fNumLk )
    {
	if ( fNumLk && (kbd_leds & mLED_NUML) )
	{
	    fNumLk = FALSE ;	// Real NL over-rides LNL

	    set_layer() ;
	}

	_mLeds |= mLED_NUM ;

	set_leds( mLED_NUM, (fNumLk ? LED_LVL_MAX - led_lvl : led_lvl) ) ;
    }

    if ( (kbd_leds & mLED_SCRL) )
    {
	_mLeds |= mLED_SCR ;

	set_leds( mLED_SCR, led_lvl ) ;
    }

    if ( (kbd_leds & mLED_CAPS) )
    {
	_mLeds |= mLED_CAP ;

	set_leds( mLED_CAP, led_lvl ) ;
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
	"bld    %0,1"		"\n\t"
	"andi   %0,0b00000010"	"\n\t"
	"mov    __tmp_reg__,%0"	"\n\t"
	"in     %0,%2"		"\n\t"
	"bst    %0,4"		"\n\t"
	"bld    %0,7"		"\n\t"
	"bst    %0,1"		"\n\t"
	"bld    %0,4"		"\n\t"
	"bst    %0,2"		"\n\t"
	"bld    %0,1"		"\n\t"
	"bst    %0,0"		"\n\t"
	"bld    %0,2"		"\n\t"
	"bst    %0,3"		"\n\t"
	"bld    %0,0"		"\n\t"
	"bst    %0,5"		"\n\t"
	"bld    %0,3"		"\n\t"
	"swap   %0"		"\n\t"
	"andi   %0,0b11111101"	"\n\t"
	"or     %0,__tmp_reg__"	"\n\t"
	: "=d" (_cb)
	: "I" (_SFR_IO_ADDR(pCOL1)),
	  "I" (_SFR_IO_ADDR(pCOL2))
	: "cc"
    ) ;

    return ( _cb ) ;
}

//------------------------------------------------------------------------------

#endif	// __Squid_h__
