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
* Filename : can_msg.h
* Version  : V2.42.01
* Purpose  : This include file defines the symbolic constants for the CAN message database.
*********************************************************************************************************
*/

#ifndef _CAN_MSG_H_
#define _CAN_MSG_H_

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

#if CANMSG_EN > 0


/*-----------------------------------------------------------------------------------------------------*/
/*!
* \brief                      TYPE: NOT USED
*
*           This define holds the coding for the information: 'message not used'. This status
*           is the default status, before the CAN message initialization function is called.
*           After the CAN message initialization, this status remains in the CAN message interface
*           slots, which is not configured via the configuration structure.
*/
/*-----------------------------------------------------------------------------------------------------*/

#define CANMSG_UNUSED        0x00


/*-----------------------------------------------------------------------------------------------------*/
/*!
* \brief                      TYPE: TX MESSAGE
*
*           This define holds the coding for the information: 'transmit message'. This status
*           will be set, after a signal is created with a corresponding transmit configuration.
*/
/*-----------------------------------------------------------------------------------------------------*/

#define CANMSG_TX            0x01u


/*-----------------------------------------------------------------------------------------------------*/
/*!
* \brief                      TYPE: RX MESSAGE
*
*           This define holds the coding for the information: 'receive message'. This status
*           will be set, after a signal is created with a corresponding receive configuration.
*/
/*-----------------------------------------------------------------------------------------------------*/

#define CANMSG_RX            0x02u


/*-----------------------------------------------------------------------------------------------------*/
/*!
* \brief                      TYPE MASK FOR USER TYPES
*
*           This define holds the mask for the user definable type informations. This mask
*           is used to remove these bits within the CAN signal management. These bits are
*           free for application layer message management.
*/
/*-----------------------------------------------------------------------------------------------------*/

#define CANMSG_USER          0xF0u


/*-----------------------------------------------------------------------------------------------------*/
/*!
* \brief                      MAX NUMBER OF LINKS
*
*           This define holds the maximal number of linked signals per message.
*/
/*-----------------------------------------------------------------------------------------------------*/

#define CANMSG_MAX_LINK      8u

/*-----------------------------------------------------------------------------------------------------*/
/*!
* \brief                      I/O CONTROL FUNCTIONCODES
*
*           This enumeration defines the functioncode values for the function CanMsgIoCtl().
*/
/*-----------------------------------------------------------------------------------------------------*/

enum CANMSG_IOCTL_FUNC {
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                  IS MESSAGE CHANGED
    *
    *       With this function code the IO control will check all linked signals for a change.
    *       If at least one signal is changed, the argument pointer will be used to set the
    *       boolen return status to TRUE, otherwise the return status will be set to FALSE.
    *
    * \note Argument pointer type: CPU_BOOLEAN *
    */
    /*-------------------------------------------------------------------------------------------------*/
    CANMSG_IS_CHANGED = 0
};


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/


/*-----------------------------------------------------------------------------------------------------*/
/*!
* \brief                      SIGNAL MAP
*
*           This structure holds all informations, which are needed for the link from
*           CAN signals to the data position in the CAN message.
*/
/*-----------------------------------------------------------------------------------------------------*/

typedef struct {
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                  SIGNAL ID
    *
    *       This member holds the unique signal identifier, which shall be linked to the message.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT16U Id;
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                  POSITION
    *
    *       This member holds position of the first bit or byte, which is used in the can frame.
    *       Interpreting the position in bit or byte depends on the configuration
    *       setting 'CANSIG_GRANULARITY'.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT08U Pos;

} CANMSG_LINK;


/*-----------------------------------------------------------------------------------------------------*/
/*!
* \brief                      MESSAGE CONFIGURATION
*
*           This structure contains the informations for a CAN signal. A signal represents
*           a piece of information within the CAN payload (a single bit, a bitfield,
*           a integer value, etc...).
*
* \note     For systems with very limited amount of RAM, this structure can be placed in
*           ROM by declaring (and initializing) a const-variable during compile-time.
*/
/*-----------------------------------------------------------------------------------------------------*/

typedef struct {
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                  IDENTIFIER
    *
    *       This member holds the identifier of the message.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT32U Identifier;
    /*-------------------------------------------------------------------------------------------------*/
    /*!
     * \brief                 TYPE
     *
     *      This member holds the type of the message.
     */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT08U Type;
    /*-------------------------------------------------------------------------------------------------*/
    /*!
     * \brief                 DATA LENGTH CODE
     *
     *      This member holds the DLC of the message.
     */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT08U DLC;
    /*-------------------------------------------------------------------------------------------------*/
    /*!
     * \brief                 NUMBER OF SIGNALS
     *
     *      This member holds the used number of signals in the following link table.
     */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT08U SigNum;
    /*-------------------------------------------------------------------------------------------------*/
    /*!
     * \brief                 LINK TABLE
     *
     *      This array holds the linked signal for this message.
     */
    /*-------------------------------------------------------------------------------------------------*/
    CANMSG_LINK SigLst[CANMSG_MAX_LINK];

} CANMSG_PARA;


/*-----------------------------------------------------------------------------------------------------*/
/*!
* \brief                      CAN MESSAGE OBJECT
*
*           This structure contains the dynamic informations for a CAN message.
*/
/*-----------------------------------------------------------------------------------------------------*/

typedef struct
{
    /*-------------------------------------------------------------------------------------------------*/
    /*!
     * \brief                 MESSAGE IDENTIFIER
     *
     *      This member holds the system wide unique message identifier.
     */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT16U Id;
    /*-------------------------------------------------------------------------------------------------*/
    /*!
     * \brief                 CONFIGURATION LINK
     *
     *      This member holds a pointer to the corresponding message configuration.
     */
    /*-------------------------------------------------------------------------------------------------*/
    CANMSG_PARA *Cfg;
    /*-------------------------------------------------------------------------------------------------*/
    /*!
     * \brief                 MESSAGE LIST LINK
     *
     *      This member points to the next CAN message object, or contains NULL for the end
     *      of the list.
     */
    /*-------------------------------------------------------------------------------------------------*/
    void *Next;

} CANMSG_DATA;


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

CPU_INT16S  CanMsgInit  (CPU_INT32U    arg);

CPU_INT16S  CanMsgOpen  (CPU_INT16S    drvId,
                         CPU_INT32U    devName,
                         CPU_INT16U    mode);

CPU_INT16S  CanMsgIoCtl (CPU_INT16S    msgId,
                         CPU_INT16U    func,
                         void         *argp);

CPU_INT16S  CanMsgRead  (CPU_INT16S    msgId,
                         void         *buffer,
                         CPU_INT16U    size);

CPU_INT16S  CanMsgWrite (CPU_INT16S    msgId,
                         void         *buffer,
                         CPU_INT16U    size);

CPU_INT16S  CanMsgCreate(CANMSG_PARA  *cfg);

CPU_INT16S  CanMsgDelete(CPU_INT16S    msgId);

#endif  /* CANMSG_EN > 0 */

#ifdef __cplusplus
}
#endif


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                /* #ifndef _CAN_MSG_H_                           */
