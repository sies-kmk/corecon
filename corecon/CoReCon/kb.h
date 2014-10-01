/*******************************************************************************
 * File Name	: kb.h
 * Project	: Costar Replacement Controllers
 * Date		: 2014/05/21
 * Version	: 1.0
 * Target MCU   : ATMEGA32U2
 * Tool Chain	: Atmel AVR Studio 4.19 730, avr-gcc, avr-libc
 * Author	: "Detlef Mueller" <detlef@gmail.com>
 * Release Notes:
 *
 * $Id$
 ******************************************************************************/

#ifndef	__kb_h__
#define	__kb_h__

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

#define	sLED_on()			/* We don't have a system LED */
#define	sLED_off()

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
 #if defined(__kb__)
//-------------------------------------------------------------------------------

// Special usages

#define U_SPDn			0x81	/* U_System_Power_Down */
#define U_SSlp			0x82	/* U_System_Sleep */
#define	U_SWUp			0x83	/* U_System_Wake_up */

// Timer codes

  #if defined(__TIMER)
#define U_TOff			0xA0	/* Timer off */
#define U_1mn			0xA1	/* Timer on, add 1min */
#define U_5mn			0xA2	/* Timer on, add 5min */
#define U_10mn			0xA3	/* Timer on, add 10min */
#define U_30mn			0xA4	/* Timer on, add 30min */
#define U_60mn			0xA5	/* Timer on, add 60min */
  #else
#define U_TOff			U_None
#define U_1mn			U_None
#define U_5mn			U_None
#define U_10mn			U_None
#define U_30mn			U_None
#define U_60mn			U_None
  #endif

#define	U_CC_Offs		0xC0	/* Consumer Control Offs */

#define	U_CFwd			0xC0	/* U_NextTrack */
#define	U_CBwd			0xC1	/* U_PrevTrack */
#define	U_CStp			0xC2	/* U_Stop */

#define	U_CSEj			0xC3	/* U_StopEject */
#define	U_CPP			0xC4	/* U_PlayPause */

#define	U_CMte			0xC5	/* U_Mute */
#define	U_CVUp			0xC6	/* U_VolUp */
#define	U_CVDn			0xC7	/* U_VolDwn */

#define	U_CCfg			0xC8	/* U_AL_CC_Config */
#define	U_CEml			0xC9	/* U_AL_Email */
#define	U_CCal			0xCA	/* U_AL_Calculator */
#define	U_CLBr			0xCB	/* U_AL_LocalBrowser */

#define	U_CASc			0xCC	/* U_AC_Search */
#define	U_CAHm			0xCD	/* U_AC_Home */
#define	U_CABw			0xCE	/* U_AC_Back */
#define	U_CAFw			0xCF	/* U_AC_Fwd */
#define	U_CASt			0xD0	/* U_AC_Stop */
#define	U_CARf			0xD1	/* U_AC_Refresh */
#define	U_CABM			0xD2	/* U_AC_Bookmarks */

// 0xE0-7 are regular modifiers

#define	U_FN			0xF0	/* Function key usage */

  #if defined(__MACROS)
#define	U_M1			0xF1	/* Macros 1-8, use only */
#define	U_M2			0xF2	/* if the corresponding */
#define	U_M3			0xF3	/* MACRO_n is defined.  */
#define	U_M4			0xF4
#define	U_M5			0xF5
#define	U_M6			0xF6
#define	U_M7			0xF7
#define	U_M8			0xF8
  #else
#define	U_M1			U_None
#define	U_M2			U_None
#define	U_M3			U_None
#define	U_M4			U_None
#define	U_M5			U_None
#define	U_M6			U_None
#define	U_M7			U_None
#define	U_M8			U_None
  #endif

#define	U_Lock			0xF9	/* Keyboard lock toggle */

#define	U_BtLd			0xFA	/* Enter bootloader */
#define	U_Demo			0xFB	/* LED demo mode toggle */

#define	U_LBUp			0xFC	/* LED brightness up */
#define	U_LBDn			0xFD	/* LED brightness down */

#define	U_WLck			0xFE	/* WIN lock toggle */
#define	U_LNLk			0xFF	/* Local NUM Lock toggle */

//-------------------------------------------------------------------------------
 #endif // __kb__
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
	    U_Num0, U_None, U_Num0, U_None				\
	    /* uCC(U_AC_Home), uCC(U_None) */				\
	    /* , uDms(3000), */						\
	    /* U_LCtr, U_LAlt, U_Del, */				\
	    /* U_None */						\
	}

#define MACRO_6()							\
	{								\
	    U_Grv, U_None, uDms(500),					\
	    U_V, U_O, U_I, U_C, U_E, U_LSft, U_Mins,			\
	    U_S, U_H, U_O, U_W, U_LSft, U_Mins,				\
	    U_M, U_U, U_T, U_E, U_Ent, U_None, uDms(250),		\
	    U_V, U_O, U_I, U_C, U_E, U_LSft, U_Mins,			\
	    U_M, U_U, U_T, U_E, U_Space, U_None				\
	    /* U_LGui, U_L, U_None */					\
	}

//-------------------------------------------------------------------------------
 #endif // __macro__
//-------------------------------------------------------------------------------

#endif	// __kb_h__
