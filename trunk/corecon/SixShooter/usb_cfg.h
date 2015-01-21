/*******************************************************************************
 * File Name	: usb_cfg.h
 * Project	: Six Shooter for the CM Storm Keyboard Demo
 * Date		: 2015/01/01
 * Version	: 1.0
 * Target MCU   : ATMEGA32U4 (Teensy 2.0)
 * Tool Chain	: Atmel AVR Studio 4.19 730, avr-gcc, avr-libc
 * Author	: "Detlef Mueller" <detlef@gmail.com>
 * Release Notes:
 *
 * $Id$
 ******************************************************************************/

#ifndef __usb_cfg_h__
#define __usb_cfg_h__

//------------------------------------------------------------------------------

#define	ENABLE_CTRL_KEYS		1

#define KBD_KRO				6

//------------------------------------------------------------------------------

#define USB_MANUFACTURER_ID		wUSB( 0x2516 )	/* CM Storm */

#define	USB_PRODUCT_ID			wUSB( 0xBABE )	/* Heck, it's cute ;) */
#define USB_DEVICE_VERSION		wUSB( 0x0200 )	/* 2.00 */

#define USB_STRING_MAN			'C','o','o','l','e','r',' ', \
					'M','a','s','t','e','r'
#define USB_STRING_MAN_SZ		13

#define USB_STRING_PRO			'K','e','y','b','o','a','r','d',' ',	\
					'S','G','K','-',			\
					'1','0','1','0','-',			\
					'B','B','C','A','1'
#define USB_STRING_PRO_SZ		23

#define USB_STRING_SER			'1', '1', '4', '4','1','0','0','0','0','0'
#define USB_STRING_SER_SZ		10

#define	USB_MAXPOWER			50		/* 50mA */

#define	USB_EP_POLL_INTERVAL_KBD	1		/*  1ms polling */
#define	USB_EP_POLL_INTERVAL_CTRL	10		/* 10ms polling */
#define	USB_EP_POLL_INTERVAL_DBG	10		/* 10ms polling */

//------------------------------------------------------------------------------
// Report descriptor sizes

#define	USB_HIDREP_DESC_KBD_SZ		61
#define	USB_HIDREP_DESC_CTRL_SZ		48
#define	USB_HIDREP_DESC_DBG_SZ		21

//------------------------------------------------------------------------------
// Report sizes

#define	USB_REPORT_SZ_KBD		(KBD_KRO + 2)	/* 1 + 1 + 6 keys */
#define	USB_REPORT_SZ_CTRL		3		/* 1 + 2 */
#define	USB_REPORT_SZ_DBG		16

//------------------------------------------------------------------------------
// Endpoind definitions

#define	ENABLE_DBG_OUT			0 /* not tested or fully implemented yet */

#define	EP_HID_KBD			3		/* Endpoint 3 */
#define	EP_HID_KBD_SZ			USB_REPORT_SZ_KBD

#define	EP_HID_CTRL			4		/* Endpoint 4 */
#define	EP_HID_CTRL_SZ			(ENABLE_CTRL_KEYS * USB_REPORT_SZ_CTRL)

#define	EP_HID_DBG			2		/* Endpoint 2 */
#define	EP_HID_DBG_SZ			(ENABLE_DBG_OUT * USB_REPORT_SZ_DBG)

#define EP0_SZ				64
#define EP1_SZ				0
#define EP2_SZ				EP_HID_DBG_SZ
#define EP3_SZ				EP_HID_KBD_SZ
#define EP4_SZ				EP_HID_CTRL_SZ
#define EP5_SZ				0
#define EP6_SZ				0

//-------------------------------------------------------------------------------

#endif
