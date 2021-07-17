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
* Filename : can_bus.h
* Version  : V2.42.01
* Purpose  : This include file defines the symbolic constants and function prototypes
*            for the buffered CAN bus handling.
*********************************************************************************************************
*/

#ifndef _CAN_BUS_H_
#define _CAN_BUS_H_

#ifdef __cplusplus
extern "C" {
#endif


/*
*********************************************************************************************************
*                                              INCLUDES
*********************************************************************************************************
*/

#include "can_os.h"                                   /* operating system abstraction             */
#include "can_drv.h"                                  /* CAN driver abstraction                   */
#include "drv_def.h"                                  /* Device driver definitions                */
#include "can_cfg.h"                                  /* configuration settings uC/Can            */


#if CANBUS_EN > 0


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

/*-----------------------------------------------------------------------------------------------------*/
/*!
* \brief                      STATUS: NOT USED
* \ingroup  UCCAN
*
*           This define holds the coding for the information: 'bus not used'. This status
*           is the default status, before the CAN bus initialization function is called.
*           After the CAN bus initialization, this status remains in the CAN bus interface
*           slots, which is not configured via the configuration structure.
*/
/*-----------------------------------------------------------------------------------------------------*/

#define CANBUS_UNUSED        0x00


/*-----------------------------------------------------------------------------------------------------*/
/*!
* \brief                      STATUS: ACTIVE
* \ingroup  UCCAN
*
*           This define holds the coding for the information: 'bus is active'. This status
*           is the default status, after the CAN bus initialization function is called.
*           This status indicates, that the bus is ready for communication.
*/
/*-----------------------------------------------------------------------------------------------------*/

#define CANBUS_ACTIVE        0x01


/*-----------------------------------------------------------------------------------------------------*/
/*!
* \brief                      STATUS: PASSIVE
* \ingroup  UCCAN
*
*           This define holds the coding for the information: 'bus is passive'. This status
*           will be set, if a node status change to 'passive' is detected.
*/
/*-----------------------------------------------------------------------------------------------------*/

#define CANBUS_PASSIVE       0x02


/*-----------------------------------------------------------------------------------------------------*/
/*!
* \brief                      STATUS: ERROR
* \ingroup  UCCAN
*
*           This define holds the coding for the information: 'bus error detected'. This
*           status will be set, if a node status change to 'error' or 'bus-off' is detected.
*/
/*-----------------------------------------------------------------------------------------------------*/

#define CANBUS_ERROR         -3

/*-----------------------------------------------------------------------------------------------------*/
/*!
* \brief                      I/O CONTROL FUNCTIONCODES
* \ingroup  UCCAN
*
*           This enumeration defines the functioncode values for the function CanBusIoCtl().
*/
/*-----------------------------------------------------------------------------------------------------*/

enum CANBUS_IOCTL_FUNC {
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                  RESET BUS
    * \ingroup  UCCAN
    *
    *       This enum value is the function code to reset the bus interface.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CANBUS_RESET = 0,
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                  FLUSH TX QUEUE
    * \ingroup  UCCAN
    *
    *       This enum value is the functioncode to clear the transmission queue.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CANBUS_FLUSH_TX,
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                  FLUSH RX QUEUE
    * \ingroup  UCCAN
    *
    *       This enum value is the functioncode to clear the receive queue.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CANBUS_FLUSH_RX,
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                  SET TX TIMEOUT
    * \ingroup  UCCAN
    *
    *       This enum value is the functioncode to set the transmit timeout during waiting for a
    *       free entry in the transmission queue.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CANBUS_SET_TX_TIMEOUT,
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                  SET RX TIMEOUT
    * \ingroup  UCCAN
    *
    *       This enum value is the functioncode to set the receive timeout during waiting for a
    *       received CAN frame.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CANBUS_SET_RX_TIMEOUT
};


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/

/*-----------------------------------------------------------------------------------------------------*/
/*!
* \brief                      CAN BUS CONFIGURATION
* \ingroup  UCCAN
*
*           This structure contains the informations for a bus. A bus represents one
*           interface to the world.
*
* \note     For systems with very limited amount of RAM, this structure can be placed in
*           ROM by declaring (and initializing) a const-variable during compile-time.
*/
/*-----------------------------------------------------------------------------------------------------*/

typedef struct {
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                  EXTENDED FLAG
    * \ingroup  UCCAN
    *
    *       This member holds the default configuration for the receive buffer.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_BOOLEAN Extended;
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                  BAUDRATE
    * \ingroup  UCCAN
    *
    *       This member holds the baudrate in bit/s.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT32U Baudrate;
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                  BUS NODE
    * \ingroup  UCCAN
    *
    *       This member holds the bus node name, which must be used to interface with
    *       the can bus layer.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT32U BusNodeName;
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                  DRIVER DEVICE
    * \ingroup  UCCAN
    *
    *       This member holds the driver device name, which must be used to open the interface with
    *       the lowlevel device driver.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT32U DriverDevName;
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                  DRIVER INIT FUNCTION
    * \ingroup  UCCAN
    *
    *       This member holds the function pointer to the CAN lowlevel device driver Init()
    *       function.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT16S (*Init)(CPU_INT32U arg);
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                  DRIVER OPEN FUNCTION
    * \ingroup  UCCAN
    *
    *       This member holds the function pointer to the CAN lowlevel device driver Open()
    *       function.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT16S (*Open)(CPU_INT16S devId, CPU_INT32U devName, CPU_INT16U mode);
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                  DRIVER CLOSE FUNCTION
    * \ingroup  UCCAN
    *
    *       This member holds the function pointer to the CAN lowlevel device driver Close()
    *       function.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT16S (*Close)(CPU_INT16S paraId);
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                  DRIVER IOCTL FUNCTION
    * \ingroup  UCCAN
    *
    *       This member holds the function pointer to the CAN lowlevel device driver IoCtl()
    *       function.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT16S (*IoCtl)(CPU_INT16S paraId, CPU_INT16U func, void * argp);
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                  DRIVER READ FUNCTION
    * \ingroup  UCCAN
    *
    *       This member holds the function pointer to the CAN lowlevel device driver Read()
    *       function.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT16S (*Read)(CPU_INT16S paraId, CPU_INT08U * buffer, CPU_INT16U size);
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                  DRIVER WRITE FUNCTION
    * \ingroup  UCCAN
    *
    *       This member holds the function pointer to the CAN lowlevel device driver Write()
    *       function.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT16S (*Write)(CPU_INT16S paraId, CPU_INT08U *buffer, CPU_INT16U size);
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                  DRIVER IO FUNCTION CODES
    * \ingroup  UCCAN
    *
    *       This member array holds the needed function codes for the CAN bus layer. These function
    *       codes shall be available in the used low level device driver.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT16U Io[CAN_IO_FUNC_N];

} CANBUS_PARA;


/*-----------------------------------------------------------------------------------------------------*/
/*!
* \brief                      CAN BUS OBJECT
* \ingroup  UCCAN
*
*           This structure holds the runtime data for the CAN bus management.
*/
/*-----------------------------------------------------------------------------------------------------*/

typedef struct {
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                  CONFIG LINK
    * \ingroup  UCCAN
    *
    *       This member holds the pointer to the readonly CAN bus configuration.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CANBUS_PARA *Cfg;
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                  DEVICE
    * \ingroup  UCCAN
    *
    *       This member holds the device ID, which is returned by the CANOpen() function of the
    *       linked lowlevel device driver.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT16S Dev;
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                  RX TIMEOUT
    * \ingroup  UCCAN
    *
    *       This member holds the timeout, which is used during the CanBusRead().
    */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT16U RxTimeout;
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                  TX TIMEOUT
    * \ingroup  UCCAN
    *
    *       This member holds the timeout, which is used during the CanBusWrite().
    */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT16U TxTimeout;

	/*-------------------------------------------------------------------------------------------------*/
	/*!
	* \brief                      CAN BUS TRANSMIT FRAME BUFFER
	* \ingroup  UCCAN
	*
	*           Allocation of transmit can bus frame buffer
	*/
	/*-------------------------------------------------------------------------------------------------*/
	CANFRM     BufTx[CANBUS_TX_QSIZE];

	/*-------------------------------------------------------------------------------------------------*/
	/*!
	* \brief                      CAN BUS TRANSMIT BUFFER READ FRAME
	* \ingroup  UCCAN
	*
	*           Next read location in can bus frame buffer
	*/
	/*-------------------------------------------------------------------------------------------------*/
	CPU_INT16U BufTxRd;

	/*-------------------------------------------------------------------------------------------------*/
	/*!
	* \brief                      CAN BUS TRANSMIT BUFFER WRITE FRAME
	* \ingroup  UCCAN
	*
	*           Next write location in can bus frame buffer
	*/
	/*-------------------------------------------------------------------------------------------------*/
	CPU_INT16U BufTxWr;

	/*-------------------------------------------------------------------------------------------------*/
	/*!
	* \brief                      CAN BUS RECEIVE FRAME BUFFER
	* \ingroup  UCCAN
	*
	*           Allocation of receive can bus frame buffer
	*/
	/*-------------------------------------------------------------------------------------------------*/
	CANFRM     BufRx[CANBUS_RX_QSIZE];

	/*-------------------------------------------------------------------------------------------------*/
	/*!
	* \brief                      CAN BUS RECEIVE BUFFER READ FRAME
	* \ingroup  UCCAN
	*
	*           Next read location in can bus frame buffer
	*/
	/*-------------------------------------------------------------------------------------------------*/
	CPU_INT16U BufRxRd;

	/*-------------------------------------------------------------------------------------------------*/
	/*!
	* \brief                      CAN BUS RECEIVE BUFFER WRITE FRAME
	* \ingroup  UCCAN
	*
	*           Next write location in can bus frame buffer
	*/
	/*-------------------------------------------------------------------------------------------------*/
	CPU_INT16U BufRxWr;

#if CANBUS_STAT_EN > 0
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                  RECEIVED FRAMES
    * \ingroup  UCCAN
    *
    *       This member holds the counter, which will be incremented every received CAN frame.
    *
    * \note This counter is incremented, when the CanBusRead() function successfully gives the
    *       CAN frame to the application layer.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT16U RxOk;
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                  TRANSMITTED FRAMES
    * \ingroup  UCCAN
    *
    *       This member holds the counter, which will be incremented every transmitted CAN frame.
    *
    * \note This counter is incremented, when the CAN transmit complete interrupt indicates,
    *       that the CAN frame is sent to the bus.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT16U TxOk;
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                  LOST RECEIVE FRAMES
    * \ingroup  UCCAN
    *
    *       This member holds the counter, which will be incremented for every received CAN frame
    *       which can not transfered to the application, due to a full receive queue.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT16U RxLost;
    /*-------------------------------------------------------------------------------------------------*/
    /*!
    * \brief                  LOST RECEIVE FRAMES
    * \ingroup  UCCAN
    *
    *       This member holds the counter, which will be incremented for every transmission CAN
    *       frame, which can not transfered to the transmit interrupt handler due to a full
    *       transmission queue.
    */
    /*-------------------------------------------------------------------------------------------------*/
    CPU_INT16U TxLost;
#endif /* CANBUS_STAT_EN > 0 */

} CANBUS_DATA;


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

CPU_INT16S  CanBusInit     (CPU_INT32U    arg);

CPU_INT16S  CanBusIoCtl    (CPU_INT16S    busId,
                            CPU_INT16U    func,
                            void         *argp);

CPU_INT16S  CanBusRead     (CPU_INT16S    busId,
                            void         *buffer,
                            CPU_INT16U    size);

CPU_INT16S  CanBusWrite    (CPU_INT16S    busId,
                            void         *buffer,
                            CPU_INT16U    size);

CPU_INT16S  CanBusEnable   (CANBUS_PARA  *cfg);

CPU_INT16S  CanBusDisable  (CPU_INT16S    busId);

void        CanBusTxHandler(CPU_INT16S    busId);

void        CanBusRxHandler(CPU_INT16S    busId);

void        CanBusNSHandler(CPU_INT16S    busId);

#if CANBUS_HOOK_NS_EN == 1
void        CanBusNSHook    (CPU_INT16S    busId);    /* hooks are found in can_cfg.c */
#endif
#if CANBUS_HOOK_RX_EN == 1
CPU_INT16S  CanBusRxHook    (CPU_INT16S    busId,
                            void *buffer);
#endif


#endif  /* CANBUS_EN > 0 */

#ifdef __cplusplus
}
#endif


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                /* #ifndef _CAN_BUS_H_                           */
