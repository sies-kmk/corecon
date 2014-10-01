/*******************************************************************************
 * File Name	: usb_cfg.h
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

#ifndef __usb_cfg_h__
#define __usb_cfg_h__

//------------------------------------------------------------------------------

#define	ENABLE_CTRL_KEYS		1

#define	KBD_KRO				6	/* 6KRO. 14KRO tested */

#define	USB_EP_POLL_INTERVAL_KBD	2	/* Check EP every  2ms */

//******************************************************************************
#if defined(BlackPetal)
//******************************************************************************

 #define MX_BLUE			' ',' '
 #define MX_BROWN			'B','R'
 #define MX_BLACK			'B','L'
 #define MX_RED				'R','E'

 #define MX_WHITE			'W','H'
 #define MX_CLEAR			'C','L'
 #define MX_GREEN			'G','R'
 #define MX_MIXED			'X','X'

 #define MX_TYPE			MX_BLUE
// #define RK_SERIAL			'1','2','3','4','5','6','7','8','9','0'

//------------------------------------------------------------------------------

 #define USB_MANUFACTURER_ID		wUSB( 0x04D9 )	/* Holtek */
 #define USB_PRODUCT_ID			wUSB( 0x2323 )	/* RK-9000 */

 #define USB_STRING_MAN			'C','o','s','t','a','r',' ','E','l','e','c',\
					't','r','o','n','i','c','s',' ','I','n','c'
 #define USB_STRING_MAN_SZ		22

 #define USB_STRING_PRO			'K','e','y','b','o','a','r','d',' ',	\
					'R','K','-','9','0','0','0',MX_TYPE
 #define USB_STRING_PRO_SZ		18

 #define USB_DEVICE_VERSION		wUSB( 0x0101 )	/* 1.01 */

 #if defined(RK_SERIAL)
  #define USB_STRING_SER		'2','3','2','0','1','0','4',RK_SERIAL
  #define USB_STRING_SER_SZ		17
 #endif

//******************************************************************************
#elif defined(GhostSquid)
//******************************************************************************

 #define MX_GREEN			'G'
 #define MX_BROWN			'M'
 #define MX_BLUE			'L'
 #define MX_BLACK			'C'
 #define MX_RED				'R'
 #define MX_CLEAR			'E'
 #define MX_WHITE			'W'
 #define MX_MIXED			'X'

 #define MX_TYPE			MX_BLUE
// #define QF_SERIAL			'1','2','3','4','5','6','7','8','9','0'

//------------------------------------------------------------------------------

 #define USB_MANUFACTURER_ID		wUSB( 0x2516 )	/* CM Storm */
 #define USB_PRODUCT_ID			wUSB( 0x001A )	/* QuickFire XT */

 #define USB_STRING_MAN			'C','M',' ','S','t','o','r','m'
 #define USB_STRING_MAN_SZ		8

 #define USB_STRING_PRO			'K','e','y','b','o','a','r','d',' ',	\
					'S','G','K','-',			\
					'4','0','3','0','-',			\
					'G','K','C',MX_TYPE,'1','-','U','S'
 #define USB_STRING_PRO_SZ		26

 #define USB_DEVICE_VERSION		wUSB( 0x0100 )	/* 1.00 */

 #if defined(QF_SERIAL)
  #define USB_STRING_SER		QF_SERIAL
  #define USB_STRING_SER_SZ		10
 #endif

//******************************************************************************
#elif defined(FrostyFlake) || defined(FrostyFlake2)
//******************************************************************************

 #define MX_GREEN			'G'
 #define MX_BROWN			'M'
 #define MX_BLUE			'L'
 #define MX_BLACK			'C'
 #define MX_RED				'R'
 #define MX_CLEAR			'E'
 #define MX_WHITE			'W'
 #define MX_MIXED			'X'

 #define QF_RAPID			'1'
 #define QF_STEALTH			'2'

 #define QF_TYPE			QF_RAPID
 #define MX_TYPE			MX_BLUE
// #define QF_SERIAL			'1','2','3','4','5','6','7','8','9','0'

//------------------------------------------------------------------------------

 #define USB_MANUFACTURER_ID		wUSB( 0x2516 )	/* CM Storm */

 #if   QF_TYPE == QF_RAPID
  #define USB_PRODUCT_ID		wUSB( 0x0004 )	/* QuickFire Rapid */
 #elif QF_TYPE == QF_STEALTH
  #define USB_PRODUCT_ID		wUSB( 0x0017 )	/* QuickFire Stealth */
 #endif

 #define USB_STRING_MAN			'C','M',' ','S','t','o','r','m'
 #define USB_STRING_MAN_SZ		8

 #define USB_STRING_PRO			'K','e','y','b','o','a','r','d',' ',	\
					'S','G','K','-',			\
					'4','0','0','0','-',			\
					'G','K','C',MX_TYPE,QF_TYPE,'-','U','S'
 #define USB_STRING_PRO_SZ		26

 #define USB_DEVICE_VERSION		wUSB( 0x0100 )	/* 1.00 */

 #if defined(QF_SERIAL)
  #define USB_STRING_SER		QF_SERIAL
  #define USB_STRING_SER_SZ		10
 #endif

//******************************************************************************
#elif defined(PegasusHoof)
//******************************************************************************

 #define USB_MANUFACTURER_ID		wUSB( 0x04D9 )	/* Holtek */
 #define USB_PRODUCT_ID			wUSB( 0x1818 )	/* FKBN87 */

 #define USB_STRING_MAN			'D','I','A','T','E','C',' ','C','o','r','p'
 #define USB_STRING_MAN_SZ		11

 #define USB_STRING_PRO			'F','I','L','C','O',' ','M','a','j','e','s',\
 					't','o','u','c','h',' ','2',' ','T','K','L'
 #define USB_STRING_PRO_SZ		22

 #define USB_DEVICE_VERSION		wUSB( 0x0102 )	/* 1.02 */

// #define USB_STRING_SER			'1','2','3','4','5','6','7','8','9'
// #define USB_STRING_SER_SZ		9

//******************************************************************************
#elif defined(KittenPaw)
//******************************************************************************

 #define USB_MANUFACTURER_ID		wUSB( 0x04D9 )	/* Holtek */
 #define USB_PRODUCT_ID			wUSB( 0x4545 )	/* FKBN104 */

 #define USB_STRING_MAN			'D','I','A','T','E','C',' ','C','o','r','p'
 #define USB_STRING_MAN_SZ		11

 #define USB_STRING_PRO			'F','I','L','C','O',' ','M','a','j',	\
 					'e','s','t','o','u','c','h',' ','2',
 #define USB_STRING_PRO_SZ		18

 #define USB_DEVICE_VERSION		wUSB( 0x0101 )	/* 1.01 */

// #define USB_STRING_SER			'1','2','3','4','5','6','7','8','9'
// #define USB_STRING_SER_SZ		9

//******************************************************************************
#else
 #error "Unknown Target Controller"
#endif
//******************************************************************************

#define	USB_MAXPOWER			50	/* 50mA */

#define	USB_EP_POLL_INTERVAL_CTRL	10	/* Check EP every 10ms */
#define	USB_EP_POLL_INTERVAL_DBG	10	/* Check EP every 10ms */

#define	ENABLE_DBG_OUT			0 /* not tested or fully implemented yet */

//------------------------------------------------------------------------------
// Report descriptor sizes

#define	USB_HIDREP_DESC_KBD_SZ		61
#define	USB_HIDREP_DESC_CTRL_SZ		48
#define	USB_HIDREP_DESC_DBG_SZ		21

//------------------------------------------------------------------------------
// Report sizes

#define	USB_REPORT_SZ_KBD		(2 + KBD_KRO)	/* 1 (mods) + 1 (fill) + KRO keys */
#define	USB_REPORT_SZ_CTRL		3		/* 1 (id) + 2 (key) */
#define	USB_REPORT_SZ_DBG		16

//------------------------------------------------------------------------------
// Endpoind definitions

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

//-------------------------------------------------------------------------------

#endif	// __usb_cfg_h__
