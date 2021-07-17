/*
*********************************************************************************************************
*                                               uC/CAN
*                                       The Embedded CAN suite
*
*                         (c) Copyright 2003-2019; Silicon Laboratories Inc.,
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
* Filename : can_sig.h
* Version  : V2.42.01
* Purpose  : This include file defines the symbolic constants for the CAN signal database.
*********************************************************************************************************
*/

#ifndef _CAN_SIG_H_
#define _CAN_SIG_H_

#ifdef __cplusplus
extern "C" {
#endif


/*
*********************************************************************************************************
*                                              INCLUDES
*********************************************************************************************************
*/

#include "cpu.h"                                      /* CPU configuration                             */
#include "can_cfg.h"                                  /* CAN abstraction module configuration          */


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

#define CANSIG_CLR_STATUS 0xF8u


/*
*********************************************************************************************************
*                                                TYPES
*********************************************************************************************************
*/

#if CANSIG_EN > 0

#if   CANSIG_MAX_WIDTH == 1u
typedef CPU_INT08U CANSIG_VAL_T;
#elif CANSIG_MAX_WIDTH == 2u
typedef CPU_INT16U CANSIG_VAL_T;
#elif CANSIG_MAX_WIDTH == 4u
typedef CPU_INT32U CANSIG_VAL_T;
#else
#error "can_sig.h: CANSIG_MAX_WIDTH is invalid; check definition to be 1, 2 or 4!"
#endif

#if CANSIG_CALLBACK_EN > 0
typedef void (*CANSIG_CALLBACK)(void* arg, CANSIG_VAL_T* value, CPU_INT32U CallbackId);
#endif


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

/*-----------------------------------------------------------------------------------------------------*/
/*! \brief                        STATUS: NOT USED
*
*           This define holds the coding for the information: 'signal not used'. This status
*           is the default status, before the CAN signal initialization function is called.
*           After the CAN signal initialization, this status remains in the CAN signal interface
*           slots, which is not configured via the configuration structure.
*/
/*-----------------------------------------------------------------------------------------------------*/

#define CANSIG_UNUSED        0x00u


/*-----------------------------------------------------------------------------------------------------*/
/*! \brief                        STATUS: NOT CHANGED
*
*           This define holds the coding for the information: 'signal is unchanged'. This status
*           will be set, after a signal access with CanSigRead().
*/
/*-----------------------------------------------------------------------------------------------------*/

#define CANSIG_UNCHANGED     0x01u


/*-----------------------------------------------------------------------------------------------------*/
/*! \brief                        STATUS: UPDATED
*
*           This define holds the coding for the information: 'signal is updated'. This status
*           will be set, after a signal write access with CanSigWrite() - independent from the
*           signal value.
*/
/*-----------------------------------------------------------------------------------------------------*/

#define CANSIG_UPDATED       0x02u


/*-----------------------------------------------------------------------------------------------------*/
/*! \brief                        STATUS: CHANGED
*
*           This define holds the coding for the information: 'signal is changed'. This status
*           will be set, when a CanSigWrite() access changes the value of the signal.
*/
/*-----------------------------------------------------------------------------------------------------*/

#define CANSIG_CHANGED       0x03u


/*-----------------------------------------------------------------------------------------------------*/
/*! \brief                        STATUS: ERROR
*
*           This define holds the coding for the information: 'signal error'.
*/
/*-----------------------------------------------------------------------------------------------------*/

#define CANSIG_ERROR         0x04u


/*-----------------------------------------------------------------------------------------------------*/
/*! \brief                        STATUS: WRITE PROTECTION ENABLED
*
*           This define holds the coding for the information: signal is write protected.
*/
/*-----------------------------------------------------------------------------------------------------*/

#define CANSIG_PROT_RO       0x40u


/*-----------------------------------------------------------------------------------------------------*/
/*! \brief                        STATUS: TIMESTAMPING DISABLED
*
*           This define holds the coding for the information: signal timestamp is disabled.
*/
/*-----------------------------------------------------------------------------------------------------*/

#define CANSIG_NO_TIMESTAMP  0x80u


/*-----------------------------------------------------------------------------------------------------*/
/*! \brief                        CALLBACK ID BITS
*
*           This define holds the coding for the information: callback identification bits
*/
/*-----------------------------------------------------------------------------------------------------*/

#define CANSIG_CALLBACK_WRITE_ID 0x00000001u
#define CANSIG_CALLBACK_READ_ID  0x00000002u


/*-----------------------------------------------------------------------------------------------------*/
/*! \brief                        I/O CONTROL FUNCTIONCODES
*
*           This enumeration defines the functioncode values for the function CanSigIoCtl().
*/
/*-----------------------------------------------------------------------------------------------------*/

enum CANSIG_IOCTL_FUNC {
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                      FUNCTIONCODE: GET WIDTH
    *
    *       This enum value is the functioncode to get the width of the signal.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CANSIG_GET_WIDTH = 1,
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                      FUNCTIONCODE: GET STATUS
    *
    *       This enum value is the functioncode to get the status of the signal.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CANSIG_GET_STATUS,
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                      FUNCTIONCODE: GET TIMESTAMP
    *
    *       This enum value is the functioncode to get the timestamp of the signal.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CANSIG_GET_TIMESTAMP,
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                      FUNCTIONCODE: GET TIME SINCE LAST UPDATE
    *
    *       This enum value is the functioncode to get the time since last update of the signal.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CANSIG_GET_TIME_SINCE_UPDATE,
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                      FUNCTIONCODE: DISABLE TIMESTAMPING
    *
    *       This enum value is the functioncode to disable the timestamping.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CANSIG_DISABLE_TIMESTAMP,                         /*!< \brief Disable timestamp for next upd.      */
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                      FUNCTIONCODE: ENABLE TIMESTAMPING
    *
    *       This enum value is the functioncode to enable the timestamping.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CANSIG_ENABLE_TIMESTAMP,
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                      FUNCTIONCODE: SET TIMESTAMP
    *
    *       This enum value is the functioncode to set the timestamp.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CANSIG_SET_TIMESTAMP,
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                      FUNCTIONCODE: GET WRITE PROTECTION
    *
    *       This enum value is the functioncode to get the status of the write protection.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CANSIG_GET_WRITE_PROTECTION,
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                      FUNCTIONCODE: SET WRITE PROTECTION
    *
    *       This enum value is the functioncode to set the signal status to write protection.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CANSIG_SET_WRITE_PROTECTION
};


/*
*********************************************************************************************************
*                                            DATA TYPES
*********************************************************************************************************
*/

/*-----------------------------------------------------------------------------------------------------*/
/*! \brief                        SIGNAL CONFIGURATION
*
*           This structure contains the configuration informations for a signal. A signal
*           represents a piece of information within the application (a single bit,
*           a bitfield, a integer value, etc...).
*
* \note     For systems with very limited amount of RAM, this structure can be placed in
*           ROM by declaring (and initializing) a const-variable during compile-time.
*/
/*-----------------------------------------------------------------------------------------------------*/

typedef struct {
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                      INITIAL STATUS
    *
    *       This member holds the initial status of the signal. This status will be copied to the
    *       signal status during initialization phase by calling CanSigInit().
    */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT08U Status;
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                      WIDTH
    *
    *       This member holds the width of the signal in bit/byte depending on CANSIG_GRANULARITY.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT08U Width;
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                      INITIAL VALUE
    *
    *       This member holds the initial value of the signal. This value will be copied to the
    *       signal value during initialization phase by calling CanSigInit().
    */
    /*-------------------------------------------------------------------------------------------------*/
    CANSIG_VAL_T Value;

#if CANSIG_CALLBACK_EN > 0
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                      CALLBACK FUNCTION
    *
    *       This member holds the function pointer to the callback function.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CANSIG_CALLBACK CallbackFct;
#endif

} CANSIG_PARA;

/*-----------------------------------------------------------------------------------------------------*/
/*! \brief                        SIGNAL OBJECT
 *
 *           This structure contains the current status informations for a signal.
 */
/*-----------------------------------------------------------------------------------------------------*/

typedef struct {
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                      CONFIG LINK
    *
    *       This member holds the pointer to the corresponding CAN signal parameters.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CANSIG_PARA *Cfg;
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                      VALUE
    *
    *       This member holds the current value of the signal.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CANSIG_VAL_T Value;
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                      STATUS
    *
    *       This member holds the current status of the signal.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT08U Status;
#if CANSIG_STATIC_CONFIG == 0
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                      TIMESTAMP
    *
    *       This member holds the timestamp of the corresponding CAN signal.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT32U   TimeStamp;
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                      NEXT LINK
    *
    *       This member holds the pointer to the next signal in the signal list.
    */
    /*-------------------------------------------------------------------------------------------------*/
    void *Next;
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                      SIGNAL ID
    *
    *       This member holds the unique signal identifier
    */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT16U Id;
#endif
} CANSIG_DATA;


/*
*********************************************************************************************************
*                                       FUNCTION PROTOTYPES
*********************************************************************************************************
*/

CPU_INT16S  CanSigIoCtl (CPU_INT16S    sigId,
                         CPU_INT16U    func,
                         void         *argp);

CPU_INT16S  CanSigWrite (CPU_INT16S    sigId,
                         void         *buffer,
                         CPU_INT16U    size);

CPU_INT16S  CanSigRead  (CPU_INT16S    sigId,
                         void         *buffer,
                         CPU_INT16U    size);

CPU_INT16S  CanSigInit  (CPU_INT32U    arg);

#if CANSIG_STATIC_CONFIG == 0
CPU_INT16S  CanSigCreate(CANSIG_PARA  *cfg);

#if CANSIG_USE_DELETE == 1
CPU_INT16S  CanSigDelete(CPU_INT16S    sigId);
#endif
#endif

#endif  /* CANSIG_EN > 0 */

#ifdef __cplusplus
}
#endif


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                /* #ifndef _CAN_SIG_H_                           */
