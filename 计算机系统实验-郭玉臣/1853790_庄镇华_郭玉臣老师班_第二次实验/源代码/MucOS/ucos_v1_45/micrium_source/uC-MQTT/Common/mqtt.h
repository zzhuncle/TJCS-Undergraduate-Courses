/*
*********************************************************************************************************
*                                               uC/MQTT
*                                Message Queue Telemetry Transport Client
*
*                         (c) Copyright 2014-2017; Micrium, Inc.; Weston, FL
*
*                  All rights reserved.  Protected by international copyright laws.
*
*                  uC/MQTT is provided in source form to registered licensees ONLY.  It is
*                  illegal to distribute this source code to any third party unless you receive
*                  written permission by an authorized Micrium representative.  Knowledge of
*                  the source code may NOT be used to develop a similar product.
*
*                  Please help us continue to provide the Embedded community with the finest
*                  software available.  Your honesty is greatly appreciated.
*
*                  You can find our product's user manual, API reference, release notes and
*                  more information at: https://doc.micrium.com
*
*                  You can contact us at: http://www.micrium.com
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                             MQTT DEF
*
* Filename      : mqtt.h
* Version       : V1.01.00
* Programmer(s) : OD
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This main network protocol suite header file is protected from multiple pre-processor
*               inclusion through use of the MQTTc module present pre-processor macro definition.
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  MQTT_DEF_MODULE_PRESENT
#define  MQTT_DEF_MODULE_PRESENT

/*
*********************************************************************************************************
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#define    MICRIUM_SOURCE
#define    MQTT_DEF_MODULE
#include  <lib_def.h>
#include  <cpu.h>


/*
*********************************************************************************************************
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            MSG LEN DEFINES
*********************************************************************************************************
*/

#define  MQTT_MSG_BASE_LEN                                         4u
#define  MQTT_MSG_PING_DISCONN_LEN                                 2u


/*
*********************************************************************************************************
*                                           MSG TYPE DEFINES
*********************************************************************************************************
*/

#define  MQTT_MSG_TYPE_BIT_SHIFT                                   4u
#define  MQTT_MSG_TYPE_MSK                                          DEF_BIT_FIELD( 4u, MQTT_MSG_TYPE_BIT_SHIFT)
#define  MQTT_MSG_TYPE_CONNECT                                   ( 1u << MQTT_MSG_TYPE_BIT_SHIFT)           /* Tx only. */
#define  MQTT_MSG_TYPE_CONNACK                                   ( 2u << MQTT_MSG_TYPE_BIT_SHIFT)           /* Rx only. */
#define  MQTT_MSG_TYPE_PUBLISH                                   ( 3u << MQTT_MSG_TYPE_BIT_SHIFT)           /* Both.    */
#define  MQTT_MSG_TYPE_PUBACK                                    ( 4u << MQTT_MSG_TYPE_BIT_SHIFT)           /* Both.    */
#define  MQTT_MSG_TYPE_PUBREC                                    ( 5u << MQTT_MSG_TYPE_BIT_SHIFT)           /* Both.    */
#define  MQTT_MSG_TYPE_PUBREL                                    ( 6u << MQTT_MSG_TYPE_BIT_SHIFT)           /* Both.    */
#define  MQTT_MSG_TYPE_PUBCOMP                                   ( 7u << MQTT_MSG_TYPE_BIT_SHIFT)           /* Both.    */
#define  MQTT_MSG_TYPE_SUBSCRIBE                                 ( 8u << MQTT_MSG_TYPE_BIT_SHIFT)           /* Tx only. */
#define  MQTT_MSG_TYPE_SUBACK                                    ( 9u << MQTT_MSG_TYPE_BIT_SHIFT)           /* Rx only. */
#define  MQTT_MSG_TYPE_UNSUBSCRIBE                               (10u << MQTT_MSG_TYPE_BIT_SHIFT)           /* Tx only. */
#define  MQTT_MSG_TYPE_UNSUBACK                                  (11u << MQTT_MSG_TYPE_BIT_SHIFT)           /* Rx only. */
#define  MQTT_MSG_TYPE_PINGREQ                                   (12u << MQTT_MSG_TYPE_BIT_SHIFT)           /* Tx only. */
#define  MQTT_MSG_TYPE_PINGRESP                                  (13u << MQTT_MSG_TYPE_BIT_SHIFT)           /* Rx only. */
#define  MQTT_MSG_TYPE_DISCONNECT                                (14u << MQTT_MSG_TYPE_BIT_SHIFT)           /* Tx only. */


/*
*********************************************************************************************************
*                                         MSG FIXED HDR DEFINES
*********************************************************************************************************
*/

                                                                /* ----------------------- DUP ------------------------ */
#define  MQTT_MSG_FIXED_HDR_FLAGS_DUP_BIT_SHIFT                    3u
#define  MQTT_MSG_FIXED_HDR_FLAGS_DUP_MSK                           DEF_BIT(MQTT_MSG_FIXED_HDR_FLAGS_DUP_BIT_SHIFT)

                                                                /* ----------------------- QoS ------------------------ */
#define  MQTT_MSG_FIXED_HDR_FLAGS_QOS_LVL_BIT_SHIFT                1u
#define  MQTT_MSG_FIXED_HDR_FLAGS_QOS_LVL_MSK                       DEF_BIT_FIELD( 2u, MQTT_MSG_FIXED_HDR_FLAGS_QOS_LVL_BIT_SHIFT)
#define  MQTT_MSG_FIXED_HDR_FLAGS_QOS_LVL_0                    (0x00u << MQTT_MSG_FIXED_HDR_FLAGS_QOS_LVL_BIT_SHIFT)
#define  MQTT_MSG_FIXED_HDR_FLAGS_QOS_LVL_1                    (0x01u << MQTT_MSG_FIXED_HDR_FLAGS_QOS_LVL_BIT_SHIFT)
#define  MQTT_MSG_FIXED_HDR_FLAGS_QOS_LVL_2                    (0x02u << MQTT_MSG_FIXED_HDR_FLAGS_QOS_LVL_BIT_SHIFT)
#define  MQTT_MSG_QOS_LVL_MAX                                      2u

                                                                /* ---------------------- RETAIN ---------------------- */
#define  MQTT_MSG_FIXED_HDR_FLAGS_RETAIN_BIT_SHIFT                 0u
#define  MQTT_MSG_FIXED_HDR_FLAGS_RETAIN_MSK                        DEF_BIT(MQTT_MSG_FIXED_HDR_FLAGS_RETAIN_BIT_SHIFT)

                                                                /* --------------------- REM LEN ---------------------- */
#define  MQTT_MSG_FIXED_HDR_REM_LEN_MSK                             DEF_BIT_FIELD( 7u, 0u)
#define  MQTT_MSG_FIXED_HDR_REM_LEN_MAX_LEN                      128u
#define  MQTT_MSG_FIXED_HDR_REM_LEN_CONTINUATION_BIT                DEF_BIT_07
#define  MQTT_MSG_FIXED_HDR_REM_LEN_NBR_BYTES_MAX                  4u
#define  MQTT_MSG_FIXED_HDR_REM_LEN_MAX                    268435455u

                                                                /* --------------- FIXED HDR TOTAL LEN ---------------- */
#define  MQTT_MSG_FIXED_HDR_MAX_LEN_BYTES                          5u


/*
*********************************************************************************************************
*                                          MSG VAR HDR DEFINES
*********************************************************************************************************
*/

#define  MQTT_MSG_VAR_HDR_PROTOCOL_NAME_STR                        "MQTT"
#define  MQTT_MSG_VAR_HDR_PROTOCOL_NAME_LEN                        4u
#define  MQTT_MSG_VAR_HDR_PROTOCOL_VERSION                         4u


/*
*********************************************************************************************************
*                                    VAR HDR FOR CONNECT MSG DEFINES
*********************************************************************************************************
*/

                                                                /* ------------------- VAR HDR LEN -------------------- */
#define  MQTT_MSG_VAR_HDR_CONNECT_LEN                               MQTT_MSG_VAR_HDR_PROTOCOL_NAME_LEN + 6u

                                                                /* -------------- VAR HDR CONNECT FLAGS --------------- */
#define  MQTT_MSG_VAR_HDR_CONNECT_FLAG_CLEAN_SESSION                DEF_BIT_01
#define  MQTT_MSG_VAR_HDR_CONNECT_FLAG_WILL_FLAG                    DEF_BIT_02
#define  MQTT_MSG_VAR_HDR_CONNECT_FLAG_WILL_QOS_BIT_SHIFT          3u
#define  MQTT_MSG_VAR_HDR_CONNECT_FLAG_WILL_QOS                    (DEF_BIT_04 | DEF_BIT_03)
#define  MQTT_MSG_VAR_HDR_CONNECT_FLAG_WILL_RETAIN                  DEF_BIT_05
#define  MQTT_MSG_VAR_HDR_CONNECT_FLAG_PSWD_FLAG                    DEF_BIT_06
#define  MQTT_MSG_VAR_HDR_CONNECT_FLAG_USER_NAME_FLAG               DEF_BIT_07

                                                                /* -------------- VAR HDR CLIENT ID LEN --------------- */
#define  MQTT_MSG_VAR_HDR_CONNECT_CLIENT_ID_MAX_STR_LEN           23u


/*
*********************************************************************************************************
*                                    VAR HDR FOR CONNACK MSG DEFINES
*********************************************************************************************************
*/

                                                                /* ------------ VAR HDR CONNACK RET CODES ------------- */
#define  MQTT_MSG_VAR_HDR_CONNACK_RET_CODE_ACCEPTED                0u
#define  MQTT_MSG_VAR_HDR_CONNACK_RET_CODE_UNACCEPTABLE_VERSION    1u
#define  MQTT_MSG_VAR_HDR_CONNACK_RET_CODE_ID_REJECTED             2u
#define  MQTT_MSG_VAR_HDR_CONNACK_RET_CODE_SERVER_UNAVAIL          3u
#define  MQTT_MSG_VAR_HDR_CONNACK_RET_CODE_BAD_USER_NAME_PSWD      4u
#define  MQTT_MSG_VAR_HDR_CONNACK_RET_CODE_NOT_AUTHORIZED          5u
#define  MQTT_MSG_VAR_HDR_CONNACK_RET_CODE_MAX                     5u


/*
*********************************************************************************************************
*                                            MSG ID DEFINES
*********************************************************************************************************
*/

#define  MQTT_MSG_ID_SIZE                                          2u
#define  MQTT_MSG_ID_INVALID                                       0u
#define  MQTT_MSG_ID_NONE                                          0u


/*
*********************************************************************************************************
*                                         UTF-8 UTILITY DEFINES
*********************************************************************************************************
*/

#define  MQTT_MSG_UTF8_LEN_RD(p_buf)                             (((p_buf)[0u] << 8u) | (p_buf)[1u])
#define  MQTT_MSG_UTF8_LEN_SIZE                                    2u


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*********************************************************************************************************
*/

#endif
