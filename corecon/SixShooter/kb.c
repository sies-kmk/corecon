/*******************************************************************************
 * File Name	: kb.c
 * Project	: Six Shooter
 * Date		: 2015/01/01
 * Version	: 1.0
 * Target MCU	: ATMEGA32U4 (Teensy 2.0)
 * Tool Chain	: Atmel AVR Studio 4.19 730, avr-gcc, avr-libc
 * Author	: "Detlef Mueller" <detlef@gmail.com>
 * References	:
 * Description	: Firmware for the CM Storm Demo Tester w/ Six Shooter PCB
 * Device	: CM Storm SGK-1010-BBCA1
 *
 * Release Notes:
 *
 * Pinout ATMEGAXU4
 * ================
 *
 * PB0 --                        PD0 D0                        PF0 S3
 * PB1 --                        PD1 --                        PF1 S0
 * PB2 --                        PD2 --         PE2 1k GND
 * PB3 --                        PD3 --
 * PB4 --                     	 PD4 --                        PF4 S4
 * PB5 D5                        PD5 --                        PF5 S5
 * PB6 D2         PC6 --         PD6 System LED PE6 --         PF6 S1
 * PB7 D3         PC7 D1         PD7 D4                        PF7 S2
 *
 * ,-----------.
 * | 2 | 1 | 0 |
 * |-----------|
 * | 5 | 4 | 3 |
 * `-----------'
 *
 * $Id$
 ******************************************************************************/

#define __TIMER

#include <includes.h>
#include "usb_cfg.h"
#include <u_des_kb.h>
#include <u_hid_kb.h>
#include <kbusages.h>
#include <kbmacro.h>

#include "kb.h"

//------------------------------------------------------------------------------
//******************************************************************************
//------------------------------------------------------------------------------

// Port setup

// Port B

#define PBPU		(_B0( PB7) | _B0( PB6) | _B0( PB5) | PU1( PB4) | \
			 PU1( PB3) | PU1( PB2) | PU1( PB1) | PU1( PB0))
#define DDB		(DDI(DDB7) | DDI(DDB6) | DDI(DDB5) | DDI(DDB4) | \
			 DDI(DDB3) | DDI(DDB2) | DDI(DDB1) | DDI(DDB0))

// Port C

#define PCPU		(_B0( PC7) | PU1( PC6))
#define DDC		(DDI(DDC7) | DDI(DDC6))

// Port D

#define PDPU		(_B0( PD7) | _B1( PD6) | PU1( PD5) | PU1( PD4) | \
			 PU1( PD3) | PU1( PD2) | PU1( PD1) | _B0( PD0))
#define DDD		(DDI(DDD7) | DDO(DDD6) | DDI(DDD5) | DDI(DDD4) | \
			 DDI(DDD3) | DDI(DDD2) | DDI(DDD1) | DDI(DDD0))

// Port E

#define PEPU		(PU1( PE6) | PU0( PE2))
#define DDE		(DDI(DDE6) | DDI(DDE2))

// Port F

#define PFPU		(PU1( PF7) | PU1( PF6) | PU1( PF5) | PU1( PF4) | \
						 PU1( PF1) | PU1( PF0))
#define DDF		(DDI(DDF7) | DDI(DDF6) | DDI(DDF5) | DDI(DDF4) | \
						 DDI(DDF1) | DDI(DDF0))

//------------------------------------------------------------------------------
// 6% Key & LED definitions

#define	KEY_IN_P		PINF

// Number of keys on the board, 6 on the CM STORM Keyboard Demo, each w/ a LED

#define	KEY_COUNT		6
#define	LED_COUNT		6

// Key indices and masks

#define bKEY_3			PF0
#define bKEY_0			PF1
#define bKEY_4			PF4
#define bKEY_5			PF5
#define bKEY_1			PF6
#define bKEY_2			PF7

#define	mKEYS_P			0b11110011
#define	mKEYS			0b00111111

// After swapping the KEY_IN_P the switch order is 032154

//------------------------------------------------------------------------------

// LED indices and masks

#define bLED_5			0
#define bLED_4			1
#define bLED_3			2
#define bLED_2			3
#define bLED_1			4
#define bLED_0			5

#define	LED_SLP			bLED_2

#define mLED_5			_BV(bLED_5)		/* PB5 OC1A */
#define mLED_4			_BV(bLED_4)		/* PD7 OC4D */
#define mLED_3			_BV(bLED_3)		/* PB7 OC0A */
#define mLED_2			_BV(bLED_2)		/* PB6 OC1B */
#define mLED_1			_BV(bLED_1)		/* PC7 OC4A */
#define mLED_0			_BV(bLED_0)		/* PD0 OC0B */

#if LED_SLP == bLED_0
 #define pLED_SLP		PORTD
 #define bLED_SLP		PD0
#elif LED_SLP == bLED_1
 #define pLED_SLP		PORTC
 #define bLED_SLP		PC7
#elif LED_SLP == bLED_2
 #define pLED_SLP		PORTB
 #define bLED_SLP		PB6
#elif LED_SLP == bLED_3
 #define pLED_SLP		PORTB
 #define bLED_SLP		PB7
#elif LED_SLP == bLED_4
 #define pLED_SLP		PORTD
 #define bLED_SLP		PD7
#elif LED_SLP == bLED_5
 #define pLED_SLP		PORTB
 #define bLED_SLP		PB5
#endif

#define mLEDS			(mLED_5 | mLED_4 | mLED_3 | \
				 mLED_2 | mLED_1 | mLED_0)

//------------------------------------------------------------------------------

// Bit definitions for the keyboard LED OUT report

#define	bLED_NUM		0
#define	bLED_CAPS		1
#define	bLED_SCR		2
#define	bLED_COMPOSE		3
#define	bLED_KANA		4

// Bit definitions for the keyboard modifiers in the IN report

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
    alarm ;

//------------------------------------------------------------------------------

#if defined(__TIMER)
 static uint8_t
    tm_on,				// Timer on/off flag
    tm_min ;				// Minutes until alarm

 static uint16_t
    tm_ms ;				// msec count down
#endif

//------------------------------------------------------------------------------
// Misc. helper functions
//------------------------------------------------------------------------------

// Reboot into bootloader.

static void FA_NORETURN( bootloader ) ( void )
{
    TIMSK3 = 0 ;			// Kill timer interrupt

    UDCON  = _B1(DETACH) ;		// Detach from USB

    cli() ;				// Disable interrupts

    wdt_disable() ;			// Leash watchdog

    USBCON = _B0(USBE) | _B1(FRZCLK) ;	// Stop USB module

    clr_bit( PLLCSR, PLLE ) ;		// Stop PLL

    // Disable peripherals

    DDRB   = 0 ; PORTB  = 0 ;
    DDRC   = 0 ; PORTC  = 0 ;
    DDRD   = 0 ; PORTD  = 0 ;
    DDRE   = 0 ; PORTE  = 0 ;
    DDRF   = 0 ; PORTF  = 0 ;

    TCCR0A = 0 ; TCCR0B = 0 ;
    TCCR1A = 0 ; TCCR1B = 0 ;
    TCCR3A = 0 ; TCCR3B = 0 ;
    TCCR4A = 0 ; TCCR4B = 0 ; TCCR4C = 0 ;

//    OCR0A  = 0 ; OCR0B  = 0 ;
//    OCR1A  = 0 ; OCR1B  = 0 ;
//    OCR3A  = 0 ;
//    OCR4A  = 0 ; OCR4C  = 0 ; OCR4D  = 0 ;

//    TCNT0  = 0 ; TCNT1  = 0 ;
//    TCNT3  = 0 ; TCNT4  = 0 ;

//    TIFR1  = 0 ;

//    PRR0   = 0 ; PRR1   = 0   ;

    // Wait 15ms to make sure detach is recognized

    _delay_loop_2( (uint16_t)MS2TM( 15, 4 ) ) ;

    jmp_bootloader() ;			// Off we go
}

//------------------------------------------------------------------------------
// LED related functions
//------------------------------------------------------------------------------

// Maintain Sleep LED. Note: called every 1ms (CPU clock throttled)

#define	SLEEP_OFF_DELAY		1				/* 1ms on */
#define	SLEEP_ON_DELAY		(3000 - SLEEP_OFF_DELAY)	/* once every 3s */

void sleep_led ( uint8_t reset )
{
    static uint16_t
	timer ;

    if ( reset )
    {
	clr_bit( pLED_SLP, bLED_SLP ) ;		// LED off

	timer = 0 ;

	return ;
    }

    if ( ! timer-- )				// Timer expired
    {
	if ( bit_is_set( pLED_SLP, bLED_SLP ) )	// LED on ?
	{
	    clr_bit( pLED_SLP, bLED_SLP ) ;	// LED off
	    timer = SLEEP_ON_DELAY ;
	}
	else
	{
	    set_bit( pLED_SLP, bLED_SLP ) ;	// LED on
	    timer = SLEEP_OFF_DELAY ;
	}
    }
}

//------------------------------------------------------------------------------

static void set_leds_off ( uint8_t leds )
{
    if ( leds & mLED_0 )
	OCR0B = 255 ;

    if ( leds & mLED_1 )
	OCR4A = 255 ;

    if ( leds & mLED_2 )
	OCR1BL = 255 ;

    if ( leds & mLED_3 )
	OCR0A = 255 ;

    if ( leds & mLED_4 )
	OCR4D = 255 ;

    if ( leds & mLED_5 )
	OCR1AL = 255 ;
}

//------------------------------------------------------------------------------

static void set_leds_raw ( uint8_t leds, uint8_t lvl )
{
    if ( leds & mLED_0 )
	OCR0B = lvl ;

    if ( leds & mLED_1 )
	OCR4A = lvl ;

    if ( leds & mLED_2 )
	OCR1BL = lvl ;

    if ( leds & mLED_3 )
	OCR0A = lvl ;

    if ( leds & mLED_4 )
	OCR4D = lvl ;

    if ( leds & mLED_5 )
	OCR1AL = lvl ;
}

//------------------------------------------------------------------------------

// Set dim level for multiple LEDs. 0: off, 100: full on

#define _x( n )		(255 - ((n) <= 80 ? (n) * 2 : 155 + ((n) - 80) * 4 + ((n) - 80)))

static void set_leds ( uint8_t leds, uint8_t lvl )
{
    set_leds_raw( leds, _x( lvl ) ) ;
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
	lev ;				// Index into breathe array

    uint8_t
	i ;

    if ( reset )
    {
	timer = 0 ;
	lev   = 0 ;

	return ;
    }

    if ( ! timer-- )
    {
	timer = DEMO_TIMER ;

	for ( i = 0 ; i < LED_COUNT ; ++i )
	    set_leds_raw( _bv8( i ), pgm_read_byte( breathe + ((lev + i * 5) % ARRSZ( breathe )) ) ) ;

	lev = (lev + 1) % ARRSZ( breathe ) ;
    }
}

//------------------------------------------------------------------------------

// Maintain LEDs

#define	DEMO_DELAY		3000		/* Start demo after 3s w/o lock LEDs */
#define	BOOT_DELAY		10000		/* Start boot delay, 10s */
#define	LED_TIMER		1		/* 500us */

void maint_leds ( uint8_t reset )
{
    static uint16_t
      #if defined(__TIMER)
	al_pat,				// Alarm pattern
      #endif
	bload,				// Bootloader call timer
	demo ;				// Demo start timer

    static uint8_t
      #if defined(__TIMER)
	kb_leds,
      #endif
	timer,
	dir_f,
	dim_lvl ;

    uint8_t
	mLeds ;

    if ( reset )
    {
	bload   = BOOT_DELAY ;
	demo    = DEMO_DELAY ;
	timer   = 0 ;
	dir_f   = 0 ;
	dim_lvl = 0 ;

      #if defined(__TIMER)
	al_pat = 0b000000010101u ;	// 50ms per bit, three flashes per cycle
      #endif

	set_leds_off( mLEDS ) ;

	_delay_loop_2( 512 ) ;		// let timers elapse, 512 * 4 clocks

	return ;
    }

    if ( ! timer-- )			// Maintain LED's every .5ms * timer
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

		    if ( al_pat & 0x001 )
			set_leds_raw( mLED_5 | mLED_1 | mLED_3, _x(100) ) ;

		    if ( al_pat & 0x100 )
			set_leds_raw( mLED_2 | mLED_4 | mLED_0, _x(100) ) ;

		    al_pat = ror16( al_pat ) ;	// 16bit * 25ms, 400ms
		}
		else
		if ( al_tim == 5 )	// Flashes are 20ms on, 30ms off
		    set_leds_off( mLEDS ) ;

		if ( kb_leds != kbd_leds )
		    tm_on = 0 ;

		return ;
	    }
	    else			// Timer not expired yet
	    {
		if ( ! --tm_ms )	// 1min expired
		{			// Reset ms timer, count down minutes
		    tm_ms = MS_PER_MIN ;

		    if ( ! --tm_min )
			kb_leds = kbd_leds ;
		}
	    }
	}
      #endif

	if ( kbd_leds & (_BV(bLED_NUM) | _BV(bLED_CAPS) | _BV(bLED_SCR)) )
	{					// LOCK LED(s) active
	    if ( demo != DEMO_DELAY )		// Demo mode was possibly on
	    {
		demo  = DEMO_DELAY ;		// Reset demo timer

		dir_f   = 0 ;
		dim_lvl = 0 ;
	    }

	    if ( kbd_leds == (_BV(bLED_SCR) | _BV(bLED_NUM) | _BV(bLED_CAPS)) )
	    {
		if ( ! --bload )		// All LED's on
		    bootloader() ;
	    }
	    else
		bload = BOOT_DELAY ;

	    if ( dir_f )
		--dim_lvl ;

	    mLeds = 0 ;

	    if ( kbd_leds & _BV(bLED_NUM) )
		mLeds |= mLED_0 ;

	    if ( kbd_leds & _BV(bLED_CAPS) )
		mLeds |= (mLED_1 | mLED_5 | mLED_4 | mLED_3) ;

	    if ( kbd_leds & _BV(bLED_SCR) )
		mLeds |= mLED_2 ;

	    set_leds( mLeds, dim_lvl ) ;
	    set_leds_off( mLeds ^ mLEDS ) ;

	    if ( dir_f )
	    {
		if ( ! dim_lvl )
		{
		    dir_f = 0 ;
		}
	    }
	    else
	    {
		if ( ++dim_lvl > 100 )
		{
		    dim_lvl -= 2 ;
		    dir_f = 1 ;
		}
	    }
	}
	else				// No LOCK LED active
	{
	    if ( demo )			// Waiting for demo display
	    {
		if ( demo == DEMO_DELAY )	// First run after LEDs off
		    set_leds_off( mLEDS ) ;	// Set all LEDs off

		if ( ! --demo )			// Demo enable on next run
		    led_demo( SIG_RESET ) ;	// Reset demo routine
	    }
	    else
		led_demo( SIG_MAINT ) ;
	}
    }
}

//------------------------------------------------------------------------------
// Key related functions
//------------------------------------------------------------------------------

// Check for any key down in sleep mode

uint8_t wakeup ( uint8_t reset )
{
    static uint8_t
	db ;				// Debounce bits

    if ( reset )
    {
	db = 0 ;

	return ( FALSE ) ;
    }

    db = (db << 1) | ((KEY_IN_P & mKEYS_P) != mKEYS_P) ;

    return ( db == 0x7F ) ;
}

//------------------------------------------------------------------------------

// Build the keyboard report

static uint8_t build_kbd_report ( uint8_t keys )
{
  #define U_Macro_1	0x0010
  #define U_Macro_2	0x0020
  #define U_Macro_3	0x0030
  #define U_Macro_4	0x0040
  #define U_Macro_5	0x0050
  #define U_Macro_6	0x0060

    static const uint16_t
	key_usages[KEY_COUNT] PROGMEM =	// Key-to-usage matrix
	  KEYMAP(
	    uKB(U_LftCtrl),	uKB(U_UpArrow),		uKB(U_NumLock),
	    uKB(U_LftArrow),	uKB(U_DwnArrow),	uKB(U_RgtArrow) ),
	key_usagesN[KEY_COUNT] PROGMEM = // Key-to-usage matrix if NUM lock is set
	 #if defined(__TIMER)
	  KEYMAP(
	    uKB(U_TOff),	uKB(U_30mn),		uKB(U_NumLock),
	    uKB(U_10mn),	uKB(U_5mn),		uKB(U_1mn) ),
	 #else
	  KEYMAP(
	    uKB(U_ScrollLock),	uM(U_Macro_4),		uKB(U_NumLock),
	    uM(U_Macro_1),	uM(U_Macro_3),		uM(U_Macro_2) ),
	 #endif
	key_usagesS[KEY_COUNT] PROGMEM = // Key-to-usage matrix if SCR lock is set
	  KEYMAP(
	    uKB(U_ScrollLock),	uCC(U_VolUp),		uCC(U_PlayPause),
	    uCC(U_PrevTrack),	uCC(U_VolDwn),		uCC(U_NextTrack) ),
	key_usagesC[KEY_COUNT] PROGMEM = // Key-to-usage matrix if CAPS lock is set
	  KEYMAP(
	    uKB(U_CapsLock),	uKB(U_PgUp),		uKB(U_NumMinus),
	    uKB(U_Home),	uKB(U_PgDn),		uKB(U_Enter) ) ;

    static kb_report_t
	new_kbd_rep ;

    static uint16_t
	new_ctrl_rep1,
	new_ctrl_rep2 ;

    uint16_t
	usage ;

    uint8_t
	i,
	ki = 0,
	ret = 0 ;

    const uint16_t
	*pu ;

    CRITICAL_VAR() ;

    if ( kbd_leds == _BV(bLED_NUM) )
	pu = key_usagesN ;
    else
    if ( kbd_leds == _BV(bLED_SCR) )
	pu = key_usagesS ;
    else
    if ( kbd_leds == _BV(bLED_CAPS) )
	pu = key_usagesC ;
    else
	pu = key_usages ;

    new_ctrl_rep1     = U_None ;
    new_ctrl_rep2     = U_None ;
    new_kbd_rep.mod   = 0 ;		// Clear modifiers

    for ( i = 0 ; i < KEY_COUNT ; ++i, keys >>= 1 )
    {						// Assemble keyboard reports
	if ( (keys & 1) )			// Key down
	{
	    usage = pgm_read_word( pu + i ) ;	// get its usage

	    if ( (usage & REP_M) )		// Macro
		ret |= (usage & 0x00F0) ;
	    else
	    if ( (usage & REP_SC) )		// System Control usage
		new_ctrl_rep1 = (usage & 0x0FFF) ;
	    else
	    if ( (usage & REP_CC) )		// Consumer Control usage
		new_ctrl_rep2 = (usage & 0x0FFF) ;
	    else				// Keyboard usage
	    {
		if ( (usage & 0xF0) == 0xE0 )	// Modifier
		    new_kbd_rep.mod |= _bv8( usage & 0x07 ) ;	// Magic.
		else
	      #if defined(__TIMER)
		if ( (usage & 0xF0) == 0xA0 )	// Timer
		{
		    switch ( usage )
		    {
			case U_TOff :		/* Timer off/fire */
			    if ( tm_on )
			    {
				tm_min = 0 ;
				tm_on  = FALSE ;
			    }
			    else
			    {
				tm_min = 1 ;
				tm_ms  = 1 ;
				tm_on  = TRUE ;
			    }

			    break ;

			case U_1mn :		/* Timer on, add 1min */
			    tm_min +=  1 ; goto Set_timer ;
			case U_5mn :		/* Timer on, add 5min */
			    tm_min +=  5 ; goto Set_timer ;
			case U_10mn :		/* Timer on, add 10min */
			    tm_min += 10 ; goto Set_timer ;
			case U_30mn :		/* Timer on, add 30min */
			    tm_min += 30 ; goto Set_timer ;

			Set_timer :
			    if ( ! tm_on )
			    {
				tm_on = TRUE ;
				tm_ms = MS_PER_MIN ;
			    }
		    }
		}
		else
	      #endif
		    new_kbd_rep.keys[ki++] = (usage & 0xFF) ;
	    }
	}
    }

    // Note: assuming NUM_KEYS <= ARRSZ( kbd_report.keys )

    for ( ; ki < ARRSZ( new_kbd_rep.keys ) ; )
	new_kbd_rep.keys[ki++] = U_None ;

    ENTER_CRITICAL() ;

    // Check if a report changed

    if ( new_kbd_rep.mod != kbd_report.mod )
    {					// Mod's in report 0 changed
	kbd_report.mod = new_kbd_rep.mod ;
	ret |= REP0_CHG ;
    }

    for ( ki = ARRSZ( kbd_report.keys ) ; ki-- ; )
	if ( kbd_report.keys[ki] != new_kbd_rep.keys[ki] )
	{				// Key in report 0 changed
	    kbd_report.keys[ki] = new_kbd_rep.keys[ki] ;
	    ret |= REP0_CHG ;
	}

    if ( new_ctrl_rep1 != ctrl_report1.key )
    {					// Report 1 changed
	ctrl_report1.key = new_ctrl_rep1 ;
	ret |= REP1_CHG ;
    }

    if ( new_ctrl_rep2 != ctrl_report2.key )
    {					// Report 2 changed
	ctrl_report2.key = new_ctrl_rep2 ;
	ret |= REP2_CHG ;
    }

    EXIT_CRITICAL_RET( ret ) ;
}

//------------------------------------------------------------------------------

// Read and maintain key matrix

#define mKDN			0b00111111	/* 3ms stable */
#define mKUP			0b01000000

uint8_t read_matrix ( uint8_t reset )
{
    static uint8_t
	macro,				// Flag to signal macro playback in progress
	keys ;				// Current key status

    static int8_t
	db_c[KEY_COUNT] ;		// Debounce counters

    uint8_t
	i,
	ret = FALSE,
	key,
	key_msk ;

    int8_t
	*db = db_c ;

    if ( reset )
    {
	for ( i = KEY_COUNT ; i-- ; ++db )	// Clear debounce counters
	    *db = 0 ;

	keys  = 0 ;				// All keys off
	macro = 0 ;

	return ( build_kbd_report( keys ) ) ;	// Clear report
    }

    if ( macro )				// Playing macro
    {
	ret = play_macro( 0 ) ;

	macro = ret & M_ACTIVE ;
	ret  &= 0x0F ;

	return ( ret ) ;
    }

    asm volatile
    (
	"in     %0,%[keyp]" "\n\t"		// Active inputs: 0b11110011
	"swap   %0"         "\n\t"
	: "=r" (key)
	: [keyp] "I" _SFR_IO_ADDR(KEY_IN_P)
    ) ;						// now 0b00111111

    for ( key_msk = 1, i = KEY_COUNT ; i-- ; ++db, key_msk <<= 1 )
    {
	*db = ((*db & mKDN) << 1) | ! (key & key_msk) ;

	if ( *db == mKUP &&   (keys & key_msk) )
	{
	    keys &= ~key_msk ;
	    ret   = TRUE ;
	}
	else
	if ( *db == mKDN && ! (keys & key_msk) )
	{
	    keys |= key_msk ;
	    ret   = TRUE ;
	}
    }

    if ( ret )				// Key(s) changed
	if ( (ret = build_kbd_report( keys )) & 0xF0 )
	{				// Initialize macro playback
	    ret = play_macro( (ret & 0xF0) >> 4 ) ;

	    macro = ret & M_ACTIVE ;
	    ret  &= 0x0F ;
	}

    return ( ret ) ;
}

//------------------------------------------------------------------------------
// Initialization
//------------------------------------------------------------------------------

// Enter suspend

void susp_enter ( void )
{
   #if LED_SLP == bLED_0			// Disconnect OC0B, LED 0
    clr_bits( TCCR0A, _BV(COM0B1) | _BV(COM0B0) ) ;
   #elif LED_SLP == bLED_1			// Disconnect OC4A, LED 1
    clr_bits( TCCR4A, _BV(COM4A1) | _BV(COM4A0) ) ;
   #elif LED_SLP == bLED_2			// Disconnect OC1B, LED 2
    clr_bits( TCCR1A, _BV(COM1B1) | _BV(COM1B0) ) ;
   #elif LED_SLP == bLED_3			// Disconnect OC0A, LED 3
    clr_bits( TCCR0A, _BV(COM0A1) | _BV(COM0A0) ) ;
   #elif LED_SLP == bLED_4			// Disconnect OC4D, LED 4
    clr_bits( TCCR4C, _BV(COM4D1) | _BV(COM4D0) ) ;
   #else					// Disconnect OC1A, LED 5
    clr_bits( TCCR1A, _BV(COM1A1) | _BV(COM1A0) ) ;
   #endif

    // Take timers T0/1/4 offline

    set_bits( PRR0, _BV(PRTIM0) | _BV(PRTIM1) ) ;
    set_bit(  PRR1,     PRTIM4  ) ;
}

//------------------------------------------------------------------------------

// Exit suspend

void susp_exit ( void )
{
    // Get the timers back online

    clr_bits( PRR0, _BV(PRTIM0) | _BV(PRTIM1) ) ;
    clr_bit(  PRR1,     PRTIM4  ) ;

   #if LED_SLP == bLED_0			// Connect OC0B, LED 0
    set_bits( TCCR0A, _B1(COM0B1) | _B1(COM0B0) ) ;
   #elif LED_SLP == bLED_1			// Connect OC4A, LED 1
    set_bits( TCCR4A, _B1(COM4A1) | _B1(COM4A0) ) ;
   #elif LED_SLP == bLED_2			// Connect OC1B, LED 2
    set_bits( TCCR1A, _B1(COM1B1) | _B1(COM1B0) ) ;
   #elif LED_SLP == bLED_3			// Connect OC0A, LED 3
    set_bits( TCCR0A, _B1(COM0A1) | _B1(COM0A0) ) ;
   #elif LED_SLP == bLED_4			// Connect OC4D, LED 4
    set_bits( TCCR4C, _B1(COM4D1) | _B1(COM4D0) ) ;
   #else					// Connect OC1A, LED 5
    set_bits( TCCR1A, _B1(COM1A1) | _B1(COM1A0) ) ;
   #endif
}

//------------------------------------------------------------------------------

// Set up alarm timer

void set_alarm ( void )
{
    ResetTM( 3, 0 ) ;			// Reset T3
    ResetOCRFA( 3 ) ;			// Clear T3 Output Compare A match Flag
    set_bit( TIMSK3, OCIE3A ) ;		// Enable T3 COMPA IRQ
}

//------------------------------------------------------------------------------

// Alarm interrupt handler

ISR( TIMER3_COMPA_vect )
{
    alarm = TRUE ;			// Signal that 500us are up
}

//------------------------------------------------------------------------------

// Delay using T3 (16bit) as timing reference

static void Delay_T3 ( uint16_t time )
{
    set_bit( GTCCR, PSRSYNC ) ;		// reset prescaler

    ResetTM( 3, time ) ;		// set timer

    for ( ; ! TMexp( 3 ) ; )		// wait until timer expired
	;
}

//------------------------------------------------------------------------------

// Initialize the hardware

void hw_init ( void )
{
    // Power down..

    ACSR  = _BV(ACD) ;				// analog comp
    PRR0  =					// TWI, T0/1, SPI, ADC
	    _BV(PRTWI)  |
//	    _BV(PRTIM0) |
//	    _BV(PRTIM1) |
	    _BV(PRSPI)  |
	    _BV(ADC) ;
    PRR1  =					// T3/4, USART1
//	    _BV(PRTIM4)  |
//	    _BV(PRTIM3 ) |
	    _BV(PRUSART1) ;
//  DIDR1 = _BV(AIN0D) ;			// Disable AIN0 digital input buffer

//  MCUCR = _BV(JTD) ;				// Disable JTAG interface, JIC
//  MCUCR = _BV(JTD) ;

    // Initialize ports

    DDRB  = DDB ; PORTB = PBPU ;
    DDRC  = DDC ; PORTC = PCPU ;
    DDRD  = DDD ; PORTD = PDPU ;
    DDRE  = DDE ; PORTE = PEPU ;
    DDRF  = DDF ; PORTF = PFPU ;

    clock_prescale_set( clock_div_1 ) ;		// Set clock divider to 1, full speed

    LED_off() ;

    // Initialize timers

    SetTMPS( 3, 1024 ) ;			// Set T3 prescaler to / 1024 for ms delay

    Delay_T3( -MS2TM( 200, 1024 ) ) ;		// Wait 200ms to settle in

    // T3 is going to be used for a 500us timebase

    SetTMPS( 3, 8 ) ;				// Set T3 prescaler to / 8

    OCR3A = US2TM( 500, 8 ) ;			// Set output compare reg to 500us
    set_bit( TCCR3B, WGM12 ) ;			// Set CTC mode

    TCNT1 = 0 ;					// Set TEMP (T high byte) to 0

    // Going to use T0, T1, and T4 for LED PWM. 16 Mhz / 8 / 256 --> 7.8125 kHz cycle

    set_leds_off( mLEDS ) ;			// Preset OC registers for LEDs off
    set_leds_raw( mLED_2, _x(30) ) ;		// Except LED 2, on until USB configured

    // T0, OC0A on PB7, OC0B on PD0

    SetTMPS( 0, 8 ) ;				// Set T0 prescaler to / 8

    TCCR0A = _B1(COM0A1) | _B1(COM0A0) |	// Set OC0A on comp. match, clr at TOP
	     _B1(COM0B1) | _B1(COM0B0) |	// Set OC0B on comp. match, clr at TOP
	     _B1( WGM01) | _B1( WGM00) ;	// Fast PWM

    set_bit( DDRB, DDB7 ) ;			// Set PB7 as OUT
    set_bit( DDRD, DDD0 ) ;			// Set PD0 as OUT

    // T1, OC1A on PB5, OC1B on PB6

    SetTMPS( 1, 8 ) ;				// Set T1 prescaler to / 8

    TCCR1A = _B1(COM1A1) | _B1(COM1A0) |	// Set OC1A on comp. match, clr at TOP
	     _B1(COM1B1) | _B1(COM1B0) |	// Set OC1B on comp. match, clr at TOP
	     _B0( WGM11) | _B1( WGM10) ;	// Fast PWM 8 bit
    set_bit( TCCR1B, WGM12 ) ;			// Fast PWM

    set_bit( DDRB, DDB5 ) ;			// Set PB5 as OUT
    set_bit( DDRB, DDB6 ) ;			// Set PB6 as OUT

    // T4, OC4A on PC7, OC4D on PD7

//    TC4H  = 0x00 ;
    OCR4C = 0xFF ;				// Set TOP, limits T4 to 8bit

    TCCR4A = _B1(COM4A1)  | _B1(COM4A0)  |	// Set OC4A on comp. match, clr at TOP
	     _B1(PWM4A)   | _B0(PWM4B) ;	// Enable OC4A
    TCCR4C = _B1(COM4A1S) | _B1(COM4A0S) |	// Set OC4A on comp. match, clr at TOP
	     _B1(COM4D1)  | _B1(COM4D0)  |	// Set OC4D on comp. match, clr at TOP
	     _B1(PWM4D) ;			// Enable OC4D PWM
//    TCCR4D = _B0( WGM41 ) | _B0( WGM40 ) ;	// Fast PWM

    // Set prescaler to / 8, also starts the timer

    TCCR4B = _B0(CS43) | _B1(CS42) | _B0(CS41) | _B0(CS40) ;

    set_bit( DDRC, DDC7 ) ;			// Set PC7 as OUT
    set_bit( DDRD, DDD7 ) ;			// Set PD7 as OUT
}

//------------------------------------------------------------------------------
// End of kb.c
