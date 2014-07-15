/*******************************************************************************
 * File Name	: main.c
 * Project	: Keyboard
 * Date		: 2014/03/11
 * Version	: 1.0
 * Target MCU   : AVR
 * Tool Chain	: Atmel AVR Studio 4.19 730, avr-gcc, avr-libc
 * Author	: "Detlef Mueller" <detlef@gmail.com>
 * References	:
 * Description	: Keyboard firmware main routine
 * Device	:
 *
 * Release Notes:
 *
 * $Id$
 ******************************************************************************/

#include "includes.h"
#include <usb_cfg.h>
#include "u_des_kb.h"
#include "u_hid_kb.h"
#include <kb.h>

//------------------------------------------------------------------------------
//******************************************************************************
//------------------------------------------------------------------------------

#define	IDLE_PERIOD_MS		4			/* 4ms per idle period */
#define	IDLE_PERIOD		(IDLE_PERIOD_MS * 2)

//------------------------------------------------------------------------------
//******************************************************************************
//------------------------------------------------------------------------------

// Watchdog is not turned off by a reset, see avr-libc's wdt.h documentation

// uint8_t VA_NOINIT( mcusr_cpy ) ;

void FA_INIT3( Init3 ) ( void )
{
//  mcusr_cpy = MCUSR ;			// If we need to examine reset reason
    MCUSR = 0 ;
    wdt_disable() ;
}

//------------------------------------------------------------------------------

int FA_NORETURN( main ) ( void )
{
    uint8_t
	suspended =  TRUE,		// Offline
	idle = IDLE_PERIOD,		// idle period timer
	kbd_sendrep = FALSE ;		// "need to send report" signal

    // Initialize..

    hw_init() ;				// hardware
    usb_init() ;			// USB stack

    // Initial idle rate for keyboards is 500ms

    kbd_idle_rate = 500 / IDLE_PERIOD_MS ;

    set_sleep_mode( SLEEP_MODE_IDLE ) ;	// Idle is the best we can do in this setup

    set_alarm() ;			// Set up alarm

    for ( ;; )				// Forever..
    {
	sleep_enable() ;		// Prepare for sleep

        for ( ; ! alarm ; )		// No alarm pending,
	    sleep_cpu() ;		// snooze.

        sleep_disable() ;

	// 500us are up, do house keeping

	alarm = FALSE ;			// reset alarm

	if ( usb_configured() )		// USB link up and running
	{
	    if ( suspended )		// Coming out of sleep or reset
	    {
		// Set clock divider to 1, full speed

		clock_prescale_set( clock_div_1 ) ;

		suspended = FALSE ;

		sleep_led( SIG_RESET ) ;	// Reset sleep LED

		susp_exit() ;			// Hardware specific stuff

		wdt_enable( WDTO_500MS ) ;	// Unleash watchdog

		read_matrix( SIG_RESET ) ;	// Reset key matrix
		maint_leds( SIG_RESET ) ;	// Reset LED system

		idle = IDLE_PERIOD ;
		kbd_sendrep  = FALSE ;		// Reset send report flag
		kbd_idle_cnt = kbd_idle_rate ;	// Reset idle counter

		continue ;
	    }

	    led_pwm() ;				// Maintain LED PWM

	    if ( ! idle-- )			// Maintain idle counters
	    {
		idle = IDLE_PERIOD ;
						// Idle counter expired, rate not indef.
		if ( ! --kbd_idle_cnt && kbd_idle_rate )
		    kbd_sendrep |= REP0_CHG ;	// need to send current report
	    }

	    maint_leds( SIG_MAINT ) ;		// Maintain LEDs

	    kbd_sendrep |=
		read_matrix( SIG_MAINT ) ;	// Read & maintain key matrix

	    if ( (kbd_sendrep & REP0_CHG) && ! usb_IN_busy( EP_HID_KBD ) )
	    {
		sLED_on() ;

	      #if USB_REPORT_SZ_KBD > 8
		if ( ! kbd_protocol )		// Boot protocol
		    usb_send_IN( VP( &kbd_report ), 8, EP_HID_KBD ) ;
		else
	      #endif
		    usb_send_IN( VP( &kbd_report ), sizeof( kbd_report ), EP_HID_KBD ) ;

		kbd_sendrep &= ~REP0_CHG ;	// reset send report flag
		kbd_idle_cnt = kbd_idle_rate ;	// reset idle counter

		sLED_off() ;
	    }

	  #if ENABLE_CTRL_KEYS
	    if ( (kbd_sendrep & REP1_CHG) && ! usb_IN_busy( EP_HID_CTRL ) )
	    {
		sLED_on() ;

		usb_send_IN( VP( &ctrl_report1 ), sizeof( ctrl_report1 ), EP_HID_CTRL ) ;

		kbd_sendrep &= ~REP1_CHG ;

		sLED_off() ;
	    }

	    if ( (kbd_sendrep & REP2_CHG) && ! usb_IN_busy( EP_HID_CTRL ) )
	    {
		sLED_on() ;

		usb_send_IN( VP( &ctrl_report2 ), sizeof( ctrl_report2 ), EP_HID_CTRL ) ;

		kbd_sendrep &= ~REP2_CHG ;

		sLED_off() ;
	    }
	  #endif

	    wdt_reset() ;		// Pet watchdog
	}
	else				// USB link down
	if ( usb_suspend )		// ..because we are suspended
	{
	    if ( ! suspended )		// Just got suspended
	    {
		suspended = TRUE ;

		maint_leds( SIG_RESET ) ;	// Reset LED system
		sleep_led( SIG_RESET ) ;	// Reset sleep LED
		wakeup( SIG_RESET ) ;		// Reset wakeup check

		wdt_disable() ;			// Leash watchdog

		susp_enter() ;			// Hardware specific stuff

		// Set clock divider to 2, half speed

		clock_prescale_set( clock_div_2 ) ;

		continue ;
	    }

	    sleep_led( SIG_MAINT ) ;	// Maintain sleep LED

	    if ( wakeup( SIG_MAINT ) )	// Check for wakeup
		usb_remote_wakeup() ;
	}
    }
}

//------------------------------------------------------------------------------
// End of main.c
