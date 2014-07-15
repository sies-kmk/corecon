/*******************************************************************************
 * File Name	: main.c
 * Project	: Keyboard
 * Date		: 2014/03/17
 * Version	: 1.0
 * Target MCU   : AVR
 * Tool Chain	: Atmel AVR Studio 4.19 730, avr-gcc, avr-libc
 * Author	: "Detlef Mueller" <detlef@gmail.com>
 * References	:
 * Description	: Keyboard firmware macro playback
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
#include "kbusages.h"

#include "kbmacro.h"

#define __macro__
#include <kb.h>

//------------------------------------------------------------------------------

#define	MK_DELAY		(USB_EP_POLL_INTERVAL_KBD * 2)

#if ENABLE_CTRL_KEYS
 #define MC_DELAY		(USB_EP_POLL_INTERVAL_CTRL * 2)
#endif

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

uint8_t play_macro ( uint8_t idx )
{
    static const uint16_t
	macro_1[] PROGMEM = MACRO_1(),
	macro_2[] PROGMEM = MACRO_2(),
	macro_3[] PROGMEM = MACRO_3() ;
  #ifdef MACRO_4
    static const uint16_t
	macro_4[] PROGMEM = MACRO_4() ;
  #endif
  #ifdef MACRO_5
    static const uint16_t
	macro_5[] PROGMEM = MACRO_5() ;
  #endif
  #ifdef MACRO_6
    static const uint16_t
	macro_6[] PROGMEM = MACRO_6() ;
  #endif
  #ifdef MACRO_7
    static const uint16_t
	macro_7[] PROGMEM = MACRO_7() ;
  #endif
  #ifdef MACRO_8
    static const uint16_t
	macro_8[] PROGMEM = MACRO_8() ;
  #endif

    static const struct
	{
	    uint8_t       sz ;		// Macro size, limited to 255 bytes for now
	    const uint16_t *macro ;	// -> macro
	}
	macros[] PROGMEM =
	{
	    { ARRSZ( macro_1 ), macro_1 },
	    { ARRSZ( macro_2 ), macro_2 },
	    { ARRSZ( macro_3 ), macro_3 }

         #if defined(MACRO_4)
   	    , { ARRSZ( macro_4 ), macro_4 }
          #if defined(MACRO_5)
   	    , { ARRSZ( macro_5 ), macro_5 }
           #if defined(MACRO_6)
   	    , { ARRSZ( macro_6 ), macro_6 }
            #if defined(MACRO_7)
   	    , { ARRSZ( macro_7 ), macro_7 }
             #if defined(MACRO_8)
   	    , { ARRSZ( macro_8 ), macro_8 }
             #endif
            #endif
           #endif
          #endif
         #endif
	} ;

    static uint16_t
	timer,				// Delay timer
	*pm ;				// -> next usage to send

    static uint8_t
	sz ;				// Usages left to send

    uint16_t
	usage ;

    uint8_t
	ret = M_ACTIVE ;

    if ( idx )				// Initialize macro playback
    {
//	if ( idx > ARRSZ( macros ) )	// Should never happen
//	    return ( FALSE ) ;

	memclr8( VP( &kbd_report ), sizeof( kbd_report ) ) ;

	// Clr rep1 & 2 too ?

	sz = pgm_read_byte( &macros[idx - 1].sz ) ;
	pm = VP( pgm_read_word( &macros[idx - 1].macro ) ) ;

	timer = 0 ;

	return ( M_ACTIVE | REP0_CHG ) ;// Signal active macro
    }

    if ( ! timer-- )			// Time to send the next usage
    {
	kbd_report.mod = 0 ;

	do
	{
	    usage = pgm_read_word( pm ) ;
	    ++pm ;
	    --sz ;

	    if ( (usage & 0xFFF0) == 0x00E0 )	// Modifier
	    {
		kbd_report.mod |= _bv8( usage & 0x07 ) ;
		ret |= REP0_CHG ;
	    }
	}
	while ( (usage & 0xFFF0) == 0x00E0 ) ;

	if ( (usage & REP_DEL) )	// Delay
	    timer = (usage & 0x7FFF) ;
      #if ENABLE_CTRL_KEYS
	else
	if ( (usage & REP_SC) )		// System Control usage
	{
	    ctrl_report1.key = (usage & 0x0FFF) ;

	    ret  |= REP1_CHG ;
	    timer = MC_DELAY ;
	}
	else
	if ( (usage & REP_CC) )		// Consumer Control usage
	{
	    ctrl_report2.key = (usage & 0x0FFF) ;

	    ret  |= REP2_CHG ;
	    timer = MC_DELAY ;
	}
      #endif
	else				// Keyboard usage
	{
	    kbd_report.keys[0] = (usage & 0xFF) ;

	    ret  |= REP0_CHG ;
	    timer = MK_DELAY ;		// reset timer
	}

	if ( ! sz )
	    ret &= ~M_ACTIVE ;
    }

    return ( ret ) ;
}

//------------------------------------------------------------------------------
// End of macro.c
