/*
*********************************************************************************************************
*                                             uC/USB-Host
*                                     The Embedded USB Host Stack
*
*                         (c) Copyright 2004-2019; Silicon Laboratories Inc.,
*                                400 W. Cesar Chavez, Austin, TX 78701
*
*                   All rights reserved. Protected by international copyright laws.
*
*                  Your use of this software is subject to your acceptance of the terms
*                  of a Silicon Labs Micrium software license, which can be obtained by
*                  contacting info@micrium.com. If you do not agree to the terms of this
*                  license, you may not use this software.
*
*                  Please help us continue to provide the Embedded community with the finest
*                  software available. Your honesty is greatly appreciated.
*
*                    You can find our product's documentation at: doc.micrium.com
*
*                          For more information visit us at: www.micrium.com
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                      USB HOST CORE OPERATIONS
*
* Filename : usbh_core.h
* Version  : V3.41.09
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  USBH_CORE_MODULE_PRESENT
#define  USBH_CORE_MODULE_PRESENT


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <lib_def.h>
#include  <lib_mem.h>
#include  <usbh_cfg.h>
#include  "usbh_err.h"
#include  "usbh_os.h"


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   USBH_CORE_MODULE
#define  USBH_CORE_EXT
#else
#define  USBH_CORE_EXT  extern
#endif


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

#define  USBH_VERSION                                  34109u

#define  USBH_LEN_DESC_HDR                              0x02u
#define  USBH_LEN_DESC_DEV                              0x12u
#define  USBH_LEN_DESC_DEV_QUAL                         0x0Au
#define  USBH_LEN_DESC_CFG                              0x09u
#define  USBH_LEN_DESC_OTHERSPD_CFG                     0x09u
#define  USBH_LEN_DESC_IF_ASSOCIATION                   0x08u
#define  USBH_LEN_DESC_IF                               0x09u
#define  USBH_LEN_DESC_EP                               0x07u
#define  USBH_LEN_DESC_OTG                              0x03u
#define  USBH_LEN_SETUP_PKT                             0x08u

#define  USBH_HUB_TIMEOUT                               5000u
#define  USBH_HUB_DLY_DEV_RESET                          100u
#define  USBH_HUB_MAX_DESC_LEN                            72u

#define  USBH_HUB_LEN_HUB_DESC                          0x48u
#define  USBH_HUB_LEN_HUB_STATUS                        0x04u
#define  USBH_HUB_LEN_HUB_PORT_STATUS                   0x04u

#define  USBH_HC_NBR_NONE                               0xFFu


/*
*********************************************************************************************************
*                                       REQUEST CHARACTERISTICS
*
* Note(s) : (1) See 'Universal Serial Bus Specification Revision 2.0', Section 9.3, Table 9-2.
*
*           (2) The 'bmRequestType' field of a setup request is a bit-mapped datum with three subfields :
*
*               (a) Bit  7  : Data transfer direction.
*               (b) Bits 6-5: Type.
*               (c) Bits 4-0: Recipient.
*********************************************************************************************************
*/

#define  USBH_REQ_DIR_MASK                              0x80u
#define  USBH_REQ_DIR_HOST_TO_DEV                       0x00u
#define  USBH_REQ_DIR_DEV_TO_HOST                       0x80u

#define  USBH_REQ_TYPE_STD                              0x00u
#define  USBH_REQ_TYPE_CLASS                            0x20u
#define  USBH_REQ_TYPE_VENDOR                           0x40u
#define  USBH_REQ_TYPE_RSVD                             0x60u

#define  USBH_REQ_RECIPIENT_DEV                         0x00u
#define  USBH_REQ_RECIPIENT_IF                          0x01u
#define  USBH_REQ_RECIPIENT_EP                          0x02u
#define  USBH_REQ_RECIPIENT_OTHER                       0x03u


/*
*********************************************************************************************************
*                                          STANDARD REQUESTS
*
* Note(s) : (1) See 'Universal Serial Bus Specification Revision 2.0', Section 9.4, Table 9-4.
*
*           (2) The 'bRequest' field of a standard setup request may contain one of these values.
*********************************************************************************************************
*/

#define  USBH_REQ_GET_STATUS                            0x00u
#define  USBH_REQ_CLR_FEATURE                           0x01u
#define  USBH_REQ_SET_FEATURE                           0x03u
#define  USBH_REQ_SET_ADDR                              0x05u
#define  USBH_REQ_GET_DESC                              0x06u
#define  USBH_REQ_SET_DESC                              0x07u
#define  USBH_REQ_GET_CFG                               0x08u
#define  USBH_REQ_SET_CFG                               0x09u
#define  USBH_REQ_GET_IF                                0x0Au
#define  USBH_REQ_SET_IF                                0x0Bu
#define  USBH_REQ_SYNCH_FRAME                           0x0Cu


/*
*********************************************************************************************************
*                                          DESCRIPTOR TYPES
*
* Note(s) : (1) See 'Universal Serial Bus Specification Revision 2.0', Section 9.4, Table 9-5, and
*               Section 9.4.3.
*
*           (2) For a 'get descriptor' setup request, the low byte of the 'wValue' field may contain
*               one of these values.
*********************************************************************************************************
*/

#define  USBH_DESC_TYPE_DEV                                1u
#define  USBH_DESC_TYPE_CFG                                2u
#define  USBH_DESC_TYPE_STR                                3u
#define  USBH_DESC_TYPE_IF                                 4u
#define  USBH_DESC_TYPE_EP                                 5u
#define  USBH_DESC_TYPE_DEV_QUALIFIER                      6u
#define  USBH_DESC_TYPE_OTHER_SPD_CONFIG                   7u
#define  USBH_DESC_TYPE_IF_PWR                             8u
#define  USBH_DESC_TYPE_OTG                                9u
#define  USBH_DESC_TYPE_IAD                               11u


/*
*********************************************************************************************************
*                                          FEATURE SELECTORS
*
* Note(s) : (1) See 'Universal Serial Bus Specification Revision 2.0', Section 9.4, Table 9-6, and
*               Section 9.4.1.
*
*           (2) For a 'clear feature' setup request, the 'wValue' field may contain one of these values.
*********************************************************************************************************
*/

#define  USBH_FEATURE_SEL_EP_HALT                          0u
#define  USBH_FEATURE_SEL_DEV_REMOTE_WAKEUP                1u
#define  USBH_FEATURE_SEL_TEST_MODE                        2u
#define  USBH_FEATURE_SEL_B_HNP_EN                         3u


/*
*********************************************************************************************************
*                                    ENDPOINT TYPES (bmAttributes)
*
* Note(s) : (1) See 'Universal Serial Bus Specification Revision 2.0', Section 9.6.6, Table 9-13
*
*           (2) In an endpoint descriptor, the 'bmAttributes' value is one of these values.
*********************************************************************************************************
*/

#define  USBH_EP_TYPE_MASK                              0x03u
#define  USBH_EP_TYPE_CTRL                              0x00u
#define  USBH_EP_TYPE_ISOC                              0x01u
#define  USBH_EP_TYPE_BULK                              0x02u
#define  USBH_EP_TYPE_INTR                              0x03u
                                                                /* Synchronization Type (Bits(3..2)).                   */
#define  USBH_EP_TYPE_SYNC_NONE                         0x00u
#define  USBH_EP_TYPE_SYNC_ASYNC                        0x01u
#define  USBH_EP_TYPE_SYNC_ADAPTIVE                     0x02u
#define  USBH_EP_TYPE_SYNC_SYNC                         0x03u
                                                                /* Usage Type (Bits(5..4)).                             */
#define  USBH_EP_TYPE_USAGE_DATA                        0x00u
#define  USBH_EP_TYPE_USAGE_FEEDBACK                    0x01u
#define  USBH_EP_TYPE_USAGE_IMPLICIT_FEEDACK_DATA       0x02u


/*
*********************************************************************************************************
*                                         ENDPOINT DIRECTION
*
* Note(s) : (1) See 'Universal Serial Bus Specification Revision 2.0', Section 9.6.6, Table 9-13
*
*           (2) In an endpoint descriptor, the upper bit of 'bEndpointAddress' indicates direction.
*********************************************************************************************************
*/

#define  USBH_EP_DIR_MASK                               0x80u
#define  USBH_EP_DIR_OUT                                0x00u
#define  USBH_EP_DIR_IN                                 0x80u
#define  USBH_EP_DIR_NONE                               0x01u


/*
*********************************************************************************************************
*                                 MAX ENDPOINT SIZE (wMaxPacketSize)
*
* Note(s) : (1) See 'Universal Serial Bus Specification Revision 2.0', Section 5.5 to 5.8
*               The values come from:
*               + CONTROL    : Section 5.5.3
*               + ISOCHRONOUS: Section 5.6.3
*               + INTERRUPT  : Section 5.7.3
*               + BULK       : Section 5.8.3
*
*           (2) See 'Universal Serial Bus Specification Revision 2.0', Section 9.6.6
*               Bits 12..11 from wMaxPacketSize indicates the number of transaction per microframe.
*               Valid for high-speed isochronous and interrupt endpoint only.
*********************************************************************************************************
*/
                                                                /* See Note #1                                          */
#define  USBH_MAX_EP_SIZE_TYPE_CTRL_LS                     8u
#define  USBH_MAX_EP_SIZE_TYPE_CTRL_FS                    64u
#define  USBH_MAX_EP_SIZE_TYPE_CTRL_HS                    64u
#define  USBH_MAX_EP_SIZE_TYPE_BULK_FS                    64u
#define  USBH_MAX_EP_SIZE_TYPE_BULK_HS                   512u
#define  USBH_MAX_EP_SIZE_TYPE_INTR_LS                     8u
#define  USBH_MAX_EP_SIZE_TYPE_INTR_FS                    64u
#define  USBH_MAX_EP_SIZE_TYPE_INTR_HS                  1024u
#define  USBH_MAX_EP_SIZE_TYPE_ISOC_FS                  1023u
#define  USBH_MAX_EP_SIZE_TYPE_ISOC_HS                  1024u
                                                                /* See Note #2                                          */
#define  USBH_NBR_TRANSACTION_PER_UFRAME      (DEF_BIT_12 | DEF_BIT_11)
#define  USBH_1_TRANSACTION_PER_UFRAME                     0u
#define  USBH_2_TRANSACTION_PER_UFRAME                     1u
#define  USBH_3_TRANSACTION_PER_UFRAME                     2u


/*
*********************************************************************************************************
*                                      CONFIGURATION ATTRIBUTES
*
* Note(s) : (1) See 'Universal Serial Bus Specification Revision 2.0', Section 9.6.3, Table 9-10
*
*           (2) In a configuration descriptor, the 'bmAttributes' value is a bitmap composed of these values.
*********************************************************************************************************
*/

#define  USBH_CFG_DESC_SELF_POWERED                     0xC0u
#define  USBH_CFG_DESC_BUS_POWERED                      0x80u
#define  USBH_CFG_DESC_REMOTE_WAKEUP                    0x20u


/*
*********************************************************************************************************
*                                        LANGUAGE IDENTIFIERS
*
* Note(s) : (1) See http://www.usb.org/developers/docs/USB_LANGIDs.pdf
*********************************************************************************************************
*/

#define  USBH_LANG_ID_ARABIC_SAUDIARABIA              0x0401u
#define  USBH_LANG_ID_CHINESE_TAIWAN                  0x0404u
#define  USBH_LANG_ID_ENGLISH_UNITEDSTATES            0x0409u
#define  USBH_LANG_ID_ENGLISH_UNITEDKINGDOM           0x0809u
#define  USBH_LANG_ID_FRENCH_STANDARD                 0x040Cu
#define  USBH_LANG_ID_GERMAN_STANDARD                 0x0407u
#define  USBH_LANG_ID_GREEK                           0x0408u
#define  USBH_LANG_ID_ITALIAN_STANDARD                0x0410u
#define  USBH_LANG_ID_PORTUGUESE_STANDARD             0x0816u
#define  USBH_LANG_ID_SANSKRIT                        0x044Fu


/*
*********************************************************************************************************
*                                             CLASS CODES
*
* Note(s) : (1) See 'Universal Class Codes', www.usb.org/developers/defined_class.
*
*           (2) Except as noted, these should be used ONLY in interface descriptors.
*
*               (a) Can only be used in device descriptor.
*
*               (b) Can be used in either device or interface descriptor.
*
*           (4) Subclass & protocol codes are defined in the relevant class drivers.
*********************************************************************************************************
*/

#define  USBH_CLASS_CODE_USE_IF_DESC                    0x00u    /* See Notes #2a.                                      */
#define  USBH_CLASS_CODE_AUDIO                          0x01u
#define  USBH_CLASS_CODE_CDC_CTRL                       0x02u    /* See Notes #2b.                                      */
#define  USBH_CLASS_CODE_HID                            0x03u
#define  USBH_CLASS_CODE_PHYSICAL                       0x05u
#define  USBH_CLASS_CODE_IMAGE                          0x06u
#define  USBH_CLASS_CODE_PRINTER                        0x07u
#define  USBH_CLASS_CODE_MASS_STORAGE                   0x08u
#define  USBH_CLASS_CODE_HUB                            0x09u    /* See Notes #2a.                                      */
#define  USBH_CLASS_CODE_CDC_DATA                       0x0Au
#define  USBH_CLASS_CODE_SMART_CARD                     0x0Bu
#define  USBH_CLASS_CODE_CONTENT_SECURITY               0x0Du
#define  USBH_CLASS_CODE_VIDEO                          0x0Eu
#define  USBH_CLASS_CODE_PERSONAL_HEALTHCARE            0x0Fu
#define  USBH_CLASS_CODE_DIAGNOSTIC_DEV                 0xDCu    /* See Notes #2b                                       */
#define  USBH_CLASS_CODE_WIRELESS_CTRLR                 0xE0u
#define  USBH_CLASS_CODE_MISCELLANEOUS                  0xEFu    /* See Notes #2b.                                      */
#define  USBH_CLASS_CODE_APP_SPECIFIC                   0xFEu
#define  USBH_CLASS_CODE_VENDOR_SPECIFIC                0xFFu    /* See Notes #2b.                                      */


/*
*********************************************************************************************************
*                                           SUBCLASS CODES
*
* Note(s) : (1) See 'Universal Class Codes', www.usb.org/developers/defined_class.
*
*           (2) Except as noted, these should be used ONLY in interface descriptors.
*
*               (a) Can only be used in device descriptor.
*
*               (b) Can be used in either device or interface descriptor.
*
*           (4) Subclass & protocol codes are defined in the relevant class drivers.
*********************************************************************************************************
*/

#define  USBH_SUBCLASS_CODE_USE_IF_DESC                 0x00u    /* See Notes #2a.                                       */
#define  USBH_SUBCLASS_CODE_USE_COMMON_CLASS            0x02u    /* See Notes #2a.                                       */
#define  USBH_SUBCLASS_CODE_VENDOR_SPECIFIC             0xFFu    /* See Notes #2b.                                       */


/*
*********************************************************************************************************
*                                           PROTOCOL CODES
*
* Note(s) : (1) See 'Universal Class Codes', www.usb.org/developers/defined_class.
*
*           (2) Except as noted, these should be used ONLY in interface descriptors.
*
*               (a) Can only be used in device descriptor.
*
*               (b) See "USB Interface Association Descriptor Device Class Code and
*                   Use Model" Document at www.usb.org/developers/whitepapers/iadclasscode_r10.pdf.
*
*               (c) Can be used in either device or interface descriptor.
*
*           (4) Subclass & protocol codes are defined in the relevant class drivers.
*********************************************************************************************************
*/

#define  USBH_PROTOCOL_CODE_USE_IF_DESC                 0x00u    /* See Notes #2a.                                        */
#define  USBH_PROTOCOL_CODE_USE_IAD                     0x01u    /* See Notes #2b.                                        */
#define  USBH_PROTOCOL_CODE_VENDOR_SPECIFIC             0xFFu    /* See Notes #2c.                                        */


/*
*********************************************************************************************************
*                                  USB SPECIFICATION RELEASE NUMBER
*
* Note(s) : (1) See 'Universal Serial Bus Specification Revision 2.0', Section 9.6.1, Table 9-8.
*
*           (2) The field "bcdUSB" is part of the device descriptor and indicates the release number of the
*               USB specification to which the device complies.
*********************************************************************************************************
*/

#define  USBH_SPEC_RELEASE_NBR_1_0                      0x0100u
#define  USBH_SPEC_RELEASE_NBR_1_1                      0x0110u
#define  USBH_SPEC_RELEASE_NBR_2_0                      0x0200u


/*
********************************************************************************************************
*                                           OTG HNP AND SRP
********************************************************************************************************
*/

#define  USBO_OTG_DESC_HNP                              0x01u   /* Device is HNP capable                                 */
#define  USBO_OTG_DESC_SRP                              0x02u   /* Device is SRP capable                                 */


/*
*********************************************************************************************************
*                                         DEFAULT LANGUAGE ID
*********************************************************************************************************
*/

#define  USBH_STRING_DESC_LANGID                        0x00u


/*
*********************************************************************************************************
*                                             HOST STATE
*********************************************************************************************************
*/

#define  USBH_HOST_STATE_NONE                              0u
#define  USBH_HOST_STATE_READY                             1u
#define  USBH_HOST_STATE_SUSPENDED                         2u
#define  USBH_HOST_STATE_RESUMED                           3u


/*
*********************************************************************************************************
*                                              URB STATE
*********************************************************************************************************
*/

#define  USBH_URB_STATE_NONE                               0u
#define  USBH_URB_STATE_SCHEDULED                          1u
#define  USBH_URB_STATE_QUEUED                             2u
#define  USBH_URB_STATE_ABORTED                            3u


/*
*********************************************************************************************************
*                                              USB TOKEN
*********************************************************************************************************
*/

#define  USBH_TOKEN_SETUP                                  0u
#define  USBH_TOKEN_OUT                                    1u
#define  USBH_TOKEN_IN                                     2u


/*
*********************************************************************************************************
*                                          USB DEV + RH DEV
*********************************************************************************************************
*/

#define  USBH_MAX_NBR_DEVS                          USBH_CFG_MAX_NBR_DEVS + USBH_CFG_MAX_NBR_HC


/*
*********************************************************************************************************
*                                        HUB STANDARD REQUESTS
*
* Note(s) : (1) See 'Universal Serial Bus Specification Revision 2.0', Section 11.24.2, Table 11-16.
*
*           (2) The 'bRequest' field of a class-specific setup request may contain one of these values.
*********************************************************************************************************
*/

#define  USBH_HUB_REQ_GET_STATUS                        0x00u
#define  USBH_HUB_REQ_CLR_FEATURE                       0x01u
#define  USBH_HUB_REQ_SET_FEATURE                       0x03u
#define  USBH_HUB_REQ_GET_DESC                          0x06u
#define  USBH_HUB_REQ_SET_DESC                          0x07u
#define  USBH_HUB_REQ_CLR_TT_BUF                        0x08u
#define  USBH_HUB_REQ_RESET_TT                          0x09u
#define  USBH_HUB_REQ_GET_TT_STATE                      0x0Au
#define  USBH_HUB_REQ_STOP_TT                           0x0Bu


/*
*********************************************************************************************************
*                                        HUB DESCRIPTOR TYPES
*
* Note(s) : (1) See 'Universal Serial Bus Specification Revision 2.0', Section 11.23.2, Table 11-13.
*
*           (2) For a 'get descriptor' setup request, the low byte of the 'wValue' field may contain
*               one of these values.
*********************************************************************************************************
*/

#define  USBH_HUB_DESC_TYPE_HUB                         0x29u


/*
*********************************************************************************************************
*                                        HUB FEATURE SELECTORS
*
* Note(s) : (1) See 'Universal Serial Bus Specification Revision 2.0', Section 11.24.2, Table 11-17.
*
*           (2) For a 'clear feature' setup request, the 'wValue' field may contain one of these values.
*********************************************************************************************************
*/

#define  USBH_HUB_FEATURE_SEL_C_HUB_LOCAL_PWR              0u
#define  USBH_HUB_FEATURE_SEL_C_HUB_OVER_CUR               1u

#define  USBH_HUB_FEATURE_SEL_PORT_CONN                    0u
#define  USBH_HUB_FEATURE_SEL_PORT_EN                      1u
#define  USBH_HUB_FEATURE_SEL_PORT_SUSPEND                 2u
#define  USBH_HUB_FEATURE_SEL_PORT_OVER_CUR                3u
#define  USBH_HUB_FEATURE_SEL_PORT_RESET                   4u
#define  USBH_HUB_FEATURE_SEL_PORT_PWR                     8u
#define  USBH_HUB_FEATURE_SEL_PORT_LOW_SPD                 9u
#define  USBH_HUB_FEATURE_SEL_C_PORT_CONN                  16u
#define  USBH_HUB_FEATURE_SEL_C_PORT_EN                    17u
#define  USBH_HUB_FEATURE_SEL_C_PORT_SUSPEND               18u
#define  USBH_HUB_FEATURE_SEL_C_PORT_OVER_CUR              19u
#define  USBH_HUB_FEATURE_SEL_C_PORT_RESET                 20u
#define  USBH_HUB_FEATURE_SEL_PORT_TEST                    21u
#define  USBH_HUB_FEATURE_SEL_PORT_INDICATOR               22u


/*
*********************************************************************************************************
*                                        HUB PORT STATUS BITS
*
* Note(s) : (1) See 'Universal Serial Bus Specification Revision 2.0', Section 11.24.2.7.1, Table 11-21.
*********************************************************************************************************
*/

#define  USBH_HUB_STATUS_PORT_CONN                    0x0001u
#define  USBH_HUB_STATUS_PORT_EN                      0x0002u
#define  USBH_HUB_STATUS_PORT_SUSPEND                 0x0004u
#define  USBH_HUB_STATUS_PORT_OVER_CUR                0x0008u
#define  USBH_HUB_STATUS_PORT_RESET                   0x0010u
#define  USBH_HUB_STATUS_PORT_PWR                     0x0100u
#define  USBH_HUB_STATUS_PORT_LOW_SPD                 0x0200u
#define  USBH_HUB_STATUS_PORT_FULL_SPD                0x0000u
#define  USBH_HUB_STATUS_PORT_HIGH_SPD                0x0400u
#define  USBH_HUB_STATUS_PORT_TEST                    0x0800u
#define  USBH_HUB_STATUS_PORT_INDICATOR               0x1000u


/*
*********************************************************************************************************
*                                     HUB PORT STATUS CHANGE BITS
*
* Note(s) : (1) See 'Universal Serial Bus Specification Revision 2.0', Section 11.24.2.7.2, Table 11-22.
*********************************************************************************************************
*/

#define  USBH_HUB_STATUS_C_PORT_CONN                  0x0001u
#define  USBH_HUB_STATUS_C_PORT_EN                    0x0002u
#define  USBH_HUB_STATUS_C_PORT_SUSPEND               0x0004u
#define  USBH_HUB_STATUS_C_PORT_OVER_CUR              0x0008u
#define  USBH_HUB_STATUS_C_PORT_RESET                 0x0010u


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/

typedef  CPU_INT08U  USBH_EP_TYPE;
typedef  CPU_INT08U  USBH_EP_DIR;
typedef  CPU_INT08U  USBH_HOST_STATE;
typedef  CPU_INT08U  USBH_DEV_STATE;
typedef  CPU_INT08U  USBH_CLASS_DEV_STATE;
typedef  CPU_INT08U  USBH_EP_STATE;
typedef  CPU_INT08U  USBH_TOKEN;


/*
*********************************************************************************************************
*                                        FORWARD DECLARATIONS
*********************************************************************************************************
*/

typedef         struct  usbh_host           USBH_HOST;
typedef         struct  usbh_hub_dev        USBH_HUB_DEV;
typedef         struct  usbh_dev            USBH_DEV;
typedef         struct  usbh_cfg            USBH_CFG;
typedef         struct  usbh_if             USBH_IF;
typedef         struct  usbh_ep             USBH_EP;
typedef         struct  usbh_urb            USBH_URB;

typedef  const  struct  usbh_class_drv      USBH_CLASS_DRV;
typedef         struct  usbh_class_drv_reg  USBH_CLASS_DRV_REG;

typedef         struct  usbh_hc             USBH_HC;
typedef         struct  usbh_hc_drv         USBH_HC_DRV;
typedef  const  struct  usbh_hc_cfg         USBH_HC_CFG;
typedef  const  struct  usbh_hc_drv_api     USBH_HC_DRV_API;
typedef  const  struct  usbh_hc_bsp_api     USBH_HC_BSP_API;
typedef  const  struct  usbh_hc_rh_api      USBH_HC_RH_API;


/*
*********************************************************************************************************
*                                             DEVICE SPEED
*********************************************************************************************************
*/

typedef enum  usbh_dev_spd {
    USBH_DEV_SPD_NONE = 0,
    USBH_DEV_SPD_LOW  = 1,
    USBH_DEV_SPD_FULL = 2,
    USBH_DEV_SPD_HIGH = 3
} USBH_DEV_SPD;


/*
*********************************************************************************************************
*                                      HUB PORT STATUS DATA TYPE
*
* Note(s) : (1) See 'Universal Serial Bus Specification Revision 2.0', Section 11.24.2.7.
*********************************************************************************************************
*/

typedef  struct  usbh_hub_port_status {
    CPU_INT16U  wPortStatus;
    CPU_INT16U  wPortChange;
} USBH_HUB_PORT_STATUS;


/*
*********************************************************************************************************
*                                   USB HOST CONTROLLER DRIVER API
*********************************************************************************************************
*/

struct  usbh_hc_drv_api {
    void          (*Init)        (USBH_HC_DRV  *p_hc_drv,       /* Initialize HC.                                       */
                                  USBH_ERR     *p_err);

    void          (*Start)       (USBH_HC_DRV  *p_hc_drv,       /* Start HC.                                            */
                                  USBH_ERR     *p_err);

    void          (*Stop)        (USBH_HC_DRV  *p_hc_drv,       /* Stop  HC.                                            */
                                  USBH_ERR     *p_err);

    USBH_DEV_SPD  (*SpdGet)      (USBH_HC_DRV  *p_hc_drv,       /* Get HC speed.                                        */
                                  USBH_ERR     *p_err);

    void          (*Suspend)     (USBH_HC_DRV  *p_hc_drv,       /* Suspend HC.                                          */
                                  USBH_ERR     *p_err);

    void          (*Resume)      (USBH_HC_DRV  *p_hc_drv,       /* Resume HC.                                           */
                                  USBH_ERR     *p_err);

    CPU_INT32U    (*FrmNbrGet)   (USBH_HC_DRV  *p_hc_drv,       /* Get HC frame number.                                 */
                                  USBH_ERR     *p_err);

    void          (*EP_Open)     (USBH_HC_DRV  *p_hc_drv,       /* Open endpoint.                                       */
                                  USBH_EP      *p_ep,
                                  USBH_ERR     *p_err);

    void          (*EP_Close)    (USBH_HC_DRV  *p_hc_drv,       /* Close endpoint.                                      */
                                  USBH_EP      *p_ep,
                                  USBH_ERR     *p_err);

    void          (*EP_Abort)    (USBH_HC_DRV  *p_hc_drv,       /* Abort all pending URB on an endpoint.                */
                                  USBH_EP      *p_ep,
                                  USBH_ERR     *p_err);

    CPU_BOOLEAN   (*EP_IsHalt)   (USBH_HC_DRV  *p_hc_drv,       /* Get endpoint halt status.                            */
                                  USBH_EP      *p_ep,
                                  USBH_ERR     *p_err);

    void          (*URB_Submit)  (USBH_HC_DRV  *p_hc_drv,       /* Submit a URB.                                        */
                                  USBH_URB     *p_urb,
                                  USBH_ERR     *p_err);

    void          (*URB_Complete)(USBH_HC_DRV  *p_hc_drv,       /* Complete a URB.                                      */
                                  USBH_URB     *p_urb,
                                  USBH_ERR     *p_err);

    void          (*URB_Abort)   (USBH_HC_DRV  *p_hc_drv,       /* Abort a URB.                                         */
                                  USBH_URB     *p_urb,
                                  USBH_ERR     *p_err);
};


/*
*********************************************************************************************************
*                               USB HOST CONTROLLER ROOT HUB DRIVER API
*********************************************************************************************************
*/

struct  usbh_hc_rh_api {
                                                                /* Get port status.                                     */
    CPU_BOOLEAN  (*PortStatusGet)   (USBH_HC_DRV           *p_hc_drv,
                                     CPU_INT08U             port_nbr,
                                     USBH_HUB_PORT_STATUS  *p_port_status);

                                                                /* Get RH descriptor.                                   */
    CPU_BOOLEAN  (*HubDescGet)      (USBH_HC_DRV           *p_hc_drv,
                                     void                  *p_buf,
                                     CPU_INT08U             buf_len);

                                                                /* Set port enable.                                     */
    CPU_BOOLEAN  (*PortEnSet)       (USBH_HC_DRV           *p_hc_drv,
                                     CPU_INT08U             port_nbr);

                                                                /* Clear port enable.                                   */
    CPU_BOOLEAN  (*PortEnClr)       (USBH_HC_DRV           *p_hc_drv,
                                     CPU_INT08U             port_nbr);

                                                                /* Clear port enable change.                            */
    CPU_BOOLEAN  (*PortEnChngClr)   (USBH_HC_DRV           *p_hc_drv,
                                     CPU_INT08U             port_nbr);

                                                                /* Set port power.                                      */
    CPU_BOOLEAN  (*PortPwrSet)      (USBH_HC_DRV           *p_hc_drv,
                                     CPU_INT08U             port_nbr);

                                                                /* Clear port power.                                    */
    CPU_BOOLEAN  (*PortPwrClr)      (USBH_HC_DRV           *p_hc_drv,
                                     CPU_INT08U             port_nbr);

                                                                /* Set port reset.                                      */
    CPU_BOOLEAN  (*PortResetSet)    (USBH_HC_DRV           *p_hc_drv,
                                     CPU_INT08U             port_nbr);

                                                                /* Clear port reset change.                             */
    CPU_BOOLEAN  (*PortResetChngClr)(USBH_HC_DRV           *p_hc_drv,
                                     CPU_INT08U             port_nbr);

                                                                /* Clear port suspend.                                  */
    CPU_BOOLEAN  (*PortSuspendClr)  (USBH_HC_DRV           *p_hc_drv,
                                     CPU_INT08U             port_nbr);

                                                                /* Clear port connection change.                        */
    CPU_BOOLEAN  (*PortConnChngClr) (USBH_HC_DRV           *p_hc_drv,
                                     CPU_INT08U             port_nbr);

                                                                /* Enable RH interrupt.                                 */
    CPU_BOOLEAN  (*IntEn)           (USBH_HC_DRV           *p_hc_drv);

                                                                /* Disable RH interrupt.                                */
    CPU_BOOLEAN  (*IntDis)          (USBH_HC_DRV           *p_hc_drv);
};


/*
*********************************************************************************************************
*                                     USB HOST CONTROLLER BSP API
*********************************************************************************************************
*/

struct  usbh_hc_bsp_api {
    void  (*Init)       (USBH_HC_DRV   *p_hc_drv,               /* Init BSP.                                            */
                         USBH_ERR      *p_err);

    void  (*ISR_Reg)    (CPU_FNCT_PTR   isr_fnct,               /* Register ISR.                                        */
                         USBH_ERR      *p_err);

    void  (*ISR_Unreg)  (USBH_ERR      *p_err);                 /* Unregister ISR.                                      */
};


/*
*********************************************************************************************************
*                                           HUB DESCRIPTOR
*
* Note(s) : (1) See 'Universal Serial Bus Specification Revision 2.0', Section 11.23.2.1.
*********************************************************************************************************
*/

typedef  struct  usbh_hub_desc {
    CPU_INT08U  bDescLength;
    CPU_INT08U  bDescriptorType;
    CPU_INT08U  bNbrPorts;
    CPU_INT16U  wHubCharacteristics;
    CPU_INT08U  bPwrOn2PwrGood;
    CPU_INT08U  bHubContrCurrent;
    CPU_INT08U  DeviceRemovable;
    CPU_INT32U  PortPwrCtrlMask[USBH_CFG_MAX_HUB_PORTS];
} USBH_HUB_DESC;


/*
*********************************************************************************************************
*                                             HUB STATUS
*
* Note(s) : (1) See 'Universal Serial Bus Specification Revision 2.0', Section 11.24.2.6.
*********************************************************************************************************
*/

typedef  struct  usbh_hub_status {
    CPU_INT16U  wHubStatus;
    CPU_INT16U  wHubChange;
} USBH_HUB_STATUS;


/*
*********************************************************************************************************
-                                            SETUP REQUEST
-
- Note(s) : (1) See 'Universal Serial Bus Specification Revision 2.0', Section 9.3, Table 9-2.
*********************************************************************************************************
*/

typedef  struct  usbh_setup_req {
    CPU_INT08U  bmRequestType;
    CPU_INT08U  bRequest;
    CPU_INT16U  wValue;
    CPU_INT16U  wIndex;
    CPU_INT16U  wLength;
} USBH_SETUP_REQ;


/*
*********************************************************************************************************
*                                          DESCRIPTOR HEADER
*********************************************************************************************************
*/

typedef  struct  usbh_desc_hdr {
    CPU_INT08U  bLength;
    CPU_INT08U  bDescriptorType;
} USBH_DESC_HDR;


/*
*********************************************************************************************************
*                                          DEVICE DESCRIPTOR
*
* Note(s) : (1) See 'Universal Serial Bus Specification Revision 2.0', Section 9.6.1, Table 9-8.
*********************************************************************************************************
*/

typedef  struct  usbh_dev_desc {
    CPU_INT08U  bLength;
    CPU_INT08U  bDescriptorType;
    CPU_INT16U  bcdUSB;
    CPU_INT08U  bDeviceClass;
    CPU_INT08U  bDeviceSubClass;
    CPU_INT08U  bDeviceProtocol;
    CPU_INT08U  bMaxPacketSize0;
    CPU_INT16U  idVendor;
    CPU_INT16U  idProduct;
    CPU_INT16U  bcdDevice;
    CPU_INT08U  iManufacturer;
    CPU_INT08U  iProduct;
    CPU_INT08U  iSerialNumber;
    CPU_INT08U  bNbrConfigurations;
} USBH_DEV_DESC;


/*
*********************************************************************************************************
*                                     DEVICE QUALIFIER DESCRIPTOR
*
* Note(s) : (1) See 'Universal Serial Bus Specification Revision 2.0', Section 9.6.2, Table 9-9.
*********************************************************************************************************
*/

typedef  struct  usbh_dev_qualifier_desc {
    CPU_INT08U  bLength;
    CPU_INT08U  bDescriptorType;
    CPU_INT16U  bcdUSB;
    CPU_INT08U  bDeviceClass;
    CPU_INT08U  bDeviceSubClass;
    CPU_INT08U  bDeviceProtocol;
    CPU_INT08U  bMaxPacketSize0;
    CPU_INT08U  bNbrConfigurations;
    CPU_INT08U  bReserved;
} USBH_DEV_QUALIFIER_DESC;


/*
*********************************************************************************************************
*                                      CONFIGURATION DESCRIPTOR
*
* Note(s) : (1) See 'Universal Serial Bus Specification Revision 2.0', Section 9.6.3, Table 9-10.
*********************************************************************************************************
*/

typedef  struct  usbh_cfg_desc {
    CPU_INT08U  bLength;
    CPU_INT08U  bDescriptorType;
    CPU_INT16U  wTotalLength;
    CPU_INT08U  bNbrInterfaces;
    CPU_INT08U  bConfigurationValue;
    CPU_INT08U  iConfiguration;
    CPU_INT08U  bmAttributes;
    CPU_INT08U  bMaxPower;
} USBH_CFG_DESC;


/*
*********************************************************************************************************
*                                OTHER SPEED CONFIGURATION DESCRIPTOR
*
* Note(s) : (1) See 'Universal Serial Bus Specification Revision 2.0', Section 9.6.4, Table 9-11.
*********************************************************************************************************
*/

typedef  struct  usbh_other_spd_cfg_desc {
    CPU_INT08U  bLength;
    CPU_INT08U  bDescriptorType;
    CPU_INT16U  wTotalLength;
    CPU_INT08U  bNbrInterfaces;
    CPU_INT08U  bConfigurationValue;
    CPU_INT08U  iConfiguration;
    CPU_INT08U  bmAttributes;
    CPU_INT08U  bMaxPower;
} USBH_OTHER_SPD_CFG_DESC;


/*
*********************************************************************************************************
*                                        INTERFACE DESCRIPTOR
*
* Note(s) : (1) See 'Universal Serial Bus Specification Revision 2.0', Section 9.6.5, Table 9-12.
*********************************************************************************************************
*/

typedef  struct  usbh_if_desc {
    CPU_INT08U  bLength;
    CPU_INT08U  bDescriptorType;
    CPU_INT08U  bInterfaceNumber;
    CPU_INT08U  bAlternateSetting;
    CPU_INT08U  bNbrEndpoints;
    CPU_INT08U  bInterfaceClass;
    CPU_INT08U  bInterfaceSubClass;
    CPU_INT08U  bInterfaceProtocol;
    CPU_INT08U  iInterface;
} USBH_IF_DESC;


/*
*********************************************************************************************************
*                                  INTERFACE ASSOCIATION DESCRIPTOR
*
* Note(s) : (1) See 'www.usb.org/developers/doc/InterfaceAssociationDescriptor_ecn.pdf', Section 9.X.Y, Table 9-Z.
*********************************************************************************************************
*/

typedef  struct  usbh_if_association_desc {
    CPU_INT08U  bLength;
    CPU_INT08U  bDescriptorType;
    CPU_INT08U  bFirstInterface;
    CPU_INT08U  bInterfaceCount;
    CPU_INT08U  bFunctionClass;
    CPU_INT08U  bFunctionSubClass;
    CPU_INT08U  bFunctionProtocol;
    CPU_INT08U  iFunction;
} USBH_IF_ASSOCIATION_DESC;


/*
*********************************************************************************************************
*                                         ENDPOINT DESCRIPTOR
*
* Note(s) : (1) See 'Universal Serial Bus Specification Revision 2.0', Section 9.6.6, Table 9-14.
*********************************************************************************************************
*/

typedef  struct  usbh_ep_desc {
    CPU_INT08U  bLength;
    CPU_INT08U  bDescriptorType;
    CPU_INT08U  bEndpointAddress;
    CPU_INT08U  bmAttributes;
    CPU_INT16U  wMaxPacketSize;
    CPU_INT08U  bInterval;
    CPU_INT08U  bRefresh;
    CPU_INT08U  bSynchAddress;
} USBH_EP_DESC;


/*
*********************************************************************************************************
*                                           OTG DESCRIPTOR
*
* Note(s) : (1) See 'On-The-Go Specification Revision 1.3', Section 6.4, Table 6-1.
*********************************************************************************************************
*/

typedef  struct  usbh_otg_desc {
    CPU_INT08U  bLength;
    CPU_INT08U  bDescriptorType;
    CPU_INT08U  bmAttributes;
} USBH_OTG_DESC;


/*
*********************************************************************************************************
*                                       ISOCHRONOUS DESCRIPTOR
*********************************************************************************************************
*/

typedef  struct  usbh_isoc_desc {
    CPU_INT08U  *BufPtr;
    CPU_INT32U   BufLen;
    CPU_INT32U   StartFrm;
    CPU_INT32U   NbrFrm;
    CPU_INT16U  *FrmLen;
    USBH_ERR    *FrmErr;
} USBH_ISOC_DESC;


/*
*********************************************************************************************************
*                                 USB REQUEST BLOCK (URB) INFORMATION
*********************************************************************************************************
*/

struct  usbh_urb {
              CPU_REG08        State;                           /* State of URB.                                        */
              USBH_EP         *EP_Ptr;                          /* EP the urb belongs to.                               */
    volatile  USBH_ERR         Err;                             /* The status of URB completion.                        */

              void            *UserBufPtr;                      /* Ptr to buf supplied by app.                          */
              CPU_INT32U       UserBufLen;                      /* Buf len in bytes.                                    */
              void            *DMA_BufPtr;                      /* DMA buf ptr used by DMA HW.                          */
              CPU_INT32U       DMA_BufLen;                      /* DMA buf len.                                         */
              CPU_INT32U       XferLen;                         /* Actual len xfer'd by ctrlr.                          */

              USBH_ISOC_DESC  *IsocDescPtr;                     /* Isoc xfer desc.                                      */

              void            *FnctPtr;                         /* Fnct ptr, called when I/O is completed.              */
              void            *FnctArgPtr;                      /* Fnct context.                                        */

              void            *ArgPtr;                          /* HCD private data.                                    */

              USBH_TOKEN       Token;                           /* Token (SETUP, IN, or OUT).                           */

              CPU_BOOLEAN      URB_DoneSignal;
              USBH_URB        *AsyncURB_NxtPtr;                 /* Ptr to next URB (if any).                            */
              USBH_URB        *NxtPtr;                          /* Used for URB chained list in async task.             */

              USBH_HSEM        Sem;                             /* Sem to wait on I/O completion.                       */
};


/*
*********************************************************************************************************
*                                        ENDPOINT INFORMATION
*********************************************************************************************************
*/

struct  usbh_ep {
    USBH_DEV_SPD   DevSpd;                                      /* USB dev spd.                                         */
    CPU_INT08U     DevAddr;                                     /* USB dev addr.                                        */
    USBH_DEV      *DevPtr;                                      /* Ptr to USB dev struct.                               */
    USBH_EP_DESC   Desc;                                        /* EP desc.                                             */
    CPU_INT16U     Interval;                                    /* EP interval.                                         */
    CPU_INT32U     HC_RefFrame;                                 /* Initial HC ref frame nbr.                            */
    void          *ArgPtr;                                      /* HCD private data.                                    */
    USBH_URB       URB;                                         /* URB used for data xfer on this endpoint.             */
    USBH_HMUTEX    Mutex;                                       /* Mutex for I/O access serialization on this EP.       */
    CPU_BOOLEAN    IsOpen;                                      /* EP state.                                            */
    CPU_INT32U     XferNbrInProgress;                           /* Nbr of URB(s) in progress. Used for async omm.       */
    CPU_INT08U     DataPID;                                     /* EP Data Toggle PID tracker.                          */
};


/*
*********************************************************************************************************
*                                        INTERFACE INFORMATION
*********************************************************************************************************
*/

struct  usbh_if {
    USBH_DEV            *DevPtr;                                /* Ptr to USB dev.                                      */
    CPU_INT08U           AltIxSel;                              /* Selected alternate setting ix.                       */
    void                *ClassDevPtr;                           /* Ptr to class dev created by class drv.               */
    USBH_CLASS_DRV_REG  *ClassDrvRegPtr;                        /* Ptr to class drv registered for this IF.             */
    CPU_INT08U          *IF_DataPtr;                            /* Buf pointer containing IF data.                      */
    CPU_INT16U           IF_DataLen;                            /* Buf len.                                             */
};


/*
*********************************************************************************************************
*                                      CONFIGURATION INFORMATION
*********************************************************************************************************
*/

struct  usbh_cfg {
    CPU_INT08U  CfgData[USBH_CFG_MAX_CFG_DATA_LEN];             /* Buf containing cfg desc data.                        */
    CPU_INT16U  CfgDataLen;                                     /* Cfg desc data len.                                   */
    USBH_IF     IF_List[USBH_CFG_MAX_NBR_IFS];                  /* Device IFs.                                          */
};


/*
*********************************************************************************************************
*                                         DEVICE INFORMATION
*********************************************************************************************************
*/

struct  usbh_dev {
    USBH_HC             *HC_Ptr;                                /* Ptr to HC struct.                                    */
    CPU_INT08U           DevAddr;                               /* USB dev addr assigned by host.                       */
    USBH_DEV_SPD         DevSpd;                                /* Dev spd (low, full or high).                         */
    USBH_EP              DfltEP;                                /* Dflt ctrl EP.                                        */
    USBH_HMUTEX          DfltEP_Mutex;                          /* Dev dflt EP mutex.                                   */
    CPU_INT16U           LangID;                                /* Language ID used by the str desc.                    */
    void                *ClassDevPtr;                           /* Ptr to class dev created by class drv.               */
    USBH_CLASS_DRV_REG  *ClassDrvRegPtr;                        /* Ptr to class drv managing this dev.                  */
    CPU_INT08U           DevDesc[USBH_LEN_DESC_DEV];            /* Dev desc.                                            */
    USBH_CFG             CfgList[USBH_CFG_MAX_NBR_CFGS];        /* Dev cfg.                                             */
    CPU_INT08U           SelCfg;                                /* Selected dev cfg nbr.                                */
    USBH_DEV            *HubDevPtr;                             /* Ptr to up stream hub dev struct.                     */
    CPU_INT32U           PortNbr;                               /* Port nbr to which this dev is connected.             */
    CPU_BOOLEAN          IsRootHub;                             /* Indicate if this is a RH dev.                        */
    USBH_HUB_DEV        *HubHS_Ptr;                             /* Ptr to prev HS Hub.                                  */
};


/*
*********************************************************************************************************
*                                             HUB DEVICE
*********************************************************************************************************
*/

struct  usbh_hub_dev {
    USBH_EP         IntrEP;                                     /* Intr EP to recv events from hub.                     */
    USBH_HUB_DESC   Desc;                                       /* Hub desc.                                            */
    USBH_DEV       *DevPtrList[USBH_CFG_MAX_HUB_PORTS];         /* Ptrs to USB devs connected to this hub.              */
    USBH_DEV       *DevPtr;                                     /* USB dev ptr of the hub IF.                           */
    USBH_IF        *IF_Ptr;                                     /* HUB IF ptr.                                          */
    CPU_INT08U      HubIntrBuf[64];                             /* Buf to recv hub events.                              */
    CPU_INT32U      ErrCnt;
    CPU_INT08U      State;
    CPU_INT08U      RefCnt;
    USBH_HUB_DEV   *NxtPtr;
    CPU_INT08U      ConnCnt;                                    /* Re-connection counter                                */
};


/*
*********************************************************************************************************
*                                 HOST CONTROLLER DRIVER INFORMATION
*********************************************************************************************************
*/

struct  usbh_hc_drv {
    CPU_INT08U        Nbr;                                      /* HC nbr.                                              */
    void             *DataPtr;                                  /* Drv's data.                                          */
    USBH_DEV         *RH_DevPtr;                                /* Ptr to RH dev struct.                                */
    USBH_HC_CFG      *HC_CfgPtr;                                /* Ptr to HC config struct.                             */
    USBH_HC_DRV_API  *API_Ptr;                                  /* Ptr to HC drv API struct.                            */
    USBH_HC_RH_API   *RH_API_Ptr;                               /* Ptr to RH drv API struct.                            */
    USBH_HC_BSP_API  *BSP_API_Ptr;                              /* Ptr to HC BSP API struct.                            */
};


/*
*********************************************************************************************************
*                                    HOST CONTROLLER CONFIGURATION
*********************************************************************************************************
*/

struct  usbh_hc_cfg {
    CPU_ADDR     BaseAddr;                                      /* HC reg's base addr.                                  */
    CPU_ADDR     DedicatedMemAddr;                              /* Start addr of HC's dedicated mem.                    */
    CPU_INT32U   DedicatedMemSize;                              /* Size of HC's dedicated mem.                          */
    CPU_BOOLEAN  DataBufFromSysMemEn;                           /* Indicate if HC can access sys mem.                   */
    CPU_INT32U   DataBufMaxLen;                                 /* Max len of data buf.                                 */
    CPU_INT32U   MaxNbrEP_BulkOpen;                             /* Max nbr of opened bulk EP.                           */
    CPU_INT32U   MaxNbrEP_IntrOpen;                             /* Max nbr of opened intr EP.                           */
    CPU_INT32U   MaxNbrEP_IsocOpen;                             /* Max nbr of opened isoc EP.                           */
};


/*
*********************************************************************************************************
*                                HOST CONTROLLER INFORMATION DATA TYPE
*********************************************************************************************************
*/

struct usbh_hc {
    USBH_HC_DRV    HC_Drv;                                      /* Host Controller driver (HCD) info.                   */
    USBH_HOST     *HostPtr;                                     /* Host structure.                                      */
    USBH_HUB_DEV  *RH_ClassDevPtr;                              /* Root Hub class device pointer.                       */
    USBH_HMUTEX    HCD_Mutex;                                   /* Mutex to sync access to HCD.                         */
    CPU_BOOLEAN    IsVirRootHub;                                /* Indicate if RH is virtual.                           */
};


/*
*********************************************************************************************************
*                                     HOST INFORMATION DATA TYPE
*********************************************************************************************************
*/

struct  usbh_host {
    USBH_HOST_STATE  State;                                     /* State of USB host stack.                             */

    USBH_DEV         DevList[USBH_MAX_NBR_DEVS];                /* List of USB dev connected.                           */
    MEM_POOL         DevPool;                                   /* Pool for mem mgmt of USB devs.                       */
    MEM_POOL         IsocDescPool;
    USBH_ISOC_DESC   IsocDesc[USBH_CFG_MAX_ISOC_DESC];
    MEM_POOL         AsyncURB_Pool;                             /* Pool of extra URB when using async comm.             */

    USBH_HC          HC_Tbl[USBH_CFG_MAX_NBR_HC];               /* Array of HC structs.                                 */
    CPU_INT08U       HC_NbrNext;

    USBH_HTASK       HAsyncTask;                                /* Async task handle.                                   */
    USBH_HTASK       HHubTask;                                  /* Hub event task handle.                               */
};


/*
*********************************************************************************************************
*                                       KERNEL TASK INFORMATION
*********************************************************************************************************
*/

typedef  const  struct  usbh_kernel_task_info {
    CPU_INT32U   Prio;
    void        *StackPtr;
    CPU_INT32U   StackSize;
} USBH_KERNEL_TASK_INFO;


/*
*********************************************************************************************************
*                                   XFER COMPLETE NOTIFICATION FNCT
*********************************************************************************************************
*/

typedef  void  (*USBH_ISOC_CMPL_FNCT)(USBH_EP     *p_ep,
                                      CPU_INT08U  *p_buf,
                                      CPU_INT32U   buf_len,
                                      CPU_INT32U   cur_xfer_len,
                                      CPU_INT32U   start_frm,
                                      CPU_INT32U   nbr_frm,
                                      CPU_INT16U  *p_frm_len,
                                      USBH_ERR    *p_frm_err,
                                      void        *p_arg,
                                      USBH_ERR     err);

typedef  void  (*USBH_XFER_CMPL_FNCT)(USBH_EP     *p_ep,
                                      void        *p_buf,
                                      CPU_INT32U   buf_len,
                                      CPU_INT32U   xfer_len,
                                      void        *p_arg,
                                      USBH_ERR     err);


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MACROS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                    USB STANDARD FUNCTION MACROS
*********************************************************************************************************
*/

#define  USBH_CLR_FEATURE(p_dev, rcpt, feature, value, index, p_err)        USBH_CtrlTx((p_dev),                                                      \
                                                                                        (USBH_REQ_CLR_FEATURE),                                       \
                                                                                        (rcpt),                                                       \
                                                                                        (value),                                                      \
                                                                                        (index),                                                      \
                                                                                        (void *)0,                                                    \
                                                                                         0u,                                                          \
                                                                                        (USBH_CFG_STD_REQ_TIMEOUT),                                   \
                                                                                        (p_err))

#define  USBH_SET_FEATURE(p_dev, rcpt, feature, value, index, p_err)        USBH_CtrlTx((p_dev),                                                      \
                                                                                        (USBH_REQ_SET_FEATURE),                                       \
                                                                                        (rcpt),                                                       \
                                                                                        (value),                                                      \
                                                                                        (index),                                                      \
                                                                                        (void *)0,                                                    \
                                                                                         0u,                                                          \
                                                                                        (USBH_CFG_STD_REQ_TIMEOUT),                                   \
                                                                                        (p_err))

#define  USBH_GET_CFG(p_dev, data, p_err)                                   USBH_CtrlRx((p_dev),                                                      \
                                                                                        (USBH_REQ_GET_CFG),                                           \
                                                                                        (USBH_REQ_DIR_DEV_TO_HOST | USBH_REQ_RECIPIENT_DEV),          \
                                                                                         0u,                                                          \
                                                                                         0u,                                                          \
                                                                                        (data),                                                       \
                                                                                         1u,                                                          \
                                                                                        (USBH_CFG_STD_REQ_TIMEOUT),                                   \
                                                                                        (p_err))

#define  USBH_GET_DESC(p_dev, desc_type, desc_ix, data, length, p_err)      USBH_CtrlRx((p_dev),                                                      \
                                                                                        (USBH_REQ_GET_DESC),                                          \
                                                                                        (USBH_REQ_DIR_DEV_TO_HOST | USBH_REQ_RECIPIENT_DEV),          \
                                                                                        (desc_type << 8) | (desc_ix),                                 \
                                                                                         0u,                                                          \
                                                                                        (data),                                                       \
                                                                                        (length),                                                     \
                                                                                        (USBH_CFG_STD_REQ_TIMEOUT),                                   \
                                                                                        (p_err))

#define  USBH_GET_IF(ep, if_nbr, data, p_err)                               USBH_CtrlRx((p_dev),                                                      \
                                                                                        (USBH_REQ_GET_IF),                                            \
                                                                                        (USBH_REQ_DIR_DEV_TO_HOST | USBH_REQ_RECIPIENT_IF),           \
                                                                                         0u,                                                          \
                                                                                        (if_nbr),                                                     \
                                                                                        (data),                                                       \
                                                                                         1u,                                                          \
                                                                                        (USBH_CFG_STD_REQ_TIMEOUT),                                   \
                                                                                        (p_err))

#define  USBH_GET_STATUS(p_dev, rcpt, index, data, p_err)                   USBH_CtrlRx((p_dev),                                                      \
                                                                                        (USBH_REQ_GET_STATUS),                                        \
                                                                                        (USBH_REQ_DIR_DEV_TO_HOST) | (rcpt),                          \
                                                                                         0u,                                                          \
                                                                                        (index),                                                      \
                                                                                        (data),                                                       \
                                                                                         2u,                                                          \
                                                                                        (USBH_CFG_STD_REQ_TIMEOUT),                                   \
                                                                                        (p_err))

#define  USBH_SET_ADDR(p_dev, new_usb_addr, p_err)                          USBH_CtrlTx((p_dev),                                                      \
                                                                                        (USBH_REQ_SET_ADDR),                                          \
                                                                                        (USBH_REQ_DIR_HOST_TO_DEV | USBH_REQ_RECIPIENT_DEV),          \
                                                                                         new_usb_addr,                                                \
                                                                                         0u,                                                          \
                                                                                        (void *)0,                                                    \
                                                                                         0u,                                                          \
                                                                                        (USBH_CFG_STD_REQ_TIMEOUT),                                   \
                                                                                        (p_err))

#define  USBH_SET_CFG(p_dev, cfg_nbr, p_err)                                USBH_CtrlTx((p_dev),                                                      \
                                                                                        (USBH_REQ_SET_CFG),                                           \
                                                                                        (USBH_REQ_DIR_HOST_TO_DEV | USBH_REQ_RECIPIENT_DEV),          \
                                                                                        (cfg_nbr),                                                    \
                                                                                         0u,                                                          \
                                                                                        (void *)0,                                                    \
                                                                                         0u,                                                          \
                                                                                        (USBH_CFG_STD_REQ_TIMEOUT),                                   \
                                                                                        (p_err))

#define  USBH_SET_IF(p_dev, if_nbr, alt_nbr, p_err)                         USBH_CtrlTx((p_dev),                                                      \
                                                                                        (USBH_REQ_SET_IF),                                            \
                                                                                        (USBH_REQ_DIR_HOST_TO_DEV | USBH_REQ_RECIPIENT_IF),           \
                                                                                        (alt_nbr),                                                    \
                                                                                        (if_nbr),                                                     \
                                                                                        (void *)0,                                                    \
                                                                                         0u,                                                          \
                                                                                        (USBH_CFG_STD_REQ_TIMEOUT),                                   \
                                                                                        (p_err))


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

                                                                /* --------- USB HOST STACK GENERAL FUNCTIONS --------- */
CPU_INT32U      USBH_VersionGet       (void);

USBH_ERR        USBH_Init             (USBH_KERNEL_TASK_INFO  *async_task_info,
                                       USBH_KERNEL_TASK_INFO  *hub_task_info);

USBH_ERR        USBH_Suspend          (void);

USBH_ERR        USBH_Resume           (void);

                                                                /* ------------ HOST CONTROLLER FUNCTIONS ------------- */
CPU_INT08U      USBH_HC_Add           (USBH_HC_CFG            *p_hc_cfg,
                                       USBH_HC_DRV_API        *p_drv_api,
                                       USBH_HC_RH_API         *p_hc_rh_api,
                                       USBH_HC_BSP_API        *p_hc_bsp_api,
                                       USBH_ERR               *p_err);

USBH_ERR        USBH_HC_Start         (CPU_INT08U              hc_nbr);

USBH_ERR        USBH_HC_Stop          (CPU_INT08U              hc_nbr);

USBH_ERR        USBH_HC_PortEn        (CPU_INT08U              hc_nbr,
                                       CPU_INT08U              port_nbr);

USBH_ERR        USBH_HC_PortDis       (CPU_INT08U              hc_nbr,
                                       CPU_INT08U              port_nbr);

CPU_INT32U      USBH_HC_FrameNbrGet   (CPU_INT08U              hc_nbr,
                                       USBH_ERR               *p_err);

                                                                /* ------------- DEVICE CONTROL FUNCTIONS ------------- */
USBH_ERR        USBH_DevConn          (USBH_DEV               *p_dev);

void            USBH_DevDisconn       (USBH_DEV               *p_dev);

CPU_INT08U      USBH_DevCfgNbrGet     (USBH_DEV               *p_dev);

void            USBH_DevDescGet       (USBH_DEV               *p_dev,
                                       USBH_DEV_DESC          *p_dev_desc);

                                                                /* ---------- DEVICE CONFIGURATION FUNCTIONS ---------- */
USBH_ERR        USBH_CfgSet           (USBH_DEV               *p_dev,
                                       CPU_INT08U              cfg_nbr);

USBH_CFG       *USBH_CfgGet           (USBH_DEV               *p_dev,
                                       CPU_INT08U              cfg_ix);

CPU_INT08U      USBH_CfgIF_NbrGet     (USBH_CFG               *p_cfg);

USBH_ERR        USBH_CfgDescGet       (USBH_CFG               *p_cfg,
                                       USBH_CFG_DESC          *p_cfg_desc);

USBH_DESC_HDR  *USBH_CfgExtraDescGet  (USBH_CFG               *p_cfg,
                                       USBH_ERR               *p_err);

                                                                /* -------- DEVICE INTERFACE CONTROL FUNCTIONS -------- */
USBH_ERR        USBH_IF_Set           (USBH_IF                *p_if,
                                       CPU_INT08U              alt_nbr);

USBH_IF        *USBH_IF_Get           (USBH_CFG               *p_cfg,
                                       CPU_INT08U              if_ix);

CPU_INT08U      USBH_IF_AltNbrGet     (USBH_IF                *p_if);

CPU_INT08U      USBH_IF_NbrGet        (USBH_IF                *p_if);

CPU_INT08U      USBH_IF_EP_NbrGet     (USBH_IF                *p_if,
                                       CPU_INT08U              alt_ix);

USBH_ERR        USBH_IF_DescGet       (USBH_IF                *p_if,
                                       CPU_INT08U              alt_ix,
                                       USBH_IF_DESC           *p_if_desc);

CPU_INT08U     *USBH_IF_ExtraDescGet  (USBH_IF                *p_if,
                                       CPU_INT08U              alt_ix,
                                       CPU_INT16U             *p_data_len);

                                                                /* ---------- DEVICE ENDPOINT OPEN FUNCTIONS ---------- */
USBH_ERR        USBH_BulkInOpen       (USBH_DEV               *p_dev,
                                       USBH_IF                *p_if,
                                       USBH_EP                *p_ep);

USBH_ERR        USBH_BulkOutOpen      (USBH_DEV               *p_dev,
                                       USBH_IF                *p_if,
                                       USBH_EP                *p_ep);

USBH_ERR        USBH_IntrInOpen       (USBH_DEV               *p_dev,
                                       USBH_IF                *p_if,
                                       USBH_EP                *p_ep);

USBH_ERR        USBH_IntrOutOpen      (USBH_DEV               *p_dev,
                                       USBH_IF                *p_if,
                                       USBH_EP                *p_ep);

USBH_ERR        USBH_IsocInOpen       (USBH_DEV               *p_dev,
                                       USBH_IF                *p_if,
                                       USBH_EP                *p_ep);

USBH_ERR        USBH_IsocOutOpen      (USBH_DEV               *p_dev,
                                       USBH_IF                *p_if,
                                       USBH_EP                *p_ep);

                                                                /* -------- DEVICE ENDPOINT TRANSFER FUNCTIONS -------- */
CPU_INT16U      USBH_CtrlTx           (USBH_DEV               *p_dev,
                                       CPU_INT08U              b_req,
                                       CPU_INT08U              bm_req_type,
                                       CPU_INT16U              w_val,
                                       CPU_INT16U              w_ix,
                                       void                   *p_data,
                                       CPU_INT16U              w_len,
                                       CPU_INT32U              timeout_ms,
                                       USBH_ERR               *p_err);

CPU_INT16U      USBH_CtrlRx           (USBH_DEV               *p_dev,
                                       CPU_INT08U              b_req,
                                       CPU_INT08U              bm_req_type,
                                       CPU_INT16U              w_val,
                                       CPU_INT16U              w_ix,
                                       void                   *p_data,
                                       CPU_INT16U              w_len,
                                       CPU_INT32U              timeout_ms,
                                       USBH_ERR               *p_err);

CPU_INT32U      USBH_BulkTx           (USBH_EP                *p_ep,
                                       void                   *p_buf,
                                       CPU_INT32U              buf_len,
                                       CPU_INT32U              timeout_ms,
                                       USBH_ERR               *p_err);

USBH_ERR        USBH_BulkTxAsync      (USBH_EP                *p_ep,
                                       void                   *p_buf,
                                       CPU_INT32U              buf_len,
                                       USBH_XFER_CMPL_FNCT     fnct,
                                       void                   *p_fnct_arg);

CPU_INT32U      USBH_BulkRx           (USBH_EP                *p_ep,
                                       void                   *p_buf,
                                       CPU_INT32U              buf_len,
                                       CPU_INT32U              timeout_ms,
                                       USBH_ERR               *p_err);

USBH_ERR        USBH_BulkRxAsync      (USBH_EP                *p_ep,
                                       void                   *p_buf,
                                       CPU_INT32U              buf_len,
                                       USBH_XFER_CMPL_FNCT     fnct,
                                       void                   *p_fnct_arg);

CPU_INT32U      USBH_IntrTx           (USBH_EP                *p_ep,
                                       void                   *p_buf,
                                       CPU_INT32U              buf_len,
                                       CPU_INT32U              timeout_ms,
                                       USBH_ERR               *p_err);

USBH_ERR        USBH_IntrTxAsync      (USBH_EP                *p_ep,
                                       void                   *p_buf,
                                       CPU_INT32U              buf_len,
                                       USBH_XFER_CMPL_FNCT     fnct,
                                       void                   *p_fnct_arg);

CPU_INT32U      USBH_IntrRx           (USBH_EP                *p_ep,
                                       void                   *p_buf,
                                       CPU_INT32U              buf_len,
                                       CPU_INT32U              timeout_ms,
                                       USBH_ERR               *p_err);

USBH_ERR        USBH_IntrRxAsync      (USBH_EP                *p_ep,
                                       void                   *p_buf,
                                       CPU_INT32U              buf_len,
                                       USBH_XFER_CMPL_FNCT     fnct,
                                       void                   *p_fnct_arg);

CPU_INT32U      USBH_IsocTx           (USBH_EP                *p_ep,
                                       CPU_INT08U             *p_buf,
                                       CPU_INT32U              buf_len,
                                       CPU_INT32U              start_frm,
                                       CPU_INT32U              nbr_frm,
                                       CPU_INT16U             *p_frm_len,
                                       USBH_ERR               *p_frm_err,
                                       CPU_INT32U              timeout_ms,
                                       USBH_ERR               *p_err);

USBH_ERR        USBH_IsocTxAsync      (USBH_EP                *p_ep,
                                       CPU_INT08U             *p_buf,
                                       CPU_INT32U              buf_len,
                                       CPU_INT32U              start_frm,
                                       CPU_INT32U              nbr_frm,
                                       CPU_INT16U             *p_frm_len,
                                       USBH_ERR               *p_frm_err,
                                       USBH_ISOC_CMPL_FNCT     fnct,
                                       void                   *p_fnct_arg);

CPU_INT32U      USBH_IsocRx           (USBH_EP                *p_ep,
                                       CPU_INT08U             *p_buf,
                                       CPU_INT32U              buf_len,
                                       CPU_INT32U              start_frm,
                                       CPU_INT32U              nbr_frm,
                                       CPU_INT16U             *p_frm_len,
                                       USBH_ERR               *p_frm_err,
                                       CPU_INT32U              timeout_ms,
                                       USBH_ERR               *p_err);

USBH_ERR        USBH_IsocRxAsync      (USBH_EP                *p_ep,
                                       CPU_INT08U             *p_buf,
                                       CPU_INT32U              buf_len,
                                       CPU_INT32U              start_frm,
                                       CPU_INT32U              nbr_frm,
                                       CPU_INT16U             *p_frm_len,
                                       USBH_ERR               *p_frm_err,
                                       USBH_ISOC_CMPL_FNCT     fnct,
                                       void                   *p_fnct_arg);

                                                                /* ------------ DEVICE ENDPOINT FUNCTIONS ------------- */
CPU_INT08U      USBH_EP_LogNbrGet     (USBH_EP                *p_ep);

CPU_INT08U      USBH_EP_DirGet        (USBH_EP                *p_ep);

CPU_INT16U      USBH_EP_MaxPktSizeGet (USBH_EP                *p_ep);

CPU_INT08U      USBH_EP_TypeGet       (USBH_EP                *p_ep);

USBH_ERR        USBH_EP_Get           (USBH_IF                *p_if,
                                       CPU_INT08U              alt_ix,
                                       CPU_INT08U              ep_ix,
                                       USBH_EP                *p_ep);

USBH_ERR        USBH_EP_StallSet      (USBH_EP                *p_ep);

USBH_ERR        USBH_EP_StallClr      (USBH_EP                *p_ep);

USBH_ERR        USBH_EP_Reset         (USBH_DEV               *p_dev,
                                       USBH_EP                *p_ep);

USBH_ERR        USBH_EP_Close         (USBH_EP                *p_ep);

                                                                /* ----------- USB REQUEST BLOCK FUNCTIONS ------------ */
void            USBH_URB_Done         (USBH_URB               *p_urb);

USBH_ERR        USBH_URB_Complete     (USBH_URB               *p_urb);

                                                                /* ------------- MISCELLENEOUS FUNCTIONS -------------- */
CPU_INT32U      USBH_StrGet           (USBH_DEV               *p_dev,
                                       CPU_INT08U              desc_ix,
                                       CPU_INT16U              lang_id,
                                       CPU_INT08U             *p_buf,
                                       CPU_INT32U              buf_len,
                                       USBH_ERR               *p_err);


/*
*********************************************************************************************************
*                                         CONFIGURATION ERRORS
*********************************************************************************************************
*/

#ifndef  USBH_CFG_MAX_NBR_DEVS
#error  "USBH_CFG_MAX_NBR_DEVS                 not #define'd in 'usbh_cfg.h'"
#elif   (USBH_CFG_MAX_NBR_DEVS < 1u)
#error  "USBH_CFG_MAX_NBR_DEVS                 illegally #define'd in 'usbh_cfg.h'"
#error  "                                      [MUST be >= 1]                     "
#elif   (USBH_CFG_MAX_NBR_DEVS > 127u)
#error  "USBH_CFG_MAX_NBR_DEVS                 illegally #define'd in 'usbh_cfg.h'"
#error  "                                      [MUST be <= 127]                   "
#endif

#ifndef  USBH_CFG_MAX_NBR_CFGS
#error  "USBH_CFG_MAX_NBR_CFGS                 not #define'd in 'usbh_cfg.h'"
#elif   (USBH_CFG_MAX_NBR_CFGS < 1u)
#error  "USBH_CFG_MAX_NBR_CFGS                 illegally #define'd in 'usbh_cfg.h'"
#error  "                                      [MUST be >= 1]                     "
#endif

#ifndef  USBH_CFG_MAX_NBR_IFS
#error  "USBH_CFG_MAX_NBR_IFS                  not #define'd in 'usbh_cfg.h'"
#elif   (USBH_CFG_MAX_NBR_IFS < 1u)
#error  "USBH_CFG_MAX_NBR_IFS                  illegally #define'd in 'usbh_cfg.h'"
#error  "                                      [MUST be >= 1]                     "
#endif

#ifndef  USBH_CFG_MAX_NBR_EPS
#error  "USBH_CFG_MAX_NBR_EPS                  not #define'd in 'usbh_cfg.h'"
#elif   (USBH_CFG_MAX_NBR_EPS < 1u)
#error  "USBH_CFG_MAX_NBR_EPS                  illegally #define'd in 'usbh_cfg.h'"
#error  "                                      [MUST be >= 1]                     "
#endif

#ifndef  USBH_CFG_MAX_NBR_CLASS_DRVS
#error  "USBH_CFG_MAX_NBR_CLASS_DRVS           not #define'd in 'usbh_cfg.h'"
#elif   (USBH_CFG_MAX_NBR_CLASS_DRVS < 2u)
#error  "USBH_CFG_MAX_NBR_CLASS_DRVS           illegally #define'd in 'usbh_cfg.h'"
#error  "                                      [MUST be >= 2]                     "
#endif

#ifndef  USBH_CFG_MAX_CFG_DATA_LEN
#error  "USBH_CFG_MAX_CFG_DATA_LEN             not #define'd in 'usbh_cfg.h'"
#elif   (USBH_CFG_MAX_CFG_DATA_LEN < 18u)
#error  "USBH_CFG_MAX_CFG_DATA_LEN             illegally #define'd in 'usbh_cfg.h'"
#error  "                                      [MUST be >= 18]                    "
#endif

#ifndef  USBH_CFG_MAX_HUBS
#error  "USBH_CFG_MAX_HUBS                     not #define'd in 'usbh_cfg.h'"
#elif   (USBH_CFG_MAX_HUBS < 1u)
#error  "USBH_CFG_MAX_HUBS                     illegally #define'd in 'usbh_cfg.h'"
#error  "                                  [MUST be >= 1 (min value = 1 root hub)}"
#endif

#ifndef  USBH_CFG_MAX_HUB_PORTS
#error  "USBH_CFG_MAX_HUB_PORTS                not #define'd in 'usbh_cfg.h'"
#elif   (USBH_CFG_MAX_HUB_PORTS < 1u)
#error  "USBH_CFG_MAX_HUB_PORTS                illegally #define'd in 'usbh_cfg.h'"
#error  "                                      [MUST be >= 1]                     "
#elif   (USBH_CFG_MAX_HUB_PORTS > 7u)
#error  "USBH_CFG_MAX_HUB_PORTS                illegally #define'd in 'usbh_cfg.h'"
#error  "USBH_CFG_MAX_HUB_PORTS cannot exceed 7 ports per external hub.           "
#endif

#ifndef  USBH_CFG_STD_REQ_RETRY
#error  "USBH_CFG_STD_REQ_RETRY                not #define'd in 'usbh_cfg.h'"
#elif   (USBH_CFG_STD_REQ_RETRY < 1u)
#error  "USBH_CFG_STD_REQ_RETRY                illegally #define'd in 'usbh_cfg.h'"
#error  "                                      [MUST be >= 1]                     "
#endif

#ifndef  USBH_CFG_STD_REQ_TIMEOUT
#error  "USBH_CFG_STD_REQ_TIMEOUT                  not #define'd in 'usbh_cfg.h'"
#endif

#ifndef  USBH_CFG_MAX_STR_LEN
#error  "USBH_CFG_MAX_STR_LEN                  not #define'd in 'usbh_cfg.h'"
#elif   (USBH_CFG_MAX_STR_LEN > 256u)
#error  "USBH_CFG_MAX_STR_LEN                  illegally #define'd in 'usbh_cfg.h'"
#error  "                                      [MUST be <= 256]                   "
#endif

#ifndef  USBH_CFG_MAX_NBR_HC
#error  "USBH_CFG_MAX_NBR_HC                   not #define'd in 'usbh_cfg.h'"
#elif   (USBH_CFG_MAX_NBR_HC < 1u)
#error  "USBH_CFG_MAX_NBR_HC                   illegally #define'd in 'usbh_cfg.h'"
#error  "                                      [MUST be >= 1]                     "
#endif

#ifndef  USBH_CFG_MAX_ISOC_DESC
#error  "USBH_CFG_MAX_ISOC_DESC                not #define'd in 'usbh_cfg.h'"
#elif   (USBH_CFG_MAX_ISOC_DESC < 1u)
#error  "USBH_CFG_MAX_ISOC_DESC                illegally #define'd in 'usbh_cfg.h'"
#error  "                                      [MUST be >= 1]                     "
#endif

#ifndef  USBH_CFG_MAX_EXTRA_URB_PER_DEV
#error  "USBH_CFG_MAX_EXTRA_URB_PER_DEV        not #define'd in 'usbh_cfg.h'"
#elif   (USBH_CFG_MAX_EXTRA_URB_PER_DEV < 1u)
#error  "USBH_CFG_MAX_EXTRA_URB_PER_DEV        illegally #define'd in 'usbh_cfg.h'"
#error  "                                      [MUST be >= 1]                     "
#endif

#ifndef  USBH_CFG_MAX_NUM_DEV_RECONN
#error  "USBH_CFG_MAX_NUM_DEV_RECONN           not #define'd in 'usbh_cfg.h'"
#endif


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif
