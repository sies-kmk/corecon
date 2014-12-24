/*******************************************************************************
 * File Name	: kb.c
 * Project	: Costar Replacement Controllers
 * Date		: 2014/05/22
 * Version	: 1.0
 * Target MCU   : ATMEGA32U2
 * Tool Chain	: Atmel AVR Studio 4.19 730, avr-gcc, avr-libc
 * Author	: "Detlef Mueller" <detlef@gmail.com>
 * References	: Controllers by Fredrik Atmer, Bathroom Epiphanies Inc.
 * Release Notes:
 *
 * $Id$
 ******************************************************************************/

// #define __MACROS			// Enable macros
#define __TIMER				// Enable countdown timer

#if defined(FrostyFlake) || defined(FrostyFlake2) || defined(PegasusHoof)
 #define __ALTDEB			// Alternative debounce algorithm
#endif

#include <includes.h>
#include "usb_cfg.h"
#include <u_des_kb.h>
#include <u_hid_kb.h>
#include <kbusages.h>

#if defined(__MACROS)
 #include <kbmacro.h>
#endif

#define	__kb__
#include "kb.h"

//------------------------------------------------------------------------------
//******************************************************************************
//------------------------------------------------------------------------------

// Row & Column Definitions

#define NROWS			18
#define NCOLS			8
#define NKEYS			(NROWS * NCOLS)

//------------------------------------------------------------------------------

// LED bits and ports

#define bLED_3			PD0	/* OC0B */
#define pLED_3			PORTD
#define bLED_2			PB7	/* OC1C */
#define pLED_2			PORTB
#define	bLED_1			PC5	/* OC1B */
#define	pLED_1			PORTC
#define	bLED_0			PC6	/* OC1A */
#define	pLED_0			PORTC

//------------------------------------------------------------------------------

#define	LED_on( p, b )		clr_bit( p, b )
#define	LED_off( p, b )		set_bit( p, b )
#define	LED_sts( p, b )		bit_is_clear( p, b )

#define LED_LVL_MIN		10
#define LED_LVL_MAX		100
#define LED_LVL_STP		10
#define LED_LVL_DEF		10

//------------------------------------------------------------------------------

// Helper macros to create the ROW_A-R values. bR_A-F, mR(), and
// ROW_A-R are defined in the controller specific header files.

#define	mU2			(_B1(bR_F) | _B1(bR_E))	/* Disable U3 */
#define	mU3			(_B1(bR_D) | _B1(bR_C))	/* Disable U2 */

// U2/3 Rnn select macros

#define	mRU2( d, c, b, a )	(mU2 | _B##d(bR_D) | _B##c(bR_C) | _B##b(bR_B) | _B##a(bR_A))
#define	mRU3( f, e, b, a )	(mU3 | _B##f(bR_F) | _B##e(bR_E) | _B##b(bR_B) | _B##a(bR_A))

// Individual Rnn select masks. R00-09 connect to U2, R10-19 to U3.

#define	mR00			mRU2( 0, 0, 0, 0 )
#define	mR01			mRU2( 0, 0, 0, 1 )
#define	mR02			mRU2( 0, 0, 1, 0 )
#define	mR03			mRU2( 0, 0, 1, 1 )
#define	mR04			mRU2( 0, 1, 0, 0 )
#define	mR05			mRU2( 0, 1, 0, 1 )
#define	mR06			mRU2( 0, 1, 1, 0 )
#define	mR07			mRU2( 0, 1, 1, 1 )
#define	mR08			mRU2( 1, 0, 0, 0 )
#define	mR09			mRU2( 1, 0, 0, 1 )

#define	mR10			mRU3( 0, 0, 0, 0 )
#define	mR11			mRU3( 0, 0, 0, 1 )
#define	mR12			mRU3( 0, 0, 1, 0 )
#define	mR13			mRU3( 0, 0, 1, 1 )
#define	mR14			mRU3( 0, 1, 0, 0 )
#define	mR15			mRU3( 0, 1, 0, 1 )
#define	mR16			mRU3( 0, 1, 1, 0 )
#define	mR17			mRU3( 0, 1, 1, 1 )
#define	mR18			mRU3( 1, 0, 0, 0 )
#define	mR19			mRU3( 1, 0, 0, 1 )

// Bit pattern to deselect all rows

#define mROW			mR((mU2 | mU3 | _B1(bR_B) | _B1(bR_A)))

//------------------------------------------------------------------------------

// Bit definitions for the keyboard LED OUT report, ref. only

#define	bLED_NUML		0
#define	bLED_CAPS		1
#define	bLED_SCRL		2
#define	bLED_COMPOSE		3
#define	bLED_KANA		4

#define	mLED_NUML		_BV(bLED_NUML)
#define	mLED_CAPS		_BV(bLED_CAPS)
#define	mLED_SCRL		_BV(bLED_SCRL)

// Bit definitions for the keyboard modifiers in the IN report, ref. only

#define	bLFT_CTRL		0
#define	bLFT_SHFT		1
#define	bLFT_ALT		2
#define	bLFT_GUI		3
#define	bRGT_CTRL		4
#define	bRGT_SHFT		5
#define	bRGT_ALT		6
#define	bRGT_GUI		7

//------------------------------------------------------------------------------

#define MS_PER_SEC		1000u
#define MS_PER_MIN		(60u * MS_PER_SEC)

//------------------------------------------------------------------------------
//******************************************************************************
//------------------------------------------------------------------------------

volatile uint8_t
    alarm ;				// Signal for main() that time is up

//------------------------------------------------------------------------------

static const uint8_t
    *layer ;				// Active keyboard layer

static uint8_t
    led_lvl,				// LED brightness level
    fFN,				// FN layer selector
    fDemoOff,				// Flag: LED demo mode flag
    fLock,				// Flag: keyboard locked
    fNumLk,				// Flag: num-pad/local NUML active
    fWinLk ;				// Flag: WIN key lock active

static uint8_t
  #if ENABLE_CTRL_KEYS
    kRep1, kRep2,			// Keycodes of reports 1 & 2
  #endif
    kRep0[KBD_KRO + 8] ;		// Keycodes of report 0 (+8 mods)

#if defined(__TIMER)
 static uint8_t
    tm_on,				// Timer on/off flag
    tm_min ;				// Minutes until alarm

 static uint16_t
    tm_ms ;				// msec count down
#endif

//------------------------------------------------------------------------------

// Controller Specific Includes

#include "Petal.h"
#include "Squid.h"
#include "Flake.h"
#include "Hoof.h"
#include "Paw.h"
#include "Flake2.h"

//------------------------------------------------------------------------------

// Key Matrix Includes

#include "matrix.h"

#if   KEYS == 104
 #include "Keys104.h"
#elif KEYS == 105
 #include "Keys105.h"
#elif KEYS == 87
 #include "Keys87.h"
#elif KEYS == 88
 #include "Keys88.h"
#else
 #error "KEYS out of range or not defined"
#endif

//------------------------------------------------------------------------------

static const uint8_t
    uLayer1[] PROGMEM = U_L1,		// Keycode-to-usage matrix layer 1
    uLayer2[] PROGMEM = U_L2,		// Keycode-to-usage matrix layer 2
    uLayer3[] PROGMEM = U_L3,		// Keycode-to-usage matrix layer 3
    uLayer4[] PROGMEM = U_L4 ;		// Keycode-to-usage matrix layer 4

//------------------------------------------------------------------------------
//******************************************************************************
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Misc. helper functions
//------------------------------------------------------------------------------

// Clear a small block of memory (<256 bytes)

static void memclr8 ( uint8_t *m, uint8_t sz )
{
    uint8_t
	*p = m ;			// Stupid gcc. More efficient this way.

    for ( ; sz-- ; )
	*p++ = 0 ;
}

//------------------------------------------------------------------------------

// Set active keyboard layer

static void FA_NOINLINE( set_layer ) ( void )
{
    if ( fFN )				// FN and FN2 layers
	layer = (fFN == 1 ? uLayer2 - 1 : uLayer3 - 1) ;
    else
    if ( fNumLk )			// LNUML layer
	layer = uLayer4 - 1 ;
    else				// Regular layer
	layer = uLayer1 - 1 ;

// Left the following code here for reference. Accomplishes
// the same as the code above more clean but less efficient.
//
//  #else
//    static const uint8_t * const	// This array is addressed by fFN.
//	uMatrix[] PROGMEM =		// Note: -1 because we start counting
//	{				// keys at 1 in the matrix scanner
//	    uLayer1 - 1,		// Regular layer
//	    uLayer2 - 1,		// FN layer
//	    uLayer3 - 1,		// FN2 layer
//	    uLayer4 - 1			// Local NUM Lock layer
//	} ;
//
//    layer = (uint8_t *)pgm_read_word(
//		uMatrix + (fFN ? fFN : (fNumLk ? ARRSZ( uMatrix ) - 1 : 0)) ) ;
}

//------------------------------------------------------------------------------

// Reboot into bootloader.

static void FA_NOINRET( bootloader ) ( void )
{
    TIMSK0 = 0 ;			// Kill timer interrupt

    UDCON  = _B1(DETACH) ;		// Detach from USB

    cli() ;				// Disable interrupts

    USBCON = _B0(USBE) | _B1(FRZCLK) ;	// Stop USB module
    clr_bit( PLLCSR, PLLE ) ;		// Stop PLL

    wdt_disable() ;			// Leash watchdog

    // Disable peripherals

    DDRB   = 0 ; PORTB  = 0 ;
    DDRC   = 0 ; PORTC  = 0 ;
    DDRD   = 0 ; PORTD  = 0 ;

    PRR0   = 0 ;

    TCCR1A = 0 ; TCCR1B = 0 ;
    TCCR0A = 0 ; TCCR0B = 0 ;

    // Wait 15ms to make sure detach is recognized

    _delay_loop_2( (uint16_t)MS2TM( 15, 4 ) ) ;

    jmp_bootloader() ;			// Off we go
}

//------------------------------------------------------------------------------
// LED related functions
//------------------------------------------------------------------------------

// Maintain Sleep LED. Note: called every 1ms (system clock throttled)

#define	SLEEP_OFF_DELAY		1				/* 1ms on */
#define	SLEEP_ON_DELAY		(3000 - SLEEP_OFF_DELAY)	/* once w/in 3s */

void sleep_led ( uint8_t reset )
{
    static uint16_t
	timer ;

    if ( reset )
    {
	LED_off( pLED_SLP, bLED_SLP ) ;
	timer = 0 ;

	return ;
    }

    if ( ! timer-- )			// Timer expired
    {
	if ( LED_sts( pLED_SLP, bLED_SLP ) )
	{				// LED on
	    LED_off( pLED_SLP, bLED_SLP ) ;
	    timer = SLEEP_ON_DELAY ;
	}
	else
	{
	    LED_on( pLED_SLP, bLED_SLP ) ;
	    timer = SLEEP_OFF_DELAY ;
	}
    }
}

//------------------------------------------------------------------------------

static void set_leds_off ( uint8_t leds )
{
    if ( leds & _BV(bLED_0) )
	OCR1AL = 255 ;

    if ( leds & _BV(bLED_1) )
	OCR1BL = 255 ;

  #if defined(LED_2)
    if ( leds & _BV(bLED_2) )
	OCR1CL = 255 ;
  #endif

  #if defined(LED_3)
    if ( leds & _BV(bLED_3) )
	OCR0B  = 125 ;
  #endif
}

//------------------------------------------------------------------------------

static void set_leds_raw ( uint8_t leds, uint8_t lvl )
{
    if ( leds & _BV(bLED_0) )
	OCR1AL = lvl ;

    if ( leds & _BV(bLED_1) )
	OCR1BL = lvl ;

  #if defined(LED_2)
    if ( leds & _BV(bLED_2) )
	OCR1CL = lvl ;
  #endif

  #if defined(LED_3)
    if ( leds & _BV(bLED_3) )
	OCR0B  = (lvl > 6 ? ((lvl - 5) >> 1) : 0) ;
  #endif
}

//------------------------------------------------------------------------------

// Set dim level for multiple LEDs. 0: off, 100: full on

#define _x( n )	(255 - ((n) <= 80 ? (n) * 2 : 155 + ((n) - 80) * 4 + ((n) - 80)))

static void set_leds ( uint8_t leds, uint8_t lvl )
{
    set_leds_raw( leds, _x(lvl) ) ;
}

//------------------------------------------------------------------------------

#define	DEMO_TIMER		45	/* Change LED's every 45ms */

static void led_demo ( uint8_t reset )
{
    static const uint8_t
	breathe[] PROGMEM =
	{
	      _x(0),  _x(0),  _x(0),  _x(0),  _x(0),  _x(0),  _x(0),  _x(0),
	      _x(0),  _x(0),  _x(0),  _x(0),  _x(1),  _x(2),  _x(3),  _x(4),
	      _x(6),  _x(9), _x(13), _x(17), _x(23), _x(29), _x(37), _x(45),
	     _x(53), _x(62), _x(70), _x(78), _x(86), _x(92), _x(96), _x(99),
	    _x(100), _x(99), _x(96), _x(92), _x(86), _x(78), _x(70), _x(62),
	     _x(53), _x(45), _x(37), _x(29), _x(23), _x(17), _x(13),  _x(9),
	      _x(6),  _x(4),  _x(3),  _x(2),  _x(1),  _x(0),  _x(0),  _x(0),
	      _x(0),  _x(0),  _x(0),  _x(0),  _x(0),  _x(0),  _x(0),  _x(0)
	} ;

    static uint8_t
	timer,
	dir,
	idx ;

    if ( reset )
    {
	timer = 0 ;
	idx   = 0 ;
	dir   = 0 ;

	set_leds_off( mLEDS ) ;

	return ;
    }

    if ( ! timer-- )
    {
	timer = DEMO_TIMER ;

	if ( fLock )
	    set_leds_raw( mLED_SLP, pgm_read_byte( breathe + idx ) ) ;
	else
	    demo_leds() ;

	idx = (idx + 1) % ARRSZ( breathe ) ;

	if ( ! idx )
	    dir = ! dir ;
    }
}

//------------------------------------------------------------------------------

// Maintain LED's

#define	DEMO_DELAY		3000	/* Start demo after 3s w/o LED activity */
#define	LED_TIMER		1

void maint_leds ( uint8_t reset )
{
    static uint16_t
      #if defined(__TIMER)
	al_pat,				// Alarm pattern
      #endif
	demo ;				// Demo start timer

    static uint8_t
      #if defined(__TIMER)
	kb_leds,
      #endif
	timer ;

    uint8_t
	mLeds ;

    if ( reset )
    {
	demo  = DEMO_DELAY ;
	timer = 0 ;

      #if defined(__TIMER)
	al_pat = 0b000000010101u ;	// 50ms per bit, three flashes per cycle
      #endif

	set_leds_off( mLEDS ) ;

	_delay_loop_2( 512 ) ;		// let timers elapse, 512 * 4 clocks

	return ;
    }

    if ( ! timer-- )			// Maintain LED's every 1ms
    {
	timer = LED_TIMER ;

      #if defined(__TIMER)
	if ( tm_on )			// Timer set
	{
	    if ( ! tm_min )		// Timer expired, flash LED's
	    {
		static uint8_t
		    al_tim ;

	 	if ( ! al_tim-- )
		{
		    al_tim = (MS_PER_SEC / 40) - 1 ; // 25ms timer

		    al_flash() ;

		    al_pat = ror16( al_pat ) ;	// 16bit * 25ms, 400ms
		}
		else
		if ( al_tim == 5 )	// Flashes are 20ms on, 30ms off
		    set_leds_off( mLEDS ) ;

		if ( kb_leds != kbd_leds )
		    tm_on = FALSE ;

		return ;
	    }
	    else			// Timer not expired yet
	    {
		if ( ! --tm_ms )	// 1min expired
		{			// Reset ms timer, count down minutes
		    if ( ! --tm_min )
			kb_leds = kbd_leds ;
		    else
			tm_ms = MS_PER_MIN ;
		}
	    }
	}
      #endif

	mLeds = 0 ;

	if ( ! fLock )
	    mLeds = do_leds() ;

	if ( mLeds )
	{
	    set_leds_off( mLeds ^ mLEDS ) ;

	    demo = DEMO_DELAY ;		// Reset demo timer
	}
	else				// No LED active
	{
	    if ( demo )			// Waiting for demo display
	    {
		if ( fDemoOff && ! fLock )
		{
		    if ( demo == DEMO_DELAY )	// First run after LED's off
		    {
			set_leds_off( mLEDS ) ;	// Set all LED's off
			--demo ;
		    }
		}
		else
		{
		    if ( demo == DEMO_DELAY )	// First run after LED's off
			set_leds_off( mLEDS ) ;	// Set all LED's off

		    if ( ! --demo )		// Demo enable on next run
			led_demo( SIG_RESET ) ;	// Reset demo routine
		}
	    }
	    else
		led_demo( SIG_MAINT ) ;
	}
    }
}

//------------------------------------------------------------------------------
// Key related functions
//------------------------------------------------------------------------------

// Check for space bar down during suspend. Note: called every 1ms.

uint8_t wakeup ( uint8_t reset )
{
    static uint8_t
	sb ;

    if ( reset )
    {
	sb = 0 ;

	return ( FALSE ) ;
    }

  #if bLED_SLP == bLED_3
    pROW = (pROW & _B1(bLED_SLP)) | (ROW_SPC & ~_B1(bLED_SLP)) ;
  #else
    pROW = ROW_SPC ;
  #endif

    _delay_loop_1( (uint8_t)(US2TM( 1, 3 ) / 2) ) ;

    sb = (sb << 1) | bit_is_clear( pCOL_SPC, bCOL_SPC ) ;

  #if bLED_SLP == bLED_3
    pROW = (pROW & _B1(bLED_SLP)) | (mROW & ~_B1(bLED_SLP)) ;
  #else
    pROW = mROW ;			// Set all row bits to deselect row
  #endif

    return ( sb == 0x7F ) ;
}

//------------------------------------------------------------------------------

// Handle key down event

static uint8_t FA_NOINLINE( key_down ) ( uint8_t k )
{
    __label__
	Send_U ;

  #if ENABLE_CTRL_KEYS
    static const uint16_t
	U_CC_table[] PROGMEM =		// LUT for Consumer Control usages
	{
	    U_NextTrack, U_PrevTrack, U_Stop, U_StopEject, U_PlayPause,
	    U_Mute, U_VolUp, U_VolDwn,
	    U_AL_CC_Config, U_AL_Email, U_AL_Calculator, U_AL_LocalBrowser,
	    U_AC_Search, U_AC_Home, U_AC_Back, U_AC_Fwd,
	    U_AC_Stop, U_AC_Refresh, U_AC_Bookmarks
	} ;
  #endif

    uint8_t
	i,
	usage = pgm_read_byte( layer + k ) ;

    if ( ! usage )
	return ( FALSE ) ;		// No usage, nothing to do

    if ( (usage & 0x80) )		// Special handling
    {
	if ( (usage & 0xF0) == 0xE0 )	// Modifier
	{
	    if ( fLock || (fWinLk && (usage == U_LGui || usage == U_RGui)) )
		return ( FALSE ) ;

	    kRep0[KBD_KRO + (usage & 0x07)] = k ;

 	    kbd_report.mod |= _bv8( usage & 0x07 ) ;
	}
	else
	if ( (usage & 0xF0) == 0xF0 )	// Misc. special functions
	{
	    if ( usage == U_FN )	// FN key. Make sure you don't define
	    {				// more than two entries in a matrix
		++fFN ;

		set_layer() ;

		return ( FALSE ) ;
	    }

	    if ( usage == U_BtLd )	// Reboot into bootloader
		bootloader() ;

	    if ( usage == U_Lock )	// Keyboard lock toggle
	    {
		fLock = ! fLock ;

		return ( FALSE ) ;
	    }

	    if ( fLock )
		return ( FALSE ) ;

	    if ( usage == U_LNLk )	// Local NUM lock toggle
	    {
		if ( (kbd_leds & mLED_NUML) )
		{			// Global NL set
		    usage = U_NLck ;	// Send NL key
		    goto Send_U ;
		}

		fNumLk = ! fNumLk ;

		set_layer() ;

		return ( FALSE ) ;
	    }

	    if ( usage == U_WLck )	// Win key lock toggle
	    {
		fWinLk = ! fWinLk ;

		return ( FALSE ) ;
	    }

	    if ( usage == U_LBUp )	// LED brightness up
	    {
		if ( led_lvl < LED_LVL_MAX - LED_LVL_STP )
		    led_lvl += LED_LVL_STP ;

		return ( FALSE ) ;
	    }

	    if ( usage == U_LBDn )	// LED brightness down
	    {
		if ( led_lvl > LED_LVL_STP )
		    led_lvl -= LED_LVL_STP ;

		return ( FALSE ) ;
	    }

	    if ( usage == U_Demo )	// LED demo mode toggle
	    {
		fDemoOff = ! fDemoOff ;

		return ( FALSE ) ;
	    }

	  #if defined(__MACROS)
	    // Macro. ATTN: may need work, safe w/ multiple keys down simult.?

	    return ( (usage & 0x0F) << 4 ) ;
	  #else
	    return ( FALSE ) ;
	  #endif
	}
	else
      #if defined(__TIMER)
	if ( (usage & 0xF0) == 0xA0 )	// Timer setup
	{
	    __label__
		Set_timer ;

	    switch ( usage )
	    {
		case U_TOff :		// Timer off/fire

		    if ( tm_on )	// If on, kill it
		    {
			tm_min = 0 ;
			tm_on  = FALSE ;
		    }
		    else		// If off, 1ms delayed fire
		    {
			tm_min = 1 ;
			tm_ms  = 1 ;
			tm_on  = TRUE ;
		    }

		    break ;

		case U_1mn :		// Timer on, add 1min
		    tm_min +=  1 ; goto Set_timer ;
		case U_5mn :		// Timer on, add 5min
		    tm_min +=  5 ; goto Set_timer ;
		case U_10mn :		// Timer on, add 10min
		    tm_min += 10 ; goto Set_timer ;
		case U_30mn :		// Timer on, add 30min
		    tm_min += 30 ; goto Set_timer ;
		case U_60mn :		// Timer on, add 60min
		    tm_min += 60 ;

		Set_timer :
		    if ( ! tm_on )
		    {
			tm_on = TRUE ;
			tm_ms = MS_PER_MIN ;
		    }
	    }

	    return ( FALSE ) ;
	}
	else
      #endif
      #if ENABLE_CTRL_KEYS
	if ( fLock )
	    return ( FALSE ) ;
	else
	if ( (usage & 0xF0) == 0x80 )	// System Control
	{
	    kRep1 = k ;

	    ctrl_report1.key = (uint16_t)usage ;

	    return ( REP1_CHG ) ;
	}
	else				// Should be Consumer Control
	{
	    kRep2 = k ;

	    ctrl_report2.key =
	 	pgm_read_word( U_CC_table - U_CC_Offs + usage ) ;

	    return ( REP2_CHG ) ;
	}
      #else
	    return ( FALSE ) ;
      #endif
    }
    else				// Regular key
  Send_U:
    {
	CRITICAL_VAR() ;

	if ( fLock )
	    return ( FALSE ) ;

	for ( i = 0 ; i < KBD_KRO ; ++i )
	    if ( kRep0[i] == U_None )	// Found empty spot in report
	    {
		kRep0[i] = k ;
		kbd_report.keys[i] = usage ;

		return ( REP0_CHG ) ;
	    }

	// Report full, drop oldest key

	ENTER_CRITICAL() ;

	for ( i = 0 ; i < KBD_KRO - 1 ; ++i )
	{
	    kRep0[i] = kRep0[i + 1] ;
	    kbd_report.keys[i] = kbd_report.keys[i + 1] ;
	}

	kRep0[i] = k ;
	kbd_report.keys[i] = usage ;

	EXIT_CRITICAL() ;
    }

    return ( REP0_CHG ) ;
}

//------------------------------------------------------------------------------

// Handle key up event

static uint8_t FA_NOINLINE( key_up ) ( uint8_t k )
{
    uint8_t
	i,
	*kp = kRep0,
	*rp = kbd_report.keys ;

    for ( i = 0 ; i < KBD_KRO ; ++i, ++kp, ++rp )
	if ( *kp == k )		// Regular key
	{
	    CRITICAL_VAR() ;

	    ENTER_CRITICAL() ;

	    for ( ; i < KBD_KRO - 1 ; ++i, ++kp, ++rp )
	    {
		*kp = *(kp + 1) ;
		*rp = *(rp + 1) ;
	    }

	    *kp = 0 ;
	    *rp = U_None ;

	    EXIT_CRITICAL_RET( REP0_CHG ) ;
	}

    for ( ; i < ARRSZ( kRep0 ) ; ++i, ++kp )
	if ( *kp == k )		// Modifier
	{
	    *kp = 0 ;
	    kbd_report.mod &= ~_bv8( i - KBD_KRO ) ;

	    return ( REP0_CHG ) ;
	}

  #if ENABLE_CTRL_KEYS
    if ( kRep1 == k )			// System Control
    {
	kRep1 = 0 ;
	ctrl_report1.key = U_None ;

	return ( REP1_CHG ) ;
    }

    if ( kRep2 == k )			// Consumer Control
    {
	kRep2 = 0 ;
	ctrl_report2.key = U_None ;

	return ( REP2_CHG ) ;
    }
  #endif

    if ( fFN && pgm_read_byte( layer + k ) == U_FN )
    {
	--fFN ;				// FN key

	set_layer() ;
    }

    // Don't need to do anything for other usages or dropped keys

    return ( FALSE ) ;
}

//------------------------------------------------------------------------------

// Read and maintain key matrix

#if defined(__ALTDEB)
 #define mKUP			0b00111111	/* 3ms stable for regular deb. */
 #define mKDN			0b01000000
 #define mKUPX			0b00011111	/* 2.5ms stable for fast deb. */
 #define mKDNX			0b00100000

 #define mKST_TRANS		0b00000010	/* "In transition" flag */
 #define KST_UP			0
 #define KST_DN			1
 #define KST_T_DN		(KST_UP | mKST_TRANS)
 #define KST_T_UP		(KST_DN | mKST_TRANS)
 #define mKST_DEAD		0x80
#else
 #define mKUP			0b00111111	/* 3ms stable */
 #define mKDN			0b01000000

 #define mKST			0b10000000	/* b7 used for key state */
#endif

uint8_t read_matrix ( uint8_t reset )
{
    static const uint8_t
	row_sel[] PROGMEM =		// Patterns to select rows
	{
	    ROW_A, ROW_B, ROW_C, ROW_D, ROW_E, ROW_F,
	    ROW_G, ROW_H, ROW_I, ROW_J, ROW_K, ROW_L,
	    ROW_M, ROW_N, ROW_O, ROW_P, ROW_Q, ROW_R,
	    mROW			// Deselect rows
	} ;

  #if defined(__MACROS)
    static uint8_t
	macro ;				// Flag: macro playback active
  #endif

  #if defined(__ALTDEB)
    static struct __keys
	{
	    uint8_t
		sts, deb ;
	}
	keys[NKEYS] ;			// Key status'

    struct __keys
  #else
    static uint8_t
	keys[NKEYS] ;			// Key status'

    uint8_t
  #endif
	*kp = keys ;			// -> keys

    uint8_t
	r, c,				// row and column counters
	cb,				// Column bits
	b,				// bounce & status bits
	ret ;

    if ( reset )
    {
	// Clear key status'

	for ( r = 0 ; r < NKEYS ; ++r, ++kp )
	{
	    if ( pgm_read_byte( uLayer1 + r ) )
	  #if defined(__ALTDEB)
		kp->sts = KST_UP ;	// Key exists, matrix entry active
	    else
		kp->sts = mKST_DEAD ;	// Don't track inactive matrix entries

	    kp->deb = 0xFF ;
	  #else
		*kp = 0x7F ;		// Key exists, matrix entry active
	    else
		*kp = 0xFF ;		// Don't track inactive matrix entries
	  #endif
	}

	// Clear kbd report & key code memory

	memclr8( VP( &kbd_report ), sizeof( kbd_report ) ) ;
	memclr8( kRep0, sizeof( kRep0 ) ) ;

      #if defined(__MACROS)
	macro = FALSE ;
      #endif

	fFN = 0 ;

	set_layer() ;

      #if ENABLE_CTRL_KEYS
	kRep1 = 0 ;
	kRep2 = 0 ;

	ctrl_report1.key = U_None ;	// Clear SC report
	ctrl_report2.key = U_None ;	// Clear CC report

	return ( REP0_CHG | REP1_CHG | REP2_CHG ) ;
      #else
	return ( REP0_CHG ) ;
      #endif
   }

  #if defined(__MACROS)
    if ( macro )			// Playing macro
    {
	ret = play_macro( 0 ) ;

	macro = ret & M_ACTIVE ;

	return ( ret & 0x0F ) ;
    }
  #endif

    ret = 0 ;

    pROW = ROW_A ;			// Select 1st row

    _delay_loop_1( (uint8_t)US2TM( 1, 3 ) ) ;

    for ( r = 1 ; r <= NROWS ; ++r )
    {
	// Read column bits and get them in order

	cb = rd_cbits() ;

	// Select next row

	pROW = pgm_read_byte( row_sel + r ) ;

      #if defined(__ALTDEB)

	if ( fWinLk )
	{
	    for ( c = NCOLS ; c-- ; )
	    {
		if ( ! (kp->sts & mKST_DEAD) )	// Track only existing keys
		{
		    b  = (kp->deb & mKUPX) << 1 ;
		    b |= (cb & 1) ;

		    kp->deb = b ;		// Save new key debounce bits

		    if ( kp->sts == KST_UP )	// Key marked "UP"
		    {
			if ( ! (b & 1) )	// Started transitioning DN
			{
			    kp->sts = KST_T_DN ;// Mark as trans. DN and send event
			    ret |= key_down( kp - (keys - 1) ) ;
			}
		    }
		    else
		    if ( kp->sts == KST_DN )	// Key marked "DN"
		    {
			if ( (b & 1) )		// Started transitioning UP
			{
			    kp->sts = KST_T_UP ;// Mark as trans. UP and send event
			    ret |= key_up( kp - (keys - 1) ) ;
			}
		    }
		    else
		    if ( kp->sts == KST_T_DN )	// Key transitioning DN
		    {
			if ( b == mKDNX )	// Is DN now
			    kp->sts = KST_DN ;	// Mark as DN
			else
			if ( b == mKUPX )	// Is UP again
			{
			    kp->sts = KST_UP ;	// Mark as UP and send event
			    ret |= key_up( kp - (keys - 1) ) ;
			}
		    }
		    else
//		    if ( kp->sts == KST_T_UP )	// Key transitioning UP
		    {
			if ( b == mKUPX )	// Is UP now
			    kp->sts = KST_UP ;	// Mark as UP
			else
			if ( b == mKDNX )	// Is DN again
			{
			    kp->sts = KST_DN ;	// Mark as DN and send event
			    ret |= key_down( kp - (keys - 1) ) ;
			}
		    }
		}

		++kp ;
		cb >>= 1 ;
	    }				// for ( NCOLS )
	}
	else				// fWinLk
	{
	    for ( c = NCOLS ; c-- ; )
	    {
		if ( ! (kp->sts & mKST_DEAD) )	// Track only existing keys
		{
		    b  = (kp->deb & mKUP) << 1 ;
		    b |= (cb & 1) ;

		    kp->deb = b ;		// Save new key debounce bits

		    if ( b == mKDN && ! kp->sts )
		    {				// Is down, was up
			kp->sts = 1 ;		// Remember key down
			ret |= key_down( kp - (keys - 1) ) ;
		    }
		    else
		    if ( b == mKUP &&   kp->sts )
		    {				// Is up, was down
			kp->sts = 0 ;		// Remember key up
			ret |= key_up( kp - (keys - 1) ) ;
		    }
		}

		++kp ;
		cb >>= 1 ;
	    }				// for ( NCOLS )
	}				// fWinLk

      #else				// __ALTDEB

	for ( c = NCOLS ; c-- ; )
	{
	    if ( (uint8_t)~*kp )	// Track only existing keys
	    {
		b = *kp & (mKUP | mKST) ;

		// Replace
		// b = (b & 0x80) | (b << 1) | (cb & 1) ;
		// with asm code to speed things up:

		asm volatile
		(
		    "bst  %[b],7"	"\n\t"
		    "lsl  %[b]"		"\n\t"
		    "bld  %[b],7"	"\n\t"
		    "bst  %[cb],0"	"\n\t"
		    "bld  %[b],0"	"\n\t"
		    : [b] "+r" (b) 
		    : [cb] "r" (cb)
		    : "cc"
		) ;

		if ( b == (mKDN & ~mKST) )	// Is down, was up
		{
		    b = (mKDN | mKST) ;		// Remember key down

		    ret |= key_down( kp - (keys - 1) ) ;
		}
		else
		if ( b == (mKUP | mKST) )	// Is up, was down
		{
		    b = (mKUP & ~mKST) ;	// Remember key up

		    ret |= key_up( kp - (keys - 1) ) ;
		}

		*kp = b ;		// Store key status & debounce bits
	    }

	    ++kp ;
	    cb >>= 1 ;
	}				// for ( NCOLS )

      #endif				// __ALTDEB

    }					// for ( NROWS )

  #if defined(__MACROS)
    if ( ret & 0xF0 )
    {					// Initialize macro playback
	ret = play_macro( (ret & 0xF0) >> 4 ) ;

	macro = ret & M_ACTIVE ;
	ret  &= 0x0F ;
    }
  #endif

    return ( ret ) ;
}

//------------------------------------------------------------------------------
// Initialization
//------------------------------------------------------------------------------

// Enter suspend

void susp_enter ( void )
{
    // Disconnect LED used as sleep LED from PWM timer

  #if   bLED_SLP == bLED_0
    clr_bits( TCCR1A, _BV(COM1A1) | _BV(COM1A0) ) ;
  #elif bLED_SLP == bLED_1
    clr_bits( TCCR1A, _BV(COM1B1) | _BV(COM1B0) ) ;
  #elif bLED_SLP == bLED_2
    clr_bits( TCCR1A, _BV(COM1C1) | _BV(COM1C0) ) ;
  #elif bLED_SLP == bLED_3
    clr_bits( TCCR0A, _BV(COM0B1) | _BV(COM0B0) ) ;
  #endif

    set_bit( PRR0, PRTIM1 ) ;		// Take T1 offline
}

//------------------------------------------------------------------------------

// Exit suspend

void susp_exit ( void )
{
    clr_bit( PRR0, PRTIM1 ) ;		// Get T1 back online

    // Connect LED used as sleep LED  to PWM timer

  #if   bLED_SLP == bLED_0
    set_bits( TCCR1A, _B1(COM1A1) | _B0(COM1A0) ) ;
  #elif bLED_SLP == bLED_1
    set_bits( TCCR1A, _B1(COM1B1) | _B0(COM1B0) ) ;
  #elif bLED_SLP == bLED_2
    set_bits( TCCR1A, _B1(COM1C1) | _B0(COM1C0) ) ;
  #elif bLED_SLP == bLED_3
    set_bits( TCCR0A, _B1(COM0B1) | _B0(COM0B0) ) ;
  #endif
}

//------------------------------------------------------------------------------

// Set up alarm timer

void set_alarm ( void )
{
    ResetTM( 0, 0 ) ;			// Reset T0
    ResetOCRFA( 0 ) ;			// Clear T0 Output Compare A match Flag
    set_bit( TIMSK0, OCIE0A ) ;		// Enable T0 COMPA IRQ
}

//------------------------------------------------------------------------------

// Alarm interrupt handler

ISR( TIMER0_COMPA_vect )
{
    alarm = TRUE ;			// Signal that 500us are up
}

//------------------------------------------------------------------------------

// Delay using T0 (8bit) as timing reference

static void Delay_T0 ( uint8_t time )
{
    set_bit( GTCCR, PSRSYNC ) ;		// reset prescaler

    ResetTM( 0, time ) ;		// set timer

    for ( ; ! TMexp( 0 ) ; )		// wait until timer expired
	;
}

//------------------------------------------------------------------------------

// Initialize the hardware

void hw_init ( void )
{
    uint8_t
	i ;

    // Power down..

    ACSR  = _BV(ACD) ;			// analog comp
    PRR0  =				// T0/1, SPI
//	    _BV(PRTIM0) |		// We need T0
//	    _BV(PRTIM1) |		// T1 runs the LED PWM
	    _BV(PRSPI) ;
    PRR1  =				// USART1
	    _BV(PRUSART1) ;

    clock_prescale_set( clock_div_1 ) ;	// Set clock divider to 1, full speed

    // Initialize ports

    DDRB  = DDB ; PORTB = PBPU ;
    DDRC  = DDC ; PORTC = PCPU ;
    DDRD  = DDD ; PORTD = PDPU ;

    SetTMPS( 0, 1024 ) ;		// Set T0 prescaler to / 1024 for ms delay

    for ( i = 6 ; i-- ; )		// Wait ~100ms to settle in
	Delay_T0( (uint8_t)-MS2TM( 16, 1024 ) ) ;

    // T0 is going to be used for a 500us timebase

    SetTMPS( 0, 64 ) ;			// Set prescaler to / 64

    OCR0A = US2TM( 500, 64 ) ;		// Set output compare reg A to 500us

  #if defined(LED_3)			// 4th LED, connected to OC0B
    TCCR0A = _B1(COM0B1) | _B0(COM0B0) |	// Clr OC0B on comp. match, set at TOP
	     _B1( WGM01) | _B1( WGM00) ;
    set_bit( TCCR0B, WGM02 ) ;			// Fast PWM 8 w/ TOP = OCRA
  #else
    set_bit( TCCR0A, WGM01 ) ;		// Set CTC mode
  #endif

    // T1 is going to be used for LED PWM. 16 Mhz / 8 / 256 --> 7.8125 kHz cycle

    // OC1A on PC6, OC1B on PC5, OC1C on PB7

    TCNT1 = 0 ;				// Set TEMP (T1 high byte) to 0

    set_leds( mLEDS, LED_LVL_DEF ) ;	// LED's on until USB activates

    SetTMPS( 1, 8 ) ;			// Set T1 prescaler to / 8

    TCCR1A = _B1(COM1A1) | _B0(COM1A0) |	// Clr OC1A on comp. match, set at TOP
	     _B1(COM1B1) | _B0(COM1B0) |	// Clr OC1B on comp. match, set at TOP
	#if defined(LED_2)
	     _B1(COM1C1) | _B0(COM1C0) |	// Clr OC1C on comp. match, set at TOP
	#endif
	     _B0( WGM11) | _B1( WGM10) ;	// Fast PWM 8 bit
    set_bit( TCCR1B, WGM12 ) ;

    led_lvl  = LED_LVL_DEF ;
    fDemoOff = TRUE ;
}

//------------------------------------------------------------------------------
// End of kb.c
