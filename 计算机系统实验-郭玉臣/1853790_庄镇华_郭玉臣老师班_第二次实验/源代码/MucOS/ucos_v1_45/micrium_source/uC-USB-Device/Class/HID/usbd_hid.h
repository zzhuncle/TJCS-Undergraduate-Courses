/*
*********************************************************************************************************
*                                            uC/USB-Device
*                                    The Embedded USB Device Stack
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
*                                        USB DEVICE HID CLASS
*
* Filename : usbd_hid.h
* Version  : V4.05.06
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  USBD_HID_MODULE_PRESENT
#define  USBD_HID_MODULE_PRESENT


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  "../../Source/usbd_core.h"
#include  "usbd_hid_report.h"


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   USBD_HID_MODULE
#define  USBD_HID_EXT
#else
#define  USBD_HID_EXT  extern
#endif


/*
*********************************************************************************************************
*                                               DEFINES
*
* Note(s) : (1) See 'Device Class Definition for Human Interface Devices (HID), 6/27/01, Version 1.11',
*               section 6.2.1 for more details about HID descriptor country code.
*
*               (a) The country code identifies which country the hardware is localized for. Most
*                   hardware is not localized and thus this value would be zero (0). However, keyboards
*                   may use the field to indicate the language of the key caps.
*********************************************************************************************************
*/

                                                                /* ------------ COUNTRY CODES (see Note #1) ----------- */
typedef  enum  usbd_hid_country_code {
    USBD_HID_COUNTRY_CODE_NOT_SUPPORTED      =  0u,             /* See Note #1a.                                        */
    USBD_HID_COUNTRY_CODE_ARABIC             =  1u,
    USBD_HID_COUNTRY_CODE_BELGIAN            =  2u,
    USBD_HID_COUNTRY_CODE_CANADIAN_BILINGUAL =  3u,
    USBD_HID_COUNTRY_CODE_CANADIAN_FRENCH    =  4u,
    USBD_HID_COUNTRY_CODE_CZECH_REPUBLIC     =  5u,
    USBD_HID_COUNTRY_CODE_DANISH             =  6u,
    USBD_HID_COUNTRY_CODE_FINNISH            =  7u,
    USBD_HID_COUNTRY_CODE_FRENCH             =  8u,
    USBD_HID_COUNTRY_CODE_GERMAN             =  9u,
    USBD_HID_COUNTRY_CODE_GREEK              = 10u,
    USBD_HID_COUNTRY_CODE_HEBREW             = 11u,
    USBD_HID_COUNTRY_CODE_HUNGARY            = 12u,
    USBD_HID_COUNTRY_CODE_INTERNATIONAL      = 13u,
    USBD_HID_COUNTRY_CODE_ITALIAN            = 14u,
    USBD_HID_COUNTRY_CODE_JAPAN_KATAKANA     = 15u,
    USBD_HID_COUNTRY_CODE_KOREAN             = 16u,
    USBD_HID_COUNTRY_CODE_LATIN_AMERICAN     = 17u,
    USBD_HID_COUNTRY_CODE_NETHERLANDS_DUTCH  = 18u,
    USBD_HID_COUNTRY_CODE_NORWEGIAN          = 19u,
    USBD_HID_COUNTRY_CODE_PERSIAN_FARSI      = 20u,
    USBD_HID_COUNTRY_CODE_POLAND             = 21u,
    USBD_HID_COUNTRY_CODE_PORTUGUESE         = 22u,
    USBD_HID_COUNTRY_CODE_RUSSIA             = 23u,
    USBD_HID_COUNTRY_CODE_SLOVAKIA           = 24u,
    USBD_HID_COUNTRY_CODE_SPANISH            = 25u,
    USBD_HID_COUNTRY_CODE_SWEDISH            = 26u,
    USBD_HID_COUNTRY_CODE_SWISS_FRENCH       = 27u,
    USBD_HID_COUNTRY_CODE_SWISS_GERMAN       = 28u,
    USBD_HID_COUNTRY_CODE_SWITZERLAND        = 29u,
    USBD_HID_COUNTRY_CODE_TAIWAN             = 30u,
    USBD_HID_COUNTRY_CODE_TURKISH_Q          = 31u,
    USBD_HID_COUNTRY_CODE_UK                 = 32u,
    USBD_HID_COUNTRY_CODE_US                 = 33u,
    USBD_HID_COUNTRY_CODE_YUGOSLAVIA         = 34u,
    USBD_HID_COUNTRY_CODE_TURKISH_F          = 35u
} USBD_HID_COUNTRY_CODE;


/*
*********************************************************************************************************
*                         HUMAN INTERFACE DEVICE CLASS SUBCLASS CODES DEFINES
*
* Note(s) : (1) Human interface device class subclass codes are defined in section 4.2 of HID
*               specification revision 1.11.
*********************************************************************************************************
*/

#define  USBD_HID_SUBCLASS_NONE                        0x00u    /* No subclass.                                         */
#define  USBD_HID_SUBCLASS_BOOT                        0x01u    /* Boot interface.                                      */


/*
*********************************************************************************************************
*                         HUMAN INTERFACE DEVICE CLASS PROTOCOL CODES DEFINES
*
* Note(s) : (1) Human interface device class protocol codes are defined in section 4.3 of HID
*               specification revision 1.11.
*********************************************************************************************************
*/

#define  USBD_HID_PROTOCOL_NONE                        0x00u    /* No class specific protocol.                          */
#define  USBD_HID_PROTOCOL_KBD                         0x01u    /* Keyboard protocol.                                   */
#define  USBD_HID_PROTOCOL_MOUSE                       0x02u    /* Mouse protocol.                                      */


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/

                                                                /* Async comm callback.                                 */
typedef  void  (*USBD_HID_ASYNC_FNCT)(CPU_INT08U   class_nbr,
                                      void        *p_buf,
                                      CPU_INT32U   buf_len,
                                      CPU_INT32U   xfer_len,
                                      void        *p_callback_arg,
                                      USBD_ERR     err);

                                                                /* HID desc and req callbacks.                          */
typedef  const  struct  usbd_hid_callback {
    CPU_BOOLEAN  (*FeatureReportGet)(CPU_INT08U   class_nbr,
                                     CPU_INT08U     report_id,
                                     CPU_INT08U  *p_report_buf,
                                     CPU_INT16U     report_len);

    CPU_BOOLEAN  (*FeatureReportSet)(CPU_INT08U   class_nbr,
                                     CPU_INT08U     report_id,
                                     CPU_INT08U  *p_report_buf,
                                     CPU_INT16U     report_len);

    CPU_INT08U   (*ProtocolGet)     (CPU_INT08U   class_nbr,
                                     USBD_ERR    *p_err);

    void         (*ProtocolSet)     (CPU_INT08U   class_nbr,
                                     CPU_INT08U   protocol,
                                     USBD_ERR    *p_err);

    void         (*ReportSet)       (CPU_INT08U   class_nbr,
                                     CPU_INT08U     report_id,
                                     CPU_INT08U  *p_report_buf,
                                     CPU_INT16U     report_len);
} USBD_HID_CALLBACK;


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

                                                                /* HID class initialization.                            */
void         USBD_HID_Init   (USBD_ERR               *p_err);

                                                                /* Add new instance of the HID class.                   */
CPU_INT08U   USBD_HID_Add    (CPU_INT08U              subclass,
                              CPU_INT08U              protocol,
                              USBD_HID_COUNTRY_CODE   country_code,
                              CPU_INT08U             *p_report_desc,
                              CPU_INT16U              report_desc_len,
                              CPU_INT08U             *p_phy_desc,
                              CPU_INT16U              phy_desc_len,
                              CPU_INT16U              interval_in,
                              CPU_INT16U              interval_out,
                              CPU_BOOLEAN             ctrl_rd_en,
                              USBD_HID_CALLBACK      *p_hid_callback,
                              USBD_ERR               *p_err);

CPU_BOOLEAN  USBD_HID_CfgAdd (CPU_INT08U              class_nbr,
                              CPU_INT08U              dev_nbr,
                              CPU_INT08U              cfg_nbr,
                              USBD_ERR               *p_err);

CPU_BOOLEAN  USBD_HID_IsConn (CPU_INT08U              class_nbr);

CPU_INT32U   USBD_HID_Wr     (CPU_INT08U              class_nbr,
                              void                   *p_buf,
                              CPU_INT32U              buf_len,
                              CPU_INT16U              timeout,
                              USBD_ERR               *p_err);

void         USBD_HID_WrAsync(CPU_INT08U              class_nbr,
                              void                   *p_buf,
                              CPU_INT32U              buf_len,
                              USBD_HID_ASYNC_FNCT     async_fnct,
                              void                   *p_async_arg,
                              USBD_ERR               *p_err);

CPU_INT32U   USBD_HID_Rd     (CPU_INT08U              class_nbr,
                              void                   *p_buf,
                              CPU_INT32U              buf_len,
                              CPU_INT16U              timeout,
                              USBD_ERR               *p_err);

void         USBD_HID_RdAsync(CPU_INT08U              class_nbr,
                              void                   *p_buf,
                              CPU_INT32U              buf_len,
                              USBD_HID_ASYNC_FNCT     async_fnct,
                              void                   *p_async_arg,
                              USBD_ERR               *p_err);


/*
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif
