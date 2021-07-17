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
*                                      CAN DRIVER REGISTER CODE
*
*                                         ZYNQ-ZC7000 Series
*
* Filename : drv_can_reg.h
* Version  : V2.42.01
*********************************************************************************************************
*/

#ifndef  _DRV_CAN_REG_H
#define  _DRV_CAN_REG_H


/*
*********************************************************************************************************
*                                              INCLUDES
*********************************************************************************************************
*/

#include  "cpu.h"
#include  "can_cfg.h"


/*
*********************************************************************************************************
*                                        DEFAULT CONFIGURATION
*********************************************************************************************************
*/

#ifndef  CAN_MODULE_CHANNEL_0
#define  CAN_MODULE_CHANNEL_0                   DEF_DISABLED
#endif

#ifndef  CAN_MODULE_CHANNEL_1
#define  CAN_MODULE_CHANNEL_1                   DEF_DISABLED
#endif


/*
*********************************************************************************************************
*                                          ADDRESS REGISTERS
*********************************************************************************************************
*/
                                                                /* ------------------- BASE ADDRESS ------------------- */
#define  ZC7xxx_CAN0_ADDR                       0xE0008000u
#define  ZC7xxx_CAN1_ADDR                       0xE0009000u


/*
****************************************************************************************************
*                                           BIT DEFINITIONS
****************************************************************************************************
*/
                                                                /* ---------------- SOFTWARE RESET REG ---------------- */
#define  ZC7xxx_CAN_SRR_CEN                     DEF_BIT_01
#define  ZC7xxx_CAN_SRR_SRST                    DEF_BIT_00

                                                                /* ------------------ MODE SELECT REG ----------------- */
#define  ZC7xxx_CAN_MSR_SNOOP                   DEF_BIT_02
#define  ZC7xxx_CAN_MSR_LBACK                   DEF_BIT_01
#define  ZC7xxx_CAN_MSR_SLEEP                   DEF_BIT_00

                                                                /* ------------------ ERR STATUS REG ------------------ */
#define  ZC7xxx_CAN_ESR_ACKER                   DEF_BIT_04
#define  ZC7xxx_CAN_ESR_BERR                    DEF_BIT_03
#define  ZC7xxx_CAN_ESR_STER                    DEF_BIT_02
#define  ZC7xxx_CAN_ESR_FMER                    DEF_BIT_01
#define  ZC7xxx_CAN_ESR_CRCER                   DEF_BIT_00

                                                                /* ------------------ STATUS REGISTER ----------------- */
#define  ZC7xxx_CAN_SR_SNOOP                    DEF_BIT_12
#define  ZC7xxx_CAN_SR_ACFBSY                   DEF_BIT_11
#define  ZC7xxx_CAN_SR_TXFLL                    DEF_BIT_10
#define  ZC7xxx_CAN_SR_TXBFLL                   DEF_BIT_09

#define  ZC7xxx_CAN_SR_ESTAT_CFG_MODE          (0u << 7u)
#define  ZC7xxx_CAN_SR_ESTAT_ERR_ACTIVE        (1u << 7u)
#define  ZC7xxx_CAN_SR_ESTAT_ERR_PASS          (2u << 7u)
#define  ZC7xxx_CAN_SR_ESTAT_BUS_OFF           (3u << 7u)

#define  ZC7xxx_CAN_SR_ERRWRN                   DEF_BIT_06
#define  ZC7xxx_CAN_SR_BBSY                     DEF_BIT_05
#define  ZC7xxx_CAN_SR_BIDLE                    DEF_BIT_04
#define  ZC7xxx_CAN_SR_NORMAL                   DEF_BIT_03
#define  ZC7xxx_CAN_SR_SLEEP                    DEF_BIT_02
#define  ZC7xxx_CAN_SR_LBACK                    DEF_BIT_01
#define  ZC7xxx_CAN_SR_CONFIG                   DEF_BIT_00

                                                                /* ------------------ INT STATUS REG ------------------ */
#define  ZC7xxx_CAN_ISR_TXFEMP                  DEF_BIT_14
#define  ZC7xxx_CAN_ISR_TXFWMEMP                DEF_BIT_13
#define  ZC7xxx_CAN_ISR_RXFWMFLL                DEF_BIT_12
#define  ZC7xxx_CAN_ISR_WKUP                    DEF_BIT_11
#define  ZC7xxx_CAN_ISR_SLP                     DEF_BIT_10
#define  ZC7xxx_CAN_ISR_BSOFF                   DEF_BIT_09
#define  ZC7xxx_CAN_ISR_ERROR                   DEF_BIT_08
#define  ZC7xxx_CAN_ISR_RXNEMP                  DEF_BIT_07
#define  ZC7xxx_CAN_ISR_RXOFLW                  DEF_BIT_06
#define  ZC7xxx_CAN_ISR_RXUFLW                  DEF_BIT_05
#define  ZC7xxx_CAN_ISR_RXOK                    DEF_BIT_04
#define  ZC7xxx_CAN_ISR_TXBFLL                  DEF_BIT_03
#define  ZC7xxx_CAN_ISR_TXFLL                   DEF_BIT_02
#define  ZC7xxx_CAN_ISR_TXOK                    DEF_BIT_01
#define  ZC7xxx_CAN_ISR_ARBLST                  DEF_BIT_00

                                                                /* ------------------ INT ENABLE REG ------------------ */
#define  ZC7xxx_CAN_IER_ETXFEMP                 DEF_BIT_14
#define  ZC7xxx_CAN_IER_ETXFWMEMP               DEF_BIT_13
#define  ZC7xxx_CAN_IER_ERXFWMFLL               DEF_BIT_12
#define  ZC7xxx_CAN_IER_EWKUP                   DEF_BIT_11
#define  ZC7xxx_CAN_IER_ESLP                    DEF_BIT_10
#define  ZC7xxx_CAN_IER_EBSOFF                  DEF_BIT_09
#define  ZC7xxx_CAN_IER_EERROR                  DEF_BIT_08
#define  ZC7xxx_CAN_IER_ERXNEMP                 DEF_BIT_07
#define  ZC7xxx_CAN_IER_ERXOFLW                 DEF_BIT_06
#define  ZC7xxx_CAN_IER_ERXUFLW                 DEF_BIT_05
#define  ZC7xxx_CAN_IER_ERXOK                   DEF_BIT_04
#define  ZC7xxx_CAN_IER_ETXBFLL                 DEF_BIT_03
#define  ZC7xxx_CAN_IER_ETXFLL                  DEF_BIT_02
#define  ZC7xxx_CAN_IER_ETXOK                   DEF_BIT_01
#define  ZC7xxx_CAN_IER_EARBLST                 DEF_BIT_00

                                                                /* ------------------ INT CLEAR REG ------------------- */
#define  ZC7xxx_CAN_ICR_CTXFEMP                 DEF_BIT_14
#define  ZC7xxx_CAN_ICR_CTXFWMEMP               DEF_BIT_13
#define  ZC7xxx_CAN_ICR_CRXFWMFLL               DEF_BIT_12
#define  ZC7xxx_CAN_ICR_CWKUP                   DEF_BIT_11
#define  ZC7xxx_CAN_ICR_CSLP                    DEF_BIT_10
#define  ZC7xxx_CAN_ICR_CBSOFF                  DEF_BIT_09
#define  ZC7xxx_CAN_ICR_CERROR                  DEF_BIT_08
#define  ZC7xxx_CAN_ICR_CRXNEMP                 DEF_BIT_07
#define  ZC7xxx_CAN_ICR_CRXOFLW                 DEF_BIT_06
#define  ZC7xxx_CAN_ICR_CRXUFLW                 DEF_BIT_05
#define  ZC7xxx_CAN_ICR_CRXOK                   DEF_BIT_04
#define  ZC7xxx_CAN_ICR_CTXBFLL                 DEF_BIT_03
#define  ZC7xxx_CAN_ICR_CTXFLL                  DEF_BIT_02
#define  ZC7xxx_CAN_ICR_CTXOK                   DEF_BIT_01
#define  ZC7xxx_CAN_ICR_CARBLST                 DEF_BIT_00

                                                                /* --------------- TIMESTAMP CONTROL REG -------------- */
#define  ZC7xxx_CAN_TCR_CTS                     DEF_BIT_00

                                                                /* ---------------- CAN MSG ID DEFINES ---------------- */
                                                                /* #Defines Apply for Tx FIFO, Tx HPB & Rx FIFO Reg.    */
#define  ZC7xxx_CAN_ID_SRRRTR                   DEF_BIT_20
#define  ZC7xxx_CAN_ID_IDE                      DEF_BIT_19
#define  ZC7xxx_CAN_ID_RTR                      DEF_BIT_00

                                                                /* --------------- ACCEPTANCE FILTER REG -------------- */
#define  ZC7xxx_CAN_AFR_UAF4                    DEF_BIT_03
#define  ZC7xxx_CAN_AFR_UAF3                    DEF_BIT_02
#define  ZC7xxx_CAN_AFR_UAF2                    DEF_BIT_01
#define  ZC7xxx_CAN_AFR_UAF1                    DEF_BIT_00

                                                                /* ------------ ACCEPTANCE FILTER MSK REG x ----------- */
                                                                /* #Defines Apply for Filter Mask Registers 1, 2, 3, & 4*/
#define  ZC7xxx_CAN_AFMRx_AMSRR                 DEF_BIT_20
#define  ZC7xxx_CAN_AFMRx_AMIDE                 DEF_BIT_19
#define  ZC7xxx_CAN_AFMRx_AMRTR                 DEF_BIT_00

                                                                /* ------------ ACCEPTANCE FILTER ID REG x ------------ */
                                                                /* #Defines Apply for Filter ID Registers 1, 2, 3, & 4. */
#define  ZC7xxx_CAN_AFIRx_AISRR                 DEF_BIT_20
#define  ZC7xxx_CAN_AFIRx_AIIDE                 DEF_BIT_19
#define  ZC7xxx_CAN_AFIRx_AIRTR                 DEF_BIT_00


/*
*********************************************************************************************************
*                                               MACROS
*
* Note(s) : (1) When Using Extended IDs for Rx'd Messages, its required to save both the Standard
*               Section and Extended Section of the ID and placed both in the Rx Frame in the
*               following Format:
*                           Bits:  31     30    29                   18                   0
*                   uC/CAN Frame: [ 0u | RTR | IDE |    Standard ID    |    Extended ID    ]
*
*********************************************************************************************************
*/
                                                                /* ------------- BAUD RATE SETTING MACROs ------------- */
#define  ZC7xxx_CAN_BRPR_BRP(x)                 ((x) & 0xFFu)   /* Set Baud Rate Prescaler in the BRPR Register.        */

                                                                /* Set Bit Timing Register Settings on the BTR Register.*/
#define  ZC7xxx_CAN_BTR_SJW(x)                 (((x) & 0x03u) << 7u)
#define  ZC7xxx_CAN_BTR_TS2(x)                 (((x) & 0x07u) << 4u)
#define  ZC7xxx_CAN_BTR_TS1(x)                 (((x) & 0x0Fu) << 0u)

                                                                /* ------------- CAN MSG WATERMARK MACROs ------------- */
#define  ZC7xxx_CAN_WIR_EW(x)                  (((x) & 0xFFu) << 8u)
#define  ZC7xxx_CAN_WIR_FW(x)                  (((x) & 0xFFu) << 0u)

                                                                /* ----------------- CAN TX MSG MACROs ---------------- */
                                                                /*  - ID MACROs -                                       */
                                                                /* #Defines Apply for Tx FIFO & Tx HPB Registers.       */
                                                                /* Standard Message ID. Standard Frame is 11 Bits.      */
#define  ZC7xxx_CAN_Tx_ID_IDH(x)               (((x) & 0x07FFu) << 21u)

                                                                /* Extended Message ID. Valid only for Extended Frames. */
#define  ZC7xxx_CAN_Tx_ID_IDL(x)               (((x) & 0x03FFFFu) << 1u)

                                                                /*  - DLC MACROs -                                      */
                                                                /* #Define Applies for Tx FIFO & Tx HPB Registers.      */
#define  ZC7xxx_CAN_Tx_DLC_DLC(x)              (((x) & 0x0Fu) << 28u)

                                                                /*  - DATA 1 & 2 MACROs -                               */
                                                                /* #Defines Apply for Tx FIFO & Tx HPB Registers.       */
#define  ZC7xxx_CAN_Tx_DATA_BYTES(x, y)        (((x) & 0xFFu) << (24u - (8u * (y))))

                                                                /* ----------------- CAN RX MSG MACROs ---------------- */
                                                                /*  - ID MACROs -                                       */
                                                                /* #Defines Apply for Rx FIFO Register.                 */
                                                                /* Standard Message ID. Standard Frame is 11 Bits.      */
#define  ZC7xxx_CAN_Rx_ID_IDH(x)               (((x) >> 21u) & 0x07FFu)

                                                                /* Extended Message ID. Valid only for Extended Frames. */
#define  ZC7xxx_CAN_Rx_ID_IDL(x)               (((x) >>  1u) & 0x03FFFFu)

                                                                /* Gets BOTH Standard & Extended IDs. See Note (1).     */
#define  ZC7xxx_CAN_Rx_ID_BOTH(x)              ((ZC7xxx_CAN_Rx_ID_IDH((x)) << 18u) | ZC7xxx_CAN_Rx_ID_IDL((x)))

                                                                /*  - DLC MACROs -                                      */
                                                                /* Read DLC Quantity from Rx FIFO Register.             */
#define  ZC7xxx_CAN_Rx_DLC_DLC(x)              (((x) >> 28u) & 0x0Fu)

                                                                /* Read Rx Timestamp Value. Only for Rx'd Messages.     */
#define  ZC7xxx_CAN_Rx_DLC_RXT(x)              (((x) >>  0u) & 0xFFFFu)

                                                                /*  - DATA 1 & 2 MACROs -                               */
                                                                /* Reads Data Byte based on DLC Count Value.            */
#define  ZC7xxx_CAN_Rx_DATA_BYTES(x, y)        (((x) >> (24u - (8u * (y)))) & 0xFFu)

                                                                /* --------- ACCEPTANCE FILTER MSK REG x MACROs ------- */
                                                                /* #Defines Apply for Filter Mask Registers 1, 2, 3, & 4*/
#define  ZC7xxx_CAN_AFMRx_AMIDH(x)             (((x) &   0x07FFu) << 21u)
#define  ZC7xxx_CAN_AFMRx_AMIDL(x)             (((x) & 0x03FFFFu) <<  1u)

                                                                /* --------- ACCEPTANCE FILTER ID REG x MACROs -------- */
                                                                /* #Defines Apply for Filter ID Registers 1, 2, 3, & 4. */
#define  ZC7xxx_CAN_AFIRx_AIIDH(x)             (((x) &   0x07FFu) << 21u)
#define  ZC7xxx_CAN_AFIRx_AIIDL(x)             (((x) & 0x03FFFFu) <<  1u)


/*
*********************************************************************************************************
*                                             DATA ARRAY
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            CAN REGISTER
*
* Description : Structure defines the CAN Register Structure.
*
* Note(s)     : none.
*********************************************************************************************************
*/

typedef  volatile  struct  zc7xxx_can_reg {                     /* ---------- CAN CONTROLLER REGISTER SUMMARY --------- */
    CPU_REG32  SRR;                                             /* Software Reset       Register.                       */
    CPU_REG32  MSR;                                             /* Mode Select          Register.                       */
    CPU_REG32  BRPR;                                            /* Baud Rate Prescaler  Register.                       */
    CPU_REG32  BTR;                                             /* Bit Timing           Register.                       */
    CPU_REG32  ECR;                                             /* Error Counter        Register.                       */
    CPU_REG32  ESR;                                             /* Error Status         Register.                       */
    CPU_REG32  SR;                                              /* Status               Register.                       */
    CPU_REG32  ISR;                                             /* Interrupt Status     Register.                       */
    CPU_REG32  IER;                                             /* Interrupt Enable     Register.                       */
    CPU_REG32  ICR;                                             /* Interrupt Clear      Register.                       */
    CPU_REG32  TCR;                                             /* Timestamp Control    Register.                       */
    CPU_REG32  WIR;                                             /* Watermark Interrupt  Register.                       */
    CPU_REG32  TXFIFO_ID;                                       /* Transmit Message: FIFO Message Identifier.           */
    CPU_REG32  TXFIFO_DLC;                                      /* Transmit Message: FIFO Data Length Code.             */
    CPU_REG32  TXFIFO_DATA1;                                    /* Transmit Message: FIFO Data Word 1.                  */
    CPU_REG32  TXFIFO_DATA2;                                    /* Transmit Message: FIFO Data Word 2.                  */
    CPU_REG32  TXHPB_ID;                                        /* Transmit High Priority Buffer: Message Identifier.   */
    CPU_REG32  TXHPB_DLC;                                       /* Transmit High Priority Buffer: Data Length Code.     */
    CPU_REG32  TXHPB_DATA1;                                     /* Transmit High Priority Buffer: Data Word 1.          */
    CPU_REG32  TXHPB_DATA2;                                     /* Transmit High Priority Buffer: Data Word 2.          */
    CPU_REG32  RXFIFO_ID;                                       /* Receive Message: FIFO Message Identifier.            */
    CPU_REG32  RXFIFO_DLC;                                      /* Receive Message: FIFO Data Length Code.              */
    CPU_REG32  RXFIFO_DATA1;                                    /* Receive Message: FIFO Data Word 1.                   */
    CPU_REG32  RXFIFO_DATA2;                                    /* Receive Message: FIFO Data Word 2.                   */
    CPU_REG32  AFR;                                             /* Acceptance Filter    Register.                       */
    CPU_REG32  AFMR1;                                           /* Acceptance Filter    Mask Register: 1.               */
    CPU_REG32  AFIR1;                                           /* Acceptance Filter    ID   Register: 1.               */
    CPU_REG32  AFMR2;                                           /* Acceptance Filter    Mask Register: 2.               */
    CPU_REG32  AFIR2;                                           /* Acceptance Filter    ID   Register: 2.               */
    CPU_REG32  AFMR3;                                           /* Acceptance Filter    Mask Register: 3.               */
    CPU_REG32  AFIR3;                                           /* Acceptance Filter    ID   Register: 3.               */
    CPU_REG32  AFMR4;                                           /* Acceptance Filter    Mask Register: 4.               */
    CPU_REG32  AFIR4;                                           /* Acceptance Filter    ID   Register: 4.               */
} ZC7xxx_CAN_REG;


/*
*********************************************************************************************************
*                                        CAN BAUDRATE REGISTER
*
* Description : Structure defines the CAN BaudRate Register Structure.
*
* Note(s)     : none.
*********************************************************************************************************
*/

typedef  struct  zc7xxx_can_baud {    
    CPU_INT32U  BaudRate;                                       /* Holds the Baudrate.                                  */
    CPU_INT32U  SamplePoint;                                    /* Holds the Sample point in 0.1%                       */
    CPU_INT32U  ReSynchJumpWidth;                               /* Holds the Re-Synchronization Jump Width in 0.1%      */
    CPU_INT08U  PrescalerDiv;                                   /* Holds the Prescaler Divide Factor                    */
    CPU_INT08U  SJW;                                            /* Holds the Re-Synch Jump Width         (StdValue = 1) */
    CPU_INT08U  PropagationSeg;                                 /* Holds the Propagation Segment Time    (StdValue = 2) */
    CPU_INT08U  PhaseBufSeg1;                                   /* Holds the Phase Buffer Segment 1      (StdValue = 7) */
    CPU_INT08U  PhaseBufSeg2;                                   /* Holds the Phase Buffer Segment 2      (StdValue = 7) */
} ZC7xxx_CAN_BAUD;


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            ERROR SECTION
*********************************************************************************************************
*/

#endif
