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
*                                           CAN DRIVER CODE
*
*                                         ZYNQ-ZC7000 Series
*
* Filename : drv_can.h
* Version  : V2.42.01
*********************************************************************************************************
*/

#ifndef  _DRV_CAN_H_
#define  _DRV_CAN_H_

/*
*********************************************************************************************************
*                                              INCLUDES
*********************************************************************************************************
*/

#include  <drv_can_reg.h>
#include  "drv_def.h"
#include  "can_bsp.h"

#include  <lib_def.h>
#include  "can_bus.h"
#include  "cpu.h"


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

#define  ZC7xxx_CAN_NAME           "Zynq ZC7000:CAN Module"     /* Unique Driver Name for Installation                  */

                                                                /* ---------------- MAILBOX SETTINGS ------------------ */
#define  ZC7xxx_CAN_Tx_Rx_FIFO                      64u         /* Tx & Rx FIFO Mailbox Buffer Size. Msg Buffering Size.*/

#define  ZC7xxx_CAN_SID_LIMIT                   0x07FFu         /* CAN Standard ID Value Limit                          */
#define  ZC7xxx_CAN_SID_MASK                0xFFE00000u         /* CAN Standard ID Location Mask Value.                 */

#define  ZC7xxx_CAN_EID_LIMIT                 0x03FFFFu         /* CAN Extended ID Value Limit                          */
#define  ZC7xxx_CAN_EID_MASK                   0x7FFFEu         /* CAN Extended ID Location Mask Value.                 */

#define  ZC7xxx_CAN_DLC_LIMIT                     0x0Fu         /* Limit DLC to 8 Bytes                                 */
#define  ZC7xxx_CAN_DLC_DATA_SPLIT                   4u         /* DLC Value for Split between Data1 & Data2 Registers. */

                                                                /* ---------------- CAN FRAME SETTINGS ---------------- */
#define  ZC7xxx_CAN_FRM_RTR_FLAG            DEF_BIT_30
#define  ZC7xxx_CAN_FRM_IDE_FLAG            DEF_BIT_29


/*
*********************************************************************************************************
*                                        DEFAULT CONFIGURATION
*********************************************************************************************************
*/

#ifndef  CAN_DIAGNOSTIC_OFF
#define  CAN_DIAGNOSTIC_OFF                          0u
#endif

#ifndef  CAN_DIAGNOSTIC_LOOPBACK
#define  CAN_DIAGNOSTIC_LOOPBACK                     1u
#endif

#ifndef  CAN_DIAGNOSTIC_SNOOP
#define  CAN_DIAGNOSTIC_SNOOP                        2u
#endif

#ifndef  CAN_DIAGNOSTIC_SELECT
#define  CAN_DIAGNOSTIC_SELECT              CAN_DIAGNOSTIC_OFF
#endif

#if    ((CAN_DIAGNOSTIC_SELECT != CAN_DIAGNOSTIC_OFF     ) && \
        (CAN_DIAGNOSTIC_SELECT != CAN_DIAGNOSTIC_LOOPBACK) && \
        (CAN_DIAGNOSTIC_SELECT != CAN_DIAGNOSTIC_SNOOP   ))
#error  "CAN_DIAGNOSTIC_SELECT must be either LoopBack Mode, Snoop Mode, or Selected to be OFF.   "
#error  "Please select CAN_DIAGNOSITC_SELECT as one of these options only.                        "
#endif


/*
*********************************************************************************************************
*                                               MACROS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            DEVICE NAMES
*
* Description : Enumeration defines the Available Device Names for the Driver XXXCANInit() Function.
*
* Note(s)     : The ZYNQ ZC7000 Series Driver only Supports 2 Internal CAN Controllers (0->1).
*********************************************************************************************************
*/

typedef  enum  zc7xxx_can_dev {
    ZC7xxx_CAN_BUS_0 = 0u,                                      /* Internal CAN controller #0                           */
    ZC7xxx_CAN_BUS_1 = 1u,                                      /* Internal CAN controller #1                           */
    ZC7xxx_CAN_N_DEV = 2u                                       /* Number of CAN controllers                            */
} ZC7xxx_CAN_DEV;


/*
*********************************************************************************************************
*                                         DRIVER ERROR CODES
*
* Description : Enumeration defines the possible Driver Error Codes.
*
* Note(s)     : none.
*********************************************************************************************************
*/

typedef  enum  zc7xxx_can_err {
    ZC7xxx_CAN_ERR_NONE = 0u,                                   /*        NO ERR: Everything is OK                      */
    ZC7xxx_CAN_ERR_BUS,                                         /*       BUS ERR: Wrong Bus Was Chosen                  */
    ZC7xxx_CAN_ERR_BUSY,                                        /*      BUSY ERR: Msg Can't be Sent, Bus is Busy        */
    ZC7xxx_CAN_ERR_INIT,                                        /*      INIT ERR: Reset State not Set, Dev Init Fail    */
    ZC7xxx_CAN_ERR_MODE,                                        /*      MODE ERR: Error Accessing Wanted Mode on Device */
    ZC7xxx_CAN_ERR_OPEN,                                        /*      OPEN ERR: Device can't be Used, Device un-Opened*/
    ZC7xxx_CAN_ERR_CLOSE,                                       /*     CLOSE ERR: Device can't be Closed                */
    ZC7xxx_CAN_ERR_FUNC,                                        /*  FUNCTION ERR: Given Function Code is not Valid      */
    ZC7xxx_CAN_ERR_ARG,                                         /*  ARGUMENT ERR: Argument Check has Failed             */
    ZC7xxx_CAN_ERR_NO_DATA,                                     /*      DATA ERR: No Data is Available                  */
} ZC7xxx_CAN_ERR;


/*
*********************************************************************************************************
*                                     I/O CONTROL FUNCTION CODES
*
* Description : Enumeration defines the available Function Codes for the Driver XXXIoCtl() Function.
*
* Note(s)     : none.
*********************************************************************************************************
*/

typedef  enum  zc7xxx_can_io_list {
    IO_ZC7xxx_CAN_GET_IDENT = 0u,                               /* ---------- GET DRIVER IDENTIFICATION CODE ---------- */
                                                                /* arg = Pointer to Local Ident Variable (CPU_INT32U)   */
    IO_ZC7xxx_CAN_GET_ERRNO,                                    /* --------------- GET DRIVER ERROR CODE -------------- */
                                                                /* arg = Pointer to Local Error Code Var. (CPU_INT16U)  */
    IO_ZC7xxx_CAN_GET_DRVNAME,                                  /* ------------------ GET DRIVER NAME ----------------- */
                                                                /* arg = Pointer to Local String Variable (char)        */
    IO_ZC7xxx_CAN_SET_BAUDRATE,                                 /* ----------------- SET BUS BAUDRATE ----------------- */
                                                                /* arg = Pointer to Local Baudrate Var. (CPU_INT32U)    */
    IO_ZC7xxx_CAN_START,                                        /* -------------------- ENABLE BUS -------------------- */
                                                                /* No Pointer: Fnct Code sets CAN to Operational Mode.  */
    IO_ZC7xxx_CAN_CONFIG,                                       /* ----------------- SET CAN TO CONFIG ---------------- */
                                                                /* No Pointer: Fnct Code sets CAN to 'CONFIG' Mode.     */
    IO_ZC7xxx_CAN_SLEEP,                                        /* ----------------- SET CAN TO SLEEP ----------------- */
                                                                /* No Pointer: Fnct Code sets CAN to 'SLEEP' Mode.      */
    IO_ZC7xxx_CAN_LBACK,                                        /* ---------------- SET CAN TO LOOPBACK --------------- */
                                                                /* No Pointer: Fnct Code sets CAN to 'LOOPBACK' Mode.   */
    IO_ZC7xxx_CAN_SNOOP,                                        /* ----------------- SET CAN TO SNOOP ----------------- */
                                                                /* No Pointer: Fnct Code sets CAN to 'SNOOP' Mode.      */
    IO_ZC7xxx_CAN_RX_STANDARD,                                  /* ------- SET  RECIEVER TO STANDARD IDENTIFIER ------- */
                                                                /* No Pointer: CAN Rx recieves only CAN Standard IDs    */
    IO_ZC7xxx_CAN_RX_EXTENDED,                                  /* ------- SET  RECIEVER TO EXTENDED IDENTIFIER ------- */
                                                                /* No Pointer: CAN Rx recieves only CAN Extended IDs    */
    IO_ZC7xxx_CAN_TX_READY,                                     /* ---------------- GET TX READY STATUS --------------- */
                                                                /* arg = Pointer to TX Rdy Status Variable (CPU_INT08U) */
    IO_ZC7xxx_CAN_GET_NODE_STATUS,                              /* ------------------ GET NODE STATUS ----------------- */
                                                                /* arg = Pointer to Node Status Variable (CPU_INT08U)   */
    IO_ZC7xxx_CAN_SET_RX_FILTER,                                /* ------------------ SET RX FILTER 1 ----------------- */
                                                                /* arg = CPU_INT32U[2]: arg[0] = mask, arg[1] = ID      */
    IO_ZC7xxx_CAN_IO_FUNC_N                                     /* ------------- NUMBER OF FUNCTION CODES ------------- */
} ZC7xxx_CAN_IO_LIST;                                           /* No Pointer: Holds number of Function Codes Available */


/*
*********************************************************************************************************
*                                          ZC7xxx CAN MODES
*
* Description : Enumeration defines the ZYNQ ZC7000 Series CAN Modes.
*
* Note(s)     : none.
*********************************************************************************************************
*/

typedef  enum  zc7xxx_can_mode {
    ZC7xxx_CAN_MODE_CONFIG = 0u,                                /* CAN Controller Set To: Configure Mode / Reset Mode.  */
    ZC7xxx_CAN_MODE_RESET,                                      /* CAN Controller Set To: Reset CAN Module.             */
    ZC7xxx_CAN_MODE_SLEEP,                                      /* CAN Controller Set To: Sleep     Mode.               */
    ZC7xxx_CAN_MODE_SNOOP,                                      /* CAN Controller Set To: Snoop     Mode.               */
    ZC7xxx_CAN_MODE_LOOP_BACK,                                  /* CAN Controller Set To: Loop Back Mode.               */
    ZC7xxx_CAN_MODE_NORMAL                                      /* CAN Controller Set To: Normal    Mode.               */
} ZC7xxx_CAN_MODE;


/*
*********************************************************************************************************
*                                         DRIVER RUNTIME DATA
*
* Description : Structure holds the Driver Runtime data.
*
* Note(s)     : (1) 'Prev_Mode' is used to Maintain the "Diagnostic" settings of the CAN controller
*                   by storing the previous CAN Operating Mode prior to entering 'Configuration'
*                   Mode to then restore the CAN Module to the previous mode. Default setting will
*                   be set to 'Normal' Mode.
*********************************************************************************************************
*/

typedef  struct  zc7xxx_can_data {
    CPU_BOOLEAN       Use;                                      /* USE MARKER: Marker Indicating if Dev is In Use       */
    ZC7xxx_CAN_REG   *RegPtr;                                   /*   REGISTER: Pointer to the CAN Base Address(s)       */
    ZC7xxx_CAN_MODE   Prev_Mode;                                /* PREV MODE : Sets Previous Mode of CAN Ctrl. Note(1). */
} ZC7xxx_CAN_DATA;


/*
*********************************************************************************************************
*                                          CAN FRAME STRUCT
*
* Description : Structure defines a CAN Frame.
*
* Note(s)     : To Differentiate between Standard and Extended IDs, the following Addition to the
*               ID is implemented: (Based on the Structure found in uC/CAN Frame files).
*                   - Bit #31     : Reserved (Always 0u)
*                   - Bit #30     : Remote Transmission Request Flag (1u = RTR, 0u = Data Frame)
*                   - Bit #29     : Extended ID Flag (1u = Extended, 0u = Standard)
*                   - Bit #28 - 0 : Identifier (Standard, Extended, or Both)
*********************************************************************************************************
*/

typedef  struct  zc7xxx_can_frm {
    CPU_INT32U  Identifier;                                     /* CAN IDENTIFIER: Can Identifier                       */
    CPU_INT08U  Data[8u];                                       /*    CAN PAYLOAD: Bytes[Max 8] in Single CAN Msg       */
    CPU_INT08U  DLC;                                            /*        CAN DLC: Num of Valid Data(s) in Payload      */
    CPU_INT08U  Spare[3u];                                      /*          SPARE: Sets FRM w/ Integral Num of Pointers */
} ZC7xxx_CAN_FRM;


/*
*********************************************************************************************************
*                                         CAN REG FRM STRUCT
*
* Description : Structure defines a ZC7xxx Rx & Tx Message Format.
*
* Note(s)     : Since all 4 registers [ID, DLC, Data1, Data2] are required to be written to / read
*               from regardless of the actual number of data bytes & valid fields in the message, a
*               structure is used as a 'middle man' to read from / write to specific parts of the
*               Rx / Tx message prior to receiving / sending.
*********************************************************************************************************
*/

typedef  struct  zc7xxx_can_reg_frm {
    CPU_REG32  ID;                                              /* IDENTIFIER : CAN Rx / Tx ID     Register             */
    CPU_REG32  DLC;                                             /*        DLC : CAN Rx / Tx DLC    Register             */
    CPU_REG32  Data1;                                           /* DATA 1 REG : CAN Rx / Tx DATA 1 Register             */
    CPU_REG32  Data2;                                           /* DATA 2 REG : CAN Rx / Tx DATA 2 Register             */
} ZC7xxx_CAN_REG_FRM;


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

CPU_INT16S  ZC7xxx_CAN_Init    (CPU_INT32U   para_id);

CPU_INT16S  ZC7xxx_CAN_Open    (CPU_INT16S   dev_id,
                                CPU_INT32U   dev_name,
                                CPU_INT16U   mode);

CPU_INT16S  ZC7xxx_CAN_Close   (CPU_INT16S   para_id);

CPU_INT16S  ZC7xxx_CAN_IoCtl   (CPU_INT16S   para_id,
                                CPU_INT16U   func,
                                void        *p_arg);

CPU_INT16S  ZC7xxx_CAN_Read    (CPU_INT16S   para_id,
                                CPU_INT08U  *buf,
                                CPU_INT16U   size);

CPU_INT16S  ZC7xxx_CAN_Write   (CPU_INT16S   para_id,
                                CPU_INT08U  *buf,
                                CPU_INT16U   size);

void        ZC7xxx_CAN_ErrCheck(CPU_INT32U   para_id);


/*
*********************************************************************************************************
*                                            ERROR SECTION
*********************************************************************************************************
*/

#endif                                                          /* #ifndef _DRV_CAN_H_                                  */
