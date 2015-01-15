/*******************************************************************************
 * File Name	: kb.h
 * Project	: Six Shooter
 * Date		: 2015/01/01
 * Version	: 1.0
 * Target MCU   : ATMEGA32U4 (Teensy 2.0)
 * Tool Chain	: Atmel AVR Studio 4.19 730, avr-gcc, avr-libc
 * Author	: "Detlef Mueller" <detlef@gmail.com>
 * Release Notes:
 *
 * $Id$
 ******************************************************************************/

#ifndef	__kb_h__
#define	__kb_h__

//-------------------------------------------------------------------------------
// Hardware specific definitions

#define	LEDPORT		PORTD
#define	LEDBIT		PD6

#define LED_on()	set_bit( LEDPORT, LEDBIT )
#define LED_off()	clr_bit( LEDPORT, LEDBIT )
#define LED_sts()	bit_is_set( LEDPORT, LEDBIT )

#define	LED_tog()	tog_bit( LEDPORT, LEDBIT )

//-------------------------------------------------------------------------------
// kb.c interface

#define	SIG_RESET		TRUE	/* Signal reset to KB routines */
#define	SIG_MAINT		FALSE	/* Signal maintain to KB routines */

//-------------------------------------------------------------------------------

#define REP0_CHG		0x01	/* Report Changed flags */
#define REP1_CHG		0x02
#define REP2_CHG		0x04

//-------------------------------------------------------------------------------
// kbmain.c interface
//-------------------------------------------------------------------------------

#define	sLED_on()		LED_on()
#define	sLED_off()		LED_off()

#define led_pwm()			/* Don't need it w/ timer PWM */

//-------------------------------------------------------------------------------

extern volatile uint8_t
    alarm ;				// Alarm signal, set every 500us

//-------------------------------------------------------------------------------

extern uint8_t
    wakeup( uint8_t reset ),		// Check for wakeup condition during suspend
    read_matrix( uint8_t reset ) ;	// Read and maintain key matrix

extern void
    susp_enter( void ),			// Enter suspend
    susp_exit( void ),			// Exit suspend
    sleep_led( uint8_t reset ),		// Maintain Sleep LED
    maint_leds( uint8_t reset ),	// Maintain LED's
    set_alarm( void ),			// Set up alarm
    hw_init( void ) ;			// Initialize HW

//-------------------------------------------------------------------------------

// Timer codes

 #if defined(__TIMER)
#define U_TOff			0xA0	/* Timer off */
#define U_1mn			0xA1	/* Timer on, add 1min */
#define U_5mn			0xA2	/* Timer on, add 5min */
#define U_10mn			0xA3	/* Timer on, add 10min */
#define U_30mn			0xA4	/* Timer on, add 30min */
#define U_60mn			0xA5	/* Timer on, add 60min */
 #else
#define U_TOff			U_None	/* Timer off */
#define U_1mn			U_None	/* Timer on, add 1min */
#define U_5mn			U_None	/* Timer on, add 5min */
#define U_10mn			U_None	/* Timer on, add 10min */
#define U_30mn			U_None	/* Timer on, add 30min */
#define U_60mn			U_None	/* Timer on, add 60min */
 #endif

//-------------------------------------------------------------------------------

#define KEYMAP(		\
	K2, K1, K0,	\
	K5, K4, K3 )	\
	{ K4, K5, K1, K2, K3, K0 }

//-------------------------------------------------------------------------------

 #ifdef	__macro__

//-------------------------------------------------------------------------------

#define MACRO_1()							\
	{								\
	    U_I, U_F, U_Space,						\
	    U_LSft, U_9, U_Space, U_None, U_Space, U_LSft, U_0, U_Enter,\
	    U_LSft, U_LBrc, U_Enter,					\
	    U_LSft, U_RBrc,						\
	    U_UArr, U_None, U_UArr, U_None,				\
	    U_RArr, U_None, U_RArr, U_None,				\
	    U_RArr, U_None, U_RArr, U_None				\
	}

#define MACRO_2()							\
	{								\
	    U_Enter, U_E, U_L, U_S, U_E, U_Enter,			\
	    U_LSft, U_LBrc, U_Enter,					\
	    U_LSft, U_RBrc,						\
	    U_UArr, U_Enter, U_None					\
	}

#define MACRO_3()							\
	{								\
	    U_F, U_O, U_R, U_Space,					\
	    U_LSft, U_9, U_Space, U_SemiCol, U_None, U_SemiCol,		\
	    U_Space, U_LSft, U_0, U_Enter,				\
	    U_LSft, U_LBrc, U_Enter,					\
	    U_LSft, U_RBrc,						\
	    U_UArr, U_None, U_UArr, U_None,				\
	    U_RArr, U_None, U_RArr, U_None, U_RArr, U_None,		\
	    U_RArr, U_None, U_RArr, U_None, U_RArr, U_None		\
	}

#define MACRO_4()							\
	{								\
	    U_D, U_O, U_Enter,						\
	    U_LSft, U_LBrc, U_Enter,					\
	    U_LSft, U_RBrc, U_Enter,					\
	    U_W, U_H, U_I, U_L, U_E, U_Space,				\
	    U_LSft, U_9, U_Space, U_None, U_Space, U_LSft, U_0, U_Space,\
	    U_SemiCol,							\
	    U_LArr, U_None, U_LArr, U_None, U_LArr, U_None, U_LArr,	\
	    U_None							\
	}

#define MACRO_5()							\
	{								\
	    uCC(U_AC_Home), uCC(U_None)					\
	    /* , uDms(3000), */						\
	    /* U_LCtr, U_LAlt, U_Del, */				\
	    /* U_None */						\
	}

#define MACRO_6()							\
	{								\
	    U_LGui, U_L, U_None						\
	}

//-------------------------------------------------------------------------------

 #endif // __macro__

//-------------------------------------------------------------------------------

#endif	// __kb_h__
