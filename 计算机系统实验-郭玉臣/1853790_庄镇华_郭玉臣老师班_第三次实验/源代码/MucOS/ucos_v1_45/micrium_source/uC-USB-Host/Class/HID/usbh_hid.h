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
*                                    HUMAN INTERFACE DEVICE CLASS
*
* Filename : usbh_hid.h
* Version  : V3.41.09
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  USBH_HID_MODULE_PRESENT
#define  USBH_HID_MODULE_PRESENT


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  "../../Source/usbh_class.h"


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   USBH_HID_MODULE
#define  USBH_HID_EXT
#else
#define  USBH_HID_EXT  extern
#endif


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                          HID PROTOCOL CODES
*
* Note(s) : (1) See 'Device Class Definition for Human Interface Devices (HID), 6/27/01, Version 1.11',
*               section 4.3 for more details about HID protocol codes.
*********************************************************************************************************
*/

#define  USBH_HID_PROTOCOL_CODE_NONE                    0x00u
#define  USBH_HID_PROTOCOL_CODE_KBD                     0x01u
#define  USBH_HID_PROTOCOL_CODE_MOUSE                   0x02u


/*
*********************************************************************************************************
*                                            HID PROTOCOLS
*
* Note(s) : (1) See 'Device Class Definition for Human Interface Devices (HID), 6/27/01, Version 1.11',
*               section 7.2 for more details about HID protocols.
*********************************************************************************************************
*/

#define  USBH_HID_REQ_PROTOCOL_BOOT                   0x0000u
#define  USBH_HID_REQ_PROTOCOL_REPORT                 0x0001u


/*
*********************************************************************************************************
*                                    HID DESCRIPTOR COUNTRY CODES
*
* Note(s) : (1) See 'Device Class Definition for Human Interface Devices (HID), 6/27/01, Version 6.2.1',
*               section 7.2 for more details about HID country codes.
*********************************************************************************************************
*/

#define  USBH_HID_COUNTRY_CODE_NOT_SUPPORTED               0u
#define  USBH_HID_COUNTRY_CODE_ARABIC                      1u
#define  USBH_HID_COUNTRY_CODE_BELGIAN                     2u
#define  USBH_HID_COUNTRY_CODE_CANADIAN_BILINGUAL          3u
#define  USBH_HID_COUNTRY_CODE_CANADIAN_FRENCH             4u
#define  USBH_HID_COUNTRY_CODE_CZECH_REPUBLIC              5u
#define  USBH_HID_COUNTRY_CODE_DANISH                      6u
#define  USBH_HID_COUNTRY_CODE_FINNISH                     7u
#define  USBH_HID_COUNTRY_CODE_FRENCH                      8u
#define  USBH_HID_COUNTRY_CODE_GERMAN                      9u
#define  USBH_HID_COUNTRY_CODE_GREEK                      10u
#define  USBH_HID_COUNTRY_CODE_HEBREW                     11u
#define  USBH_HID_COUNTRY_CODE_HUNGARY                    12u
#define  USBH_HID_COUNTRY_CODE_INTERNATIONAL              13u
#define  USBH_HID_COUNTRY_CODE_ITALIAN                    14u
#define  USBH_HID_COUNTRY_CODE_JAPAN_KATAKANA             15u
#define  USBH_HID_COUNTRY_CODE_KOREAN                     16u
#define  USBH_HID_COUNTRY_CODE_LATIN_AMERICAN             17u
#define  USBH_HID_COUNTRY_CODE_NETHERLANDS_DUTCH          18u
#define  USBH_HID_COUNTRY_CODE_NORWEGIAN                  19u
#define  USBH_HID_COUNTRY_CODE_PERSIAN_FARSI              20u
#define  USBH_HID_COUNTRY_CODE_POLAND                     21u
#define  USBH_HID_COUNTRY_CODE_PORTUGUESE                 22u
#define  USBH_HID_COUNTRY_CODE_RUSSIA                     23u
#define  USBH_HID_COUNTRY_CODE_SLOVAKIA                   24u
#define  USBH_HID_COUNTRY_CODE_SPANISH                    25u
#define  USBH_HID_COUNTRY_CODE_SWEDISH                    26u
#define  USBH_HID_COUNTRY_CODE_SWISS_FRENCH               27u
#define  USBH_HID_COUNTRY_CODE_SWISS_GERMAN               28u
#define  USBH_HID_COUNTRY_CODE_SWITZERLAND                29u
#define  USBH_HID_COUNTRY_CODE_TAIWAN                     30u
#define  USBH_HID_COUNTRY_CODE_TURKISH_Q                  31u
#define  USBH_HID_COUNTRY_CODE_UK                         32u
#define  USBH_HID_COUNTRY_CODE_US                         33u
#define  USBH_HID_COUNTRY_CODE_YUGOSLAVIA                 34u
#define  USBH_HID_COUNTRY_CODE_TURKISH_F                  35u


/*
*********************************************************************************************************
*                                           HID REPORT ITEM
*
* Note(s) : (1) See 'Device Class Definition for Human Interface Devices (HID), 6/27/01, Version 1.11',
*               section 6.2.2 for more details about HID report items.
*********************************************************************************************************
*/

#define  USBH_HID_ITEM_LONG                             0xFEu

                                                                /* ----------------- SHORT ITEM TYPES ----------------- */
#define  USBH_HID_ITEM_TYPE_MAIN                        0x00u
#define  USBH_HID_ITEM_TYPE_GLOBAL                      0x01u
#define  USBH_HID_ITEM_TYPE_LOCAL                       0x02u
#define  USBH_HID_ITEM_TYPE_RESERVED                    0x03u

                                                                /* ------------------ MAIN ITEM TAGS ------------------ */
#define  USBH_HID_MAIN_ITEM_TAG_IN                      0x08u
#define  USBH_HID_MAIN_ITEM_TAG_OUT                     0x09u
#define  USBH_HID_MAIN_ITEM_TAG_COLL                    0x0Au
#define  USBH_HID_MAIN_ITEM_TAG_FEATURE                 0x0Bu
#define  USBH_HID_MAIN_ITEM_TAG_ENDCOLL                 0x0Cu

                                                                /* ----------------- GLOBAL ITEM TAGS ----------------- */
#define  USBH_HID_GLOBAL_ITEM_TAG_USAGE_PAGE            0x00u
#define  USBH_HID_GLOBAL_ITEM_TAG_LOG_MIN               0x01u
#define  USBH_HID_GLOBAL_ITEM_TAG_LOG_MAX               0x02u
#define  USBH_HID_GLOBAL_ITEM_TAG_PHY_MIN               0x03u
#define  USBH_HID_GLOBAL_ITEM_TAG_PHY_MAX               0x04u
#define  USBH_HID_GLOBAL_ITEM_TAG_UNIT_EXPONENT         0x05u
#define  USBH_HID_GLOBAL_ITEM_TAG_UNIT                  0x06u
#define  USBH_HID_GLOBAL_ITEM_TAG_REPORT_SIZE           0x07u
#define  USBH_HID_GLOBAL_ITEM_TAG_REPORT_ID             0x08u
#define  USBH_HID_GLOBAL_ITEM_TAG_REPORT_COUNT          0x09u
#define  USBH_HID_GLOBAL_ITEM_TAG_PUSH                  0x0Au
#define  USBH_HID_GLOBAL_ITEM_TAG_POP                   0x0Bu

                                                                /* ----------------- LOCAL ITEM TAGS ------------------ */
#define  USBH_HID_LOCAL_ITEM_TAG_USAGE                  0x00u
#define  USBH_HID_LOCAL_ITEM_TAG_USAGE_MIN              0x01u
#define  USBH_HID_LOCAL_ITEM_TAG_USAGE_MAX              0x02u
#define  USBH_HID_LOCAL_ITEM_TAG_DESIGNATOR_INDEX       0x03u
#define  USBH_HID_LOCAL_ITEM_TAG_DESIGNATOR_MIN         0x04u
#define  USBH_HID_LOCAL_ITEM_TAG_DESIGNATOR_MAX         0x05u
#define  USBH_HID_LOCAL_ITEM_TAG_STR_IX                 0x07u
#define  USBH_HID_LOCAL_ITEM_TAG_STR_MIN                0x08u
#define  USBH_HID_LOCAL_ITEM_TAG_STR_MAX                0x09u
#define  USBH_HID_LOCAL_ITEM_TAG_DELIMITER              0x0Au


/*
*********************************************************************************************************
*                                             HID REPORT
*
* Note(s) : (1) See 'Device Class Definition for Human Interface Devices (HID), 6/27/01, Version 1.11',
*               section 6.2.2 for more details about HID reports.
*********************************************************************************************************
*/

                                                                /* -------------------- MAIN ITEMS -------------------- */
#define  USBH_HID_MAIN_IN                               0x80u
#define  USBH_HID_MAIN_OUT                              0x90u
#define  USBH_HID_MAIN_COLL                             0xA0u
#define  USBH_HID_MAIN_FEATURE                          0xB0u
#define  USBH_HID_MAIN_ENDCOLL                          0xC0u

                                                                /* ---------- INPUT / OUTPUT / FEATURE ITEMS ---------- */
#define  USBH_HID_MAIN_CONST                            0x01u
#define  USBH_HID_MAIN_DATA                             0x00u
#define  USBH_HID_MAIN_VAR                              0x02u
#define  USBH_HID_MAIN_ARRAY                            0x00u
#define  USBH_HID_MAIN_REL                              0x04u
#define  USBH_HID_MAIN_ABS                              0x00u
#define  USBH_HID_MAIN_WRAP                             0x08u
#define  USBH_HID_MAIN_NOWRAP                           0x00u
#define  USBH_HID_MAIN_NONLINE                          0x10u
#define  USBH_HID_MAIN_LINE                             0x00u
#define  USBH_HID_MAIN_NOPREFERRED                      0x20u
#define  USBH_HID_MAIN_PREFERREDSTATE                   0x00u
#define  USBH_HID_MAIN_NULLSTATE                        0x40u
#define  USBH_HID_MAIN_NO_NULL_POS                      0x00u
#define  USBH_HID_MAIN_VOLATILE                         0x80u
#define  USBH_HID_MAIN_NONVOLATILE                      0x00u
#define  USBH_HID_MAIN_BUFFEREDBYTES                  0x0100u
#define  USBH_HID_MAIN_BITFIELD                       0x0000u

                                                                /* ----------------- COLLECTION ITEMS ----------------- */
#define  USBH_HID_COLL_PHYSICAL                         0x00u
#define  USBH_HID_COLL_APP                              0x01u
#define  USBH_HID_COLL_LOGICAL                          0x02u
#define  USBH_HID_COLL_REPORT                           0x03u
#define  USBH_HID_COLL_NAMED_ARRAY                      0x04u
#define  USBH_HID_COLL_USAGE_SWITCH                     0x05u
#define  USBH_HID_COLL_USAGE_MODIFIER                   0x06u

                                                                /* ------------------- GLOBAL ITEMS ------------------- */
#define  USBH_HID_GLOBAL_USAGE_PAGE                     0x04u
#define  USBH_HID_GLOBAL_LOG_MIN                        0x14u
#define  USBH_HID_GLOBAL_LOG_MAX                        0x24u
#define  USBH_HID_GLOBAL_PHY_MIN                        0x34u
#define  USBH_HID_GLOBAL_PHY_MAX                        0x44u
#define  USBH_HID_GLOBAL_UNIT_EXPONENT                  0x54u
#define  USBH_HID_GLOBAL_UNIT                           0x64u
#define  USBH_HID_GLOBAL_REPORT_SIZE                    0x74u
#define  USBH_HID_GLOBAL_REPORT_ID                      0x84u
#define  USBH_HID_GLOBAL_REPORT_COUNT                   0x94u
#define  USBH_HID_GLOBAL_PUSH                           0xA4u
#define  USBH_HID_GLOBAL_POP                            0xB4u

                                                                /* ------------------- LOCAL ITEMS -------------------- */
#define  USBH_HID_LOCAL_USAGE                           0x08u
#define  USBH_HID_LOCAL_USAGE_MIN                       0x18u
#define  USBH_HID_LOCAL_USAGE_MAX                       0x28u
#define  USBH_HID_LOCAL_DESIGNATOR_IX                   0x38u
#define  USBH_HID_LOCAL_DESIGNATOR_MIN                  0x48u
#define  USBH_HID_LOCAL_DESIGNATOR_MAX                  0x58u
#define  USBH_HID_LOCAL_STR_INDEX                       0x78u
#define  USBH_HID_LOCAL_STR_MIN                         0x88u
#define  USBH_HID_LOCAL_STR_MAX                         0x98u
#define  USBH_HID_LOCAL_DELIMITER                       0xA8u


/*
*********************************************************************************************************
*                                       HID PHYSICAL DESCRIPTOR
*
* Note(s) : (1) See 'Device Class Definition for Human Interface Devices (HID), 6/27/01, Version 1.11',
*               section 6.2.3 for more details about HID physical descriptors.
*********************************************************************************************************
*/

                                                                /* -------------------- BIAS VALUES ------------------- */
#define  USBH_HID_BIAS_NOT_APPLICABLE                      0u
#define  USBH_HID_BIAS_RIGHT_HAND                          1u
#define  USBH_HID_BIAS_LEFT_HAND                           2u
#define  USBH_HID_BIAS_BOTH_HANDS                          3u
#define  USBH_HID_BIAS_EITHER_HAND                         4u

                                                                /* ----------------- DESIGNATOR VALUES ---------------- */
#define  USBH_HID_DESIGNATOR_NONE                       0x00u
#define  USBH_HID_DESIGNATOR_HAND                       0x01u
#define  USBH_HID_DESIGNATOR_EYEBALL                    0x02u
#define  USBH_HID_DESIGNATOR_EYEBROW                    0x03u
#define  USBH_HID_DESIGNATOR_EYELID                     0x04u
#define  USBH_HID_DESIGNATOR_EAR                        0x05u
#define  USBH_HID_DESIGNATOR_NOSE                       0x06u
#define  USBH_HID_DESIGNATOR_MOUTH                      0x07u
#define  USBH_HID_DESIGNATOR_UPPER_LIP                  0x08u
#define  USBH_HID_DESIGNATOR_LOWER_LIP                  0x09u
#define  USBH_HID_DESIGNATOR_JAW                        0x0Au
#define  USBH_HID_DESIGNATOR_NECK                       0x0Bu
#define  USBH_HID_DESIGNATOR_UPPER_ARM                  0x0Cu
#define  USBH_HID_DESIGNATOR_ELBOW                      0x0Du
#define  USBH_HID_DESIGNATOR_FOREARM                    0x0Eu
#define  USBH_HID_DESIGNATOR_WRIST                      0x0Fu
#define  USBH_HID_DESIGNATOR_PALM                       0x10u
#define  USBH_HID_DESIGNATOR_THUMB                      0x11u
#define  USBH_HID_DESIGNATOR_INDEX_FINGER               0x12u
#define  USBH_HID_DESIGNATOR_MIDDLE_FINGER              0x13u
#define  USBH_HID_DESIGNATOR_RING_FINGER                0x14u
#define  USBH_HID_DESIGNATOR_LITTLE_FINGER              0x15u
#define  USBH_HID_DESIGNATOR_HEAD                       0x16u
#define  USBH_HID_DESIGNATOR_SHOULDER                   0x17u
#define  USBH_HID_DESIGNATOR_HIP                        0x18u
#define  USBH_HID_DESIGNATOR_WAIST                      0x19u
#define  USBH_HID_DESIGNATOR_THIGH                      0x1Au
#define  USBH_HID_DESIGNATOR_KNEE                       0x1Bu
#define  USBH_HID_DESIGNATOR_CALF                       0x1Cu
#define  USBH_HID_DESIGNATOR_ANKLE                      0x1Du
#define  USBH_HID_DESIGNATOR_FOOT                       0x1Eu
#define  USBH_HID_DESIGNATOR_HEEL                       0x1Fu
#define  USBH_HID_DESIGNATOR_BALL_OF_FOOT               0x20u
#define  USBH_HID_DESIGNATOR_BIG_TOE                    0x21u
#define  USBH_HID_DESIGNATOR_SECOND_TOE                 0x22u
#define  USBH_HID_DESIGNATOR_THIRD_TOE                  0x23u
#define  USBH_HID_DESIGNATOR_FOURTH_TOE                 0x24u
#define  USBH_HID_DESIGNATOR_LITTLE_TOE                 0x25u
#define  USBH_HID_DESIGNATOR_BROW                       0x26u
#define  USBH_HID_DESIGNATOR_CHEEK                      0x27u

                                                                /* ----------------- QUALIFIER VALUES ----------------- */
#define  USBH_HID_QUALIFIER_NOT_APPLICABLE                 0u
#define  USBH_HID_QUALIFIER_RIGHT                          1u
#define  USBH_HID_QUALIFIER_LEFT                           2u
#define  USBH_HID_QUALIFIER_BOTH                           3u
#define  USBH_HID_QUALIFIER_EITHER                         4u
#define  USBH_HID_QUALIFIER_CENTER                         5u


/*
*********************************************************************************************************
*                                           HID USAGE PAGES
*
* Note(s) : (1) See 'Universal Serial Bus HID Usage Tables, 10/28/04, Version 1.12',
*               section 3 for more details about usage pages.
*********************************************************************************************************
*/

#define  USBH_HID_USAGE_PAGE_UNDEFINED                  0x00u
#define  USBH_HID_USAGE_PAGE_GENERIC_DESKTOP_CTRLS      0x01u
#define  USBH_HID_USAGE_PAGE_SIMULATION_CTRLS           0x02u
#define  USBH_HID_USAGE_PAGE_VR_CTRLS                   0x03u
#define  USBH_HID_USAGE_PAGE_SPORT_CTRLS                0x04u
#define  USBH_HID_USAGE_PAGE_GAME_CTRLS                 0x05u
#define  USBH_HID_USAGE_PAGE_GENERIC_DEV_CTRLS          0x06u
#define  USBH_HID_USAGE_PAGE_KBD                        0x07u
#define  USBH_HID_USAGE_PAGE_LEDS                       0x08u
#define  USBH_HID_USAGE_PAGE_BUTTON                     0x09u
#define  USBH_HID_USAGE_PAGE_ORDINAL                    0x0Au
#define  USBH_HID_USAGE_PAGE_TELEPHONY                  0x0Bu
#define  USBH_HID_USAGE_PAGE_CONSUMER                   0x0Cu
#define  USBH_HID_USAGE_PAGE_DIGITIZER                  0x0Du
#define  USBH_HID_USAGE_PAGE_PID_PAGE                   0x0Fu
#define  USBH_HID_USAGE_PAGE_UNICODE                    0x10u
#define  USBH_HID_USAGE_PAGE_ALPHANUMERIC_DISP          0x14u
#define  USBH_HID_USAGE_PAGE_MEDICAL_INSTRUMENTS        0x40u
#define  USBH_HID_USAGE_PAGE_MON_0                      0x80u
#define  USBH_HID_USAGE_PAGE_MON_1                      0x81u
#define  USBH_HID_USAGE_PAGE_MON_2                      0x82u
#define  USBH_HID_USAGE_PAGE_MON_3                      0x83u
#define  USBH_HID_USAGE_PAGE_PWR_0                      0x84u
#define  USBH_HID_USAGE_PAGE_PWR_1                      0x85u
#define  USBH_HID_USAGE_PAGE_PWR_2                      0x86u
#define  USBH_HID_USAGE_PAGE_PWR_3                      0x87u
#define  USBH_HID_USAGE_PAGE_BAR_CODE_SCANNER_PAGE      0x8Cu
#define  USBH_HID_USAGE_PAGE_SCALE_PAGE                 0x8Du
#define  USBH_HID_USAGE_PAGE_MSR_DEVS                   0x8Eu
#define  USBH_HID_USAGE_PAGE_POINT_OF_SALE_PAGES        0x8Fu
#define  USBH_HID_USAGE_PAGE_CAMERA_CTRL_PAGE           0x90u
#define  USBH_HID_USAGE_PAGE_ARCADE_PAGE                0x91u

                                                                /* ---------- GENERIC DESKTOP PAGE APP USAGES --------- */
#define  USBH_HID_CA_MOUSE                        0x00010002u
#define  USBH_HID_CA_JOYSTICK                     0x00010004u
#define  USBH_HID_CA_GAME_PAD                     0x00010005u
#define  USBH_HID_CA_KBD                          0x00010006u
#define  USBH_HID_CA_KEYPAD                       0x00010007u
#define  USBH_HID_CA_MULTI_AXIS_CTRLR             0x00010008u
#define  USBH_HID_CA_SYSTEM_CTRL                  0x00010080u


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         FORWARD DECLARATION
*********************************************************************************************************
*/

typedef  struct  usbh_hid_coll  USBH_HID_COLL;


/*
*********************************************************************************************************
*                                           HID DESCRIPTOR
*
* Note(s) : (1) See 'Device Class Definition for Human Interface Devices (HID), 6/27/01, Version 1.11',
*               section 6.2.1 for more details about HID descriptor.
*********************************************************************************************************
*/

typedef  struct  usbh_hid_desc {
    CPU_INT08U  bLength;
    CPU_INT08U  bDescriptorType;
    CPU_INT16U  bcdHID;
    CPU_INT08U  bCountryCode;
    CPU_INT08U  bNbrDescriptors;
    CPU_INT08U  bClassDescriptorType;
    CPU_INT16U  wClassDescriptorLength;
} USBH_HID_DESC;


/*
*********************************************************************************************************
*                                            REPORT FORMAT
*
* Note(s) : (1) See 'Device Class Definition for Human Interface Devices (HID), 6/27/01, Version 1.11',
*               section 6.2.2 for more details about HID reports.
*********************************************************************************************************
*/

typedef  struct  usbh_hid_report_fmt {
    CPU_INT08U  ReportID;
    CPU_INT08U  ReportType;                                     /* Report type (INPUT / OUTPUT / FEATURE).              */
    CPU_INT16U  UsagePage;
    CPU_INT32U  Usage[USBH_HID_CFG_MAX_NBR_USAGE];
    CPU_INT32U  NbrUsage;
    CPU_INT32U  UsageMin;
    CPU_INT32U  UsageMax;
    CPU_INT32S  LogMin;
    CPU_INT32S  LogMax;
    CPU_INT32S  PhyMin;
    CPU_INT32S  PhyMax;
    CPU_INT32S  UnitExp;
    CPU_INT32U  Unit;
    CPU_INT32U  ReportSize;                                     /* Report size (bits).                                  */
    CPU_INT32U  ReportCnt;
    CPU_INT32U  Flag;                                           /* Flag (Data/constant, var/array ..).                  */
    CPU_INT32U  PhyUsage;                                       /* Physical usage of this report.                       */
    CPU_INT32U  AppUsage;                                       /* Application usage of this report.                    */
    CPU_INT32U  LogUsage;                                       /* Logical usage of this report.                        */
} USBH_HID_REPORT_FMT;


/*
*********************************************************************************************************
*                                            HID REPORT ID
*
* Note(s) : (1) See 'Device Class Definition for Human Interface Devices (HID), 6/27/01, Version 1.11',
*               section 5.6 for more details about HID report ID.
*********************************************************************************************************
*/

typedef  struct  usbh_hid_report_id {
    CPU_INT32U  Size;                                           /* Report size in bits.                                 */
    CPU_INT08U  ReportID;
    CPU_INT08U  Type;                                           /* Report type (INPUT / OUTPUT / FEATURE).              */
} USBH_HID_REPORT_ID;


/*
*********************************************************************************************************
*                                           HID COLLECTION
*
* Note(s) : (1) See 'Device Class Definition for Human Interface Devices (HID), 6/27/01, Version 1.11',
*               section 6.2.2.6 for more details about HID collection.
*********************************************************************************************************
*/

struct  usbh_hid_coll {
    CPU_INT32U      Usage;                                      /* Coll usage.                                          */
    CPU_INT08U      Type;                                       /* Coll type (App / Physical / Logical).                */
    USBH_HID_COLL  *NextPtr;
};

typedef  struct  usbh_hid_app_coll {
    CPU_INT32U           Usage;                                 /* Coll usage.                                          */
    CPU_INT08U           Type;                                  /* Coll type (App / Physical / Logical).                */
    CPU_INT08U           NbrReportFmt;                          /* Nbr of report fmts.                                  */

                                                                /* Array of report fmts.                                */
    USBH_HID_REPORT_FMT  ReportFmt[USBH_HID_CFG_MAX_NBR_REPORT_FMT];
} USBH_HID_APP_COLL;


                                                                /* --- APPLICATION REPORT RECEIVE CALLBACK FUNCTION --- */
typedef  void  (*USBH_HID_RXCB_FNCT)(void        *p_arg,
                                     void        *p_buf,
                                     CPU_INT08U   buf_len,
                                     USBH_ERR     err);


                                                                /* ----------- HID REPORT RECEIVE CALLBACK ------------ */
typedef  struct  usbh_hid_rxcb {
    CPU_BOOLEAN          InUse;
    CPU_INT08U           ReportID;
    void                *AsyncArgPtr;
    USBH_HID_RXCB_FNCT   AsyncFnct;
} USBH_HID_RXCB;


                                                                /* -------------------- HID DEVICE -------------------- */
typedef  struct  usbh_hid_dev {
    USBH_DEV            *DevPtr;                                /* Ptr to dev struct.                                   */
    USBH_IF             *IfPtr;                                 /* Ptr to IF struct.                                    */
    CPU_INT08U           IfNbr;                                 /* HID IF nbr of dev.                                   */
    USBH_EP              IntrInEP;                              /* Intr IN EP.                                          */
    USBH_EP              IntrOutEP;                             /* Intr OUT EP.                                         */
    CPU_BOOLEAN          IsOutEP_Present;                       /* Indicate if intr OUT EP is present on dev.           */

    CPU_INT08U           State;
    CPU_INT08U           AppRefCnt;                             /* Cnt of app ref on this dev.                          */
    USBH_HMUTEX          HMutex;

    CPU_INT08U           SubClass;                              /* HID dev IF subclass code.                            */
    CPU_INT08U           Protocol;                              /* HID dev IF protocol code.                            */
    USBH_HID_DESC        Desc;                                  /* HID desc content.                                    */

    CPU_INT08U           NbrAppColl;                            /* Nbr of app coll in main item.                        */
    USBH_HID_APP_COLL    AppColl[USBH_HID_CFG_MAX_NBR_APP_COLL];/* App coll in main item.                               */
    CPU_INT08U           NbrReportID;                           /* Tot nbr of report ID.                                */

                                                                /* Report ID of all colls.                              */
    USBH_HID_REPORT_ID   ReportID[USBH_HID_CFG_MAX_NBR_REPORT_ID];

    USBH_HID_RXCB        RxCB[USBH_HID_CFG_MAX_NBR_RXCB];

                                                                /* Rx/Tx buf, +1 for report id.                            */
    CPU_INT08U           RxBuf[USBH_HID_CFG_MAX_RX_BUF_SIZE + 1u];
    CPU_INT08U           TxBuf[USBH_HID_CFG_MAX_TX_BUF_SIZE + 1u];
    CPU_INT08U           ErrCnt;                                /* Rx error cnt.                                        */
    CPU_INT08U           Boot;                                  /* Is it a boot HID dev?                                */
    CPU_BOOLEAN          IsInit;                                /* Indicate if HID class instance is correctly init.    */
    CPU_BOOLEAN          RxInProg;                              /* Async rx is in progress.                             */

                                                                /* HID report desc content.                             */
    CPU_INT08U           ReportDesc[USBH_HID_CFG_MAX_REPORT_DESC_LEN];
    USBH_HID_REPORT_ID  *MaxReportPtr;                          /* Ptr to largest report.                               */

    CPU_INT32U           Usage;                                 /* HID device usage.                                    */
} USBH_HID_DEV;




/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/

USBH_HID_EXT  USBH_CLASS_DRV  USBH_HID_ClassDrv;                /* HID Class drv API.                                   */


/*
*********************************************************************************************************
*                                               MACROS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

USBH_ERR     USBH_HID_Init            (USBH_HID_DEV         *p_hid_dev);

USBH_ERR     USBH_HID_RefAdd          (USBH_HID_DEV         *p_hid_dev);

USBH_ERR     USBH_HID_RefRel          (USBH_HID_DEV         *p_hid_dev);

USBH_ERR     USBH_HID_GetReportIDArray(USBH_HID_DEV         *p_hid_dev,
                                       USBH_HID_REPORT_ID  **p_report_id,
                                       CPU_INT08U           *p_nbr_report_id);

USBH_ERR     USBH_HID_GetAppCollArray (USBH_HID_DEV         *p_hid_dev,
                                       USBH_HID_APP_COLL   **p_app_coll,
                                       CPU_INT08U           *p_nbr_app_coll);

CPU_BOOLEAN  USBH_HID_IsBootDev       (USBH_HID_DEV         *p_hid_dev,
                                       USBH_ERR             *p_err);

CPU_INT08U   USBH_HID_RxReport        (USBH_HID_DEV         *p_hid_dev,
                                       CPU_INT08U            report_id,
                                       void                 *p_buf,
                                       CPU_INT08U            buf_len,
                                       CPU_INT16U            timeout_ms,
                                       USBH_ERR             *p_err);

CPU_INT08U   USBH_HID_TxReport        (USBH_HID_DEV         *p_hid_dev,
                                       CPU_INT08U            report_id,
                                       void                 *p_buf,
                                       CPU_INT08U            buf_len,
                                       CPU_INT16U            timeout_ms,
                                       USBH_ERR             *p_err);

USBH_ERR     USBH_HID_RegRxCB         (USBH_HID_DEV         *p_hid_dev,
                                       CPU_INT08U            report_id,
                                       USBH_HID_RXCB_FNCT    async_fnct,
                                       void                 *p_async_arg);

USBH_ERR     USBH_HID_UnregRxCB       (USBH_HID_DEV         *p_hid_dev,
                                       CPU_INT08U            report_id);

USBH_ERR     USBH_HID_ProtocolSet     (USBH_HID_DEV         *p_hid_dev,
                                       CPU_INT16U            protocol);

USBH_ERR     USBH_HID_ProtocolGet     (USBH_HID_DEV         *p_hid_dev,
                                       CPU_INT16U           *p_protocol);

USBH_ERR     USBH_HID_IdleSet         (USBH_HID_DEV         *p_hid_dev,
                                       CPU_INT08U            report_id,
                                       CPU_INT32U            dur);

CPU_INT32U   USBH_HID_IdleGet         (USBH_HID_DEV         *p_hid_dev,
                                       CPU_INT08U            report_id,
                                       USBH_ERR             *p_err);


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
