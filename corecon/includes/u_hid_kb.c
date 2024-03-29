/*******************************************************************************
 * File Name	: u_hid_kb.c
 * Project	: Keyboard
 * Date		: 2013/11/06
 * Version	: 1.0
 * Target MCU   : AT90USB8/162, ATMEGA16/32U4, AT90USB64/1286
 * Tool Chain	: Atmel AVR Studio 4.19 730 / gcc 4.7.0 / avr-libc 1.8.0
 * Author	: "Detlef Mueller" <detlef@gmail.com>
 * References	: Some PRJC example code
 * Release Notes: Extremly simplyfied USB chapter 9 implementation.
 *
 * $Id$
 ******************************************************************************/

#include "includes.h"

#include <usb_cfg.h>
#include "u_des_kb.h"

#define __u_hid_kb__
#include "u_hid_kb.h"

#include <avr/eeprom.h>

//******************************************************************************
//  Endpoint Configuration Table
//******************************************************************************

static const uint8_t
    ep_config_table[] PROGMEM =
    {
      #if EP1_SZ
	TRUE, EP_TYPE_INTERRUPT_IN, EP_SIZE( EP1_SZ ) | EP1_BUFFER,
      #else
	FALSE,
      #endif
      #if EP2_SZ
	TRUE, EP_TYPE_INTERRUPT_IN, EP_SIZE( EP2_SZ ) | EP2_BUFFER,
      #else
	FALSE,
      #endif
      #if EP3_SZ
	TRUE, EP_TYPE_INTERRUPT_IN, EP_SIZE( EP3_SZ ) | EP3_BUFFER,
      #else
	FALSE,
      #endif
      #if EP4_SZ
	TRUE, EP_TYPE_INTERRUPT_IN, EP_SIZE( EP4_SZ ) | EP4_BUFFER
      #else
	FALSE
      #endif

      #if defined(__AVR_ATmegaXU4__) || defined(__AVR_AT90USBX6__)
       #if EP5_SZ
	,TRUE, EP_TYPE_INTERRUPT_IN, EP_SIZE( EP5_SZ ) | EP5_BUFFER
       #else
	,FALSE
       #endif
       #if EP6_SZ
	,TRUE, EP_TYPE_INTERRUPT_IN, EP_SIZE( EP6_SZ ) | EP6_BUFFER
       #else
	,FALSE
       #endif
      #endif
    } ;

#define	SUPPORT_ENDPOINT_HALT	1

//******************************************************************************
//  Descriptor Data
//******************************************************************************

// This table maps a host descriptor request to the actual data structure.

#define	IN_RAM		0
#define	IN_ROM		1
#define	IN_EEP		2

#if defined(LARGE_DESC)
 typedef uint16_t desc_sz_t ;
#else
 typedef uint8_t  desc_sz_t ;
#endif

typedef struct
    {
	uint16_t	wValue ;
	uint16_t	wIndex ;
	const uint8_t	*addr ;
	uint8_t		where ;
	desc_sz_t	length ;
    }
    descriptor_list_t ;

static const descriptor_list_t
    descriptors[] PROGMEM =
    {
	{   USBLV( USB_DEVICE_DESC_TYPE, 0 ),	0,
	    VP( &usbDeviceDesc ),		IN_ROM,		sizeof( usb_device_desc_t )
	},
	{   USBLV( USB_CONFIG_DESC_TYPE, 0 ),	0,
	    VP( &usbConfigurationDesc ),	IN_ROM,		sizeof( usb_configuration_desc_t )
	},
	{   USBLV( USB_HID_DESC_TYPE, 0 ),	USB_INTERF_NUM_KBD,
	    VP( &usbConfigurationDesc.hid_kbd ),IN_ROM,		sizeof( usb_hid_desc_t )
	},
	{   USBLV( USB_HIDREP_DESC_TYPE, 0 ),	USB_INTERF_NUM_KBD,
	    VP( &usbHidRepDescKbd ),		IN_ROM,		sizeof( usbHidRepDescKbd )
	},
#if ENABLE_CTRL_KEYS
	{   USBLV( USB_HID_DESC_TYPE, 0 ),	USB_INTERF_NUM_CTRL,
	    VP( &usbConfigurationDesc.hid_ctrl ),IN_ROM,	sizeof( usb_hid_desc_t )
	},
	{   USBLV( USB_HIDREP_DESC_TYPE, 0 ),	USB_INTERF_NUM_CTRL,
	    VP( &usbHidRepDescCtrl ),		IN_ROM,		sizeof( usbHidRepDescCtrl )
	},
#endif
#if ENABLE_DBG_OUT
	{   USBLV( USB_HID_DESC_TYPE, 0 ),	USB_INTERF_NUM_DBG,
	    VP( &usbConfigurationDesc.hid_dbg ),IN_ROM,		sizeof( usb_hid_desc_t )
	},
	{   USBLV( USB_HIDREP_DESC_TYPE, 0 ),	USB_INTERF_NUM_DBG,
	    VP( &usbHidRepDescDbg ),		IN_ROM,		sizeof( usbHidRepDescDbg )
	},
#endif
	{   USBLV( USB_STRING_DESC_TYPE, USB_STRING_IDX_LAN ),	0,
	    VP( &usbStringDescLan ),		IN_ROM,		USB_STRING_DESC_LAN_SZ
	},
	{   USBLV( USB_STRING_DESC_TYPE, USB_STRING_IDX_MAN ),	USB_STRING_LAN,
	    VP( &usbStringDescMan ),		IN_ROM,		USB_STRING_DESC_MAN_SZ
	},
	{   USBLV( USB_STRING_DESC_TYPE, USB_STRING_IDX_PRO ),	USB_STRING_LAN,
	    VP( &usbStringDescPro ),		IN_ROM,		USB_STRING_DESC_PRO_SZ
	},
	{   USBLV( USB_STRING_DESC_TYPE, USB_STRING_IDX_SER ),	USB_STRING_LAN,
	    VP( &usbStringDescSer ),		IN_ROM,
//	    NULL,				IN_EEP,
	    USB_STRING_DESC_SER_SZ
	},
    } ;

//******************************************************************************
//  Variables
//******************************************************************************

volatile uint8_t
    usb_configuration = 0,		// zero when not configured
    usb_suspend    = FALSE,		// Suspended
    usb_rem_wakeup = FALSE ;		// Remote wakeup not allowed

//******************************************************************************

kb_report_t
    kbd_report ;			// Keyboard report

volatile uint8_t
    kbd_protocol,			// Current keyboard protocol
    kbd_leds ;				// Keyboard LED status

volatile uint8_t
    kbd_idle_rate,			// idle rate in 4ms clicks, 0 for indefinite
    kbd_idle_cnt ;			// idle counter

#if ENABLE_CTRL_KEYS

ctrl_report_t
    ctrl_report1,			// System control report
    ctrl_report2 ;			// Consumer control report

#endif

#if ENABLE_DBG_OUT
#endif

//******************************************************************************
//  Public Functions
//******************************************************************************

// Initialize USB system

void usb_init ( void )
{
    // chk_serial() ;

    UHW_CONFIG() ;

    // fire up USB

    USBCON = _B1(USBE) | _B1(FRZCLK) ;	// enable USB module w/ frozen clock

    // config PLL, wait for PLL lock

    for ( PLL_CONFIG() ; bit_is_clear( PLLCSR, PLOCK ) ; )
	;

    USB_CONFIG() ;			// start USB clock

    UDCON = _B0(DETACH) ;		// enable attach resistor

    UDIEN = _B1(EORSTE) |		// setup RX
//	    _B1(SOFE) |			// SOF
	    _B1(SUSPE) ;		// suspend

  #if USB_REPORT_SZ_KBD > 8
    kbd_protocol = 0 ;			// Default is boot protocol
  #else
    kbd_protocol = 1 ;			// Default is report protocol
  #endif

  #if ENABLE_CTRL_KEYS
    ctrl_report1.id  = 1 ;		// Preset report ID's
    ctrl_report2.id  = 2 ;
  #endif

    sei() ;				// Enable interrupts
}

//------------------------------------------------------------------------------

// Check if the USB is configured

uint8_t usb_configured ( void )
{
    uint8_t
	f ;

    CRITICAL_VAR() ;

    ENTER_CRITICAL() ;

    f = (usb_configuration && ! usb_suspend) ;

    EXIT_CRITICAL_RET( f ) ;
}

//------------------------------------------------------------------------------

// USB remote wakeup

uint8_t usb_remote_wakeup ( void )
{
    if ( ! usb_suspend || ! usb_rem_wakeup )
	return ( FALSE ) ;		// Not suspended nor allowed to wake host

    for ( set_bit( PLLCSR, PLLE ) ; bit_is_clear( PLLCSR, PLOCK ) ; )
	;				// Restart PLL

    clr_bit( USBCON, FRZCLK ) ;		// restart USB module

    set_bit( UDCON, RMWKUP ) ;		// Yank wakeup bit

    // Should be moved to end of wake sequence in ISR handler ?
    usb_rem_wakeup = FALSE ;

    return ( TRUE ) ;
}

//------------------------------------------------------------------------------

// Send data via IN pipe

uint8_t usb_send_IN ( uint8_t *data, uint8_t len, uint8_t ep )
{
    CRITICAL_VAR() ;

    ENTER_CRITICAL() ;

    if ( ! usb_configuration || usb_suspend )
	EXIT_CRITICAL_RET( FALSE ) ;	// Pipe closed

    UENUM = ep ;

    if ( bit_is_clear( UEINTX, RWAL ) )	// Buffer full
	EXIT_CRITICAL_RET( FALSE ) ;

    for ( ; len-- ; )			// Write data to FIFO
	UEDATX = *data++ ;

    UEINTX =
	  _B0(FIFOCON)			// Send FIFO data & switch bank
	| _B0(NAKINI)			// Clear previous NAK
	| _B0(RWAL)			// Don't mess w/ RWAL...
	| _B1(NAKOUTI)			// no effect on the bit
	| _B1(RXSTPI)			// no effect on the bit
	| _B0(RXOUTI)			// no effect on the bit
	| _B1(STALLEDI)			// no effect on the bit
	| _B0(TXINI) ;			// clear TX ready int flag

    EXIT_CRITICAL_RET( TRUE ) ;
}

//******************************************************************************
//  Private Functions
//******************************************************************************

// USB Device Interrupt

ISR( USB_GEN_vect )
{
    uint8_t
	intbits ;

    intbits = UDINT ;
    UDINT   = 0 ;

    if ( intbits & _BV( EORSTI ) )	// USB reset
    {
	UENUM   = 0 ;
	UECONX  = 1 ;
	UECFG0X = EP_TYPE_CONTROL ;
	UECFG1X = EP_SIZE( EP0_SZ ) | EP_SINGLE_BUFFER ;
	UEIENX  = _BV( RXSTPE ) ;

      #if USB_REPORT_SZ_KBD > 8
	kbd_protocol = 0 ;		// Default is boot protocol
      #else
	kbd_protocol = 1 ;		// Default is report protocol
      #endif

	usb_suspend       = FALSE ;
	usb_rem_wakeup    = FALSE ;
	usb_configuration = 0 ;

	return ;
    }

    if ( usb_configuration )
    {
	if ( (intbits & _BV(SUSPI)) && bit_is_set( UDIEN, SUSPE ) )
	{
	    // Going to sleep

	    clr_bits( UDIEN, _BV(SUSPE) | _BV(EORSME) ) ;
	    set_bit( UDIEN, WAKEUPE ) ;

	    set_bit( USBCON, FRZCLK ) ;	// Stop USB module
	    clr_bit( PLLCSR, PLLE ) ;	// Stop PLL

	    usb_suspend = TRUE ;
	}
	else
	if ( (intbits & _BV(WAKEUPI)) && bit_is_set( UDIEN, WAKEUPE ) )
	{
	    // Waking up from sleep

	    for ( set_bit( PLLCSR, PLLE ) ; bit_is_clear( PLLCSR, PLOCK ) ; )
		;			// Start PLL

	    clr_bit( USBCON, FRZCLK ) ;	// Start USB module

	    clr_bit( UDIEN, WAKEUPE ) ;

	    set_bits( UDIEN, _BV(SUSPE) | _BV(EORSME) | _BV(SOFE) ) ;
	}

	// Should add a 2ms timeout for falling back into suspend
	// in case the wakeup event was spurious.

	if ( ((intbits & _BV(EORSMI)) && bit_is_set( UDIEN, EORSME )) ||
	     ((intbits & _BV( SOFI )) && bit_is_set( UDIEN, SOFE )) )
	{
	    clr_bits( UDIEN, _BV(EORSME) | _BV(SOFE) ) ;

	    usb_suspend = FALSE ;
	}
    }
}

//------------------------------------------------------------------------------

// Misc functions to wait for ready and send/receive packets

// Wait until packet TX

static inline void usb_wait_in_ready ( void )
{
    for ( ; bit_is_clear( UEINTX, TXINI ) ; )
	;
}

// Trigger TX

static inline void usb_send_in ( void )	
{
    UEINTX = ~_BV( TXINI ) ;
}

// Wait until packet RX

static inline void usb_wait_receive_out ( void )
{
    for ( ; bit_is_clear( UEINTX, RXOUTI ) ; )
	;
}

// Rearm RX

static inline void usb_ack_out ( void )
{
    UEINTX = ~_BV( RXOUTI ) ;
}

//------------------------------------------------------------------------------

// Send data block via EP0. Note: max. size is 255 bytes !

static void FA_NOINLINE( usb_send_EP0 ) ( const uint8_t *p, desc_sz_t len, uint8_t where )
{
    uint8_t
	i, n ;

    do
    {
	// wait until IN packet sent

	do
	{
	    i = UEINTX ;
	}
	while ( bits_are_clear( i, _BV( TXINI ) | _BV( RXOUTI ) ) ) ;

	if ( i & _BV( RXOUTI ) )
	    return ;				// Got OUT packet, abort

	// Set up IN packet

	n = len < EP0_SZ ? len : EP0_SZ ;

	for ( i = n ; i-- ; ++p )
	    if ( where == IN_RAM )
		UEDATX = *p ;
	    else
	    if ( where == IN_ROM )
		UEDATX = pgm_read_byte( p ) ;
	    else
	    if ( where == IN_EEP )
		UEDATX = eeprom_read_byte( p ) ;

	usb_send_in() ;

	len -= n ;
    }
    while ( len || n == EP0_SZ ) ;		// Send ZLP if ! (len % EP0sz)
}

//------------------------------------------------------------------------------

// Look up a descriptor

static uint8_t *lookup
    ( uint16_t wValue, uint16_t wIndex, const descriptor_list_t *desc, uint8_t len )
{
    for ( ; len-- ; ++desc )
	if (    pgm_read_word( &desc->wValue ) == wValue
	     && pgm_read_word( &desc->wIndex ) == wIndex
	   )
	    return ( VP( desc ) ) ;

    return ( NULL ) ;
}

//------------------------------------------------------------------------------

// USB Endpoint Interrupt. Endpoint 0 is handled here.

ISR( USB_COM_vect )
{
    uint16_t
	wValue, wIndex, wLength ;
    uint8_t
	bmRequestType, bRequest,
	i, n,
	*p ;

    UENUM = 0 ;					// Select EP0

    i = UEINTX ;

    if ( i & _BV( RXSTPI ) )
    {
	bmRequestType = UEDATX ;
	bRequest      = UEDATX ;

	wValue   =  UEDATX ;
	wValue  |= (UEDATX << 8) ;

	wIndex   =  UEDATX ;
	wIndex  |= (UEDATX << 8) ;

	wLength  =  UEDATX ;
	wLength |= (UEDATX << 8) ;

	UEINTX = ~(_BV( RXSTPI ) | _BV( RXOUTI ) | _BV( TXINI )) ;

	if ( bRequest == GET_DESCRIPTOR )
	{
	    if ( ! (p = lookup( wValue, wIndex, descriptors, ARRSZ( descriptors ) )) )
	    {
		UECONX = _BV( STALLRQ ) | _BV( EPEN ) ; // stall

		return ;
	    }

	  #if USB_REPORT_SZ_KBD > 8
	    // We default to bootprotocol active (=0 in USB_GEN_vect
	    // and usb_init) until the kbd report descr. is read:

	    if ( p == VP( &descriptors[3] ) )	// KBD report desriptor
		kbd_protocol = 1 ;		// Switch to report protocol
	  #endif

	  #if defined(LARGE_DESC)
	    // Get length and start address

	    wIndex = pgm_read_word( &((descriptor_list_t *)p)->length ) ;

	    if ( wLength > wIndex )
		wLength = wIndex ;

	    n = pgm_read_byte( &((descriptor_list_t *)p)->where ) ;

	    p = VP( pgm_read_word( &((descriptor_list_t *)p)->addr ) ) ;

	    usb_send_EP0( p, wLength, n ) ;
	  #else
	    // Get length and start address

	    n = pgm_read_byte( &((descriptor_list_t *)p)->length ) ;

	    // Note: code limits max. descriptor size to 255

	    if ( wLength & 0xFF00 )
		i = 255 ;
	    else
		i = wLength ;

	    if ( i > n )
		i = n ;

	    n = pgm_read_byte( &((descriptor_list_t *)p)->where ) ;

	    p = VP( pgm_read_word( &((descriptor_list_t *)p)->addr ) ) ;

	    usb_send_EP0( p, i, n ) ;
	  #endif

	    return ;
	}

	if ( bRequest == SET_ADDRESS )
	{
	    usb_send_in() ;			// Ack via ZLP
	    usb_wait_in_ready() ;		// Wait until ZLP sent

	    UDADDR = wValue | _BV( ADDEN ) ;	// Set & enable USB address

	    return ;
	}

	if ( bRequest == SET_CONFIGURATION && bmRequestType == 0 )
	{
	    usb_send_in() ;			// Ack via ZLP

	    usb_configuration = wValue ;	// Remember configuration

	    p = VP( ep_config_table ) ;

	    for ( i = 1 ; i <= MAX_ENDPOINT ; ++i )
	    {
		UENUM = i ;			// Select EP

		n = pgm_read_byte( p++ ) ;

		UECONX = n ;

		if ( n )			// If enabled,
		{				// configure EP
		    UECFG0X = pgm_read_byte( p++ ) ;
		    UECFG1X = pgm_read_byte( p++ ) ;
		}
	    }

	    // Reset EP FIFOs

	    UERST =
		#if defined(__AVR_ATmegaXU4__) || defined(__AVR_AT90USBX6__)
		    _B1(EPRST6) | _B1(EPRST5) |
		#endif
		    _B1(EPRST4) | _B1(EPRST3) | _B1(EPRST2) | _B1(EPRST1) ;

	    UERST = 0 ;

	    return ;
	}

	if ( bRequest == GET_CONFIGURATION && bmRequestType == 0x80 )
	{
	    UEDATX = usb_configuration ;

	    usb_send_in() ;

	    return ;
	}

	if ( bRequest == GET_STATUS )
	{
	    i = 0 ;

	  #if SUPPORT_ENDPOINT_HALT

	    if ( bmRequestType == 0x82 )
	    {
		UENUM = wIndex ;

		if ( bit_is_set( UECONX, STALLRQ ) )
		    i = 1 ;

		UENUM = 0 ;
	    }
	    else
	  #endif
	    if ( bmRequestType == 0x80 )
		i = (usb_rem_wakeup ? 0x02 : 0) ;

	    UEDATX = i ;
	    UEDATX = 0 ;

	    usb_send_in() ;

	    return ;
	}

	if ( bRequest == CLEAR_FEATURE || bRequest == SET_FEATURE )
	{
	  #if SUPPORT_ENDPOINT_HALT

	    if ( bmRequestType == 0x02 && wValue == 0 )
	    {
		n = wIndex & 0x7F ;

		if ( n >= 1 && n <= MAX_ENDPOINT )
		{
		    usb_send_in() ;		// Ack via ZLP

		    UENUM = n ;

		    if ( bRequest == SET_FEATURE )
			UECONX = _B1(STALLRQ) | _B1(EPEN) ;
		    else
		    {
			UECONX = _B1(STALLRQC) | _B1(RSTDT) | _B1(EPEN) ;
			UERST  = _B1(n) ;
			UERST  = 0 ;
		    }

		    return ;
		}
	    }

	  #endif

	    if ( bmRequestType == 0x00 && wValue == 1 )
	    {
		usb_send_in() ;			// Ack via ZLP

		usb_rem_wakeup = (bRequest == SET_FEATURE) ;

		return ;
	    }
	}

	if ( wIndex == USB_INTERF_NUM_KBD )
	{
	    if ( bmRequestType == 0xA1 )
	    {
		if ( bRequest == HID_GET_REPORT )
		{
		  #if USB_REPORT_SZ_KBD > 8
		    if ( ! kbd_protocol )	// Boot protocol
			usb_send_EP0( VP( &kbd_report ), 8, IN_RAM ) ;
		    else
		  #endif
			usb_send_EP0( VP( &kbd_report ), sizeof( kbd_report ), IN_RAM ) ;

		    return ;
		}

		if ( bRequest == HID_GET_IDLE )
		{
		    // LSB(wValue): report ID
		    // wIndex: interface

		    usb_send_EP0( VP( &kbd_idle_rate ), sizeof( kbd_idle_rate ), IN_RAM ) ;

		    return ;
		}

		if ( bRequest == HID_GET_PROTOCOL )
		{
		    usb_send_EP0( VP( &kbd_protocol ), sizeof( kbd_protocol ), IN_RAM ) ;

		    return ;
		}
	    }
	    else
	    if ( bmRequestType == 0x21 )
	    {
		if ( bRequest == HID_SET_REPORT )
		{
		    usb_wait_receive_out() ;

		    kbd_leds = UEDATX ;

		    usb_ack_out() ;
		    usb_send_in() ;

		    return ;
		}

		if ( bRequest == HID_SET_IDLE )
		{
		    usb_send_in() ;		// Ack via ZLP

		    // MSB(wValue): duration in 4ms clicks, 0 for indefinite
		    // LSB(wValue): report ID
		    // wIndex: interface

		    i = MSB( wValue ) ;

		    if ( ! i )			// Indefinite
			kbd_idle_cnt = 0 ;
		    else
		    {
			if ( ! kbd_idle_rate || (kbd_idle_rate - kbd_idle_cnt) >= i )
			    kbd_idle_cnt = 1 ;	// Was indefinite or new idle expired
			else			// Correct counter to new idle time
			    kbd_idle_cnt = i - kbd_idle_cnt ;
		    }

		    kbd_idle_rate = i ;

		    return ;
		}

		if ( bRequest == HID_SET_PROTOCOL )
		{
		    usb_send_in() ;

		    kbd_protocol = wValue ;

		    return ;
		}
	    }
	}

#if ENABLE_CTRL_KEYS
	if ( wIndex == USB_INTERF_NUM_CTRL )
	{
	    if ( bmRequestType == 0xA1 )
	    {
		if ( bRequest == HID_GET_REPORT )
		{
		    // wValue contains report type (h) and id (l), see HID 1,11, 7.2.1
		    // type shoud be "input" (3), id should be 0 or 1/2
		    // wLength should match sizeof( hid_report )

		    if ( LSB( wValue ) == 1 )
		    {
			usb_send_EP0( VP( &ctrl_report1 ), sizeof( ctrl_report1 ), IN_RAM ) ;

			return ;
		    }
		    else
		    if ( LSB( wValue ) == 2 )
		    {
			usb_send_EP0( VP( &ctrl_report2 ), sizeof( ctrl_report2 ), IN_RAM ) ;

			return ;
		    }
		}
	    }
	}
#endif

#if ENABLE_DBG_OUT
	if ( wIndex == USB_INTERF_NUM_DBG )
	{
	    if ( bmRequestType == 0xA1 )
	    {
		if ( bRequest == HID_GET_REPORT )
		{
		    usb_send_EP0( VP( &dbg_report ), sizeof( dbg_report ), IN_RAM ) ;

		    return ;
		}
	    }
	}
#endif

    }

    UECONX = _BV( STALLRQ ) | _BV( EPEN ) ;	// stall
}

//------------------------------------------------------------------------------
