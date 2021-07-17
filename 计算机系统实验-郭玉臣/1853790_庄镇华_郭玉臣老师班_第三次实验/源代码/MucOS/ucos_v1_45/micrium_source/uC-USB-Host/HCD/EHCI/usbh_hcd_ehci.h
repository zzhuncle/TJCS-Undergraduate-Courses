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
*                                         GENERIC EHCI DRIVER
*
* Filename : usbh_ehci.h
* Version  : V3.41.09
*********************************************************************************************************
*/

#ifndef  USBH_EHCI_H
#define  USBH_EHCI_H


/*
*********************************************************************************************************
*                                        INCLUDE HEADER FILES
*********************************************************************************************************
*/

#include  "../../Source/usbh_core.h"


/*
*********************************************************************************************************
*                                              CONSTANTS
*********************************************************************************************************
*/

#define  EHCI_MAX_ITD                                     10u   /* Max nbr of iTD per EP for HS isoc xfer               */
#define  EHCI_MAX_SITD                                    10u   /* Max nbr of siTD per EP for FS isoc xfer              */


#define  EHCI_MAX_PERIODIC_BW                             90u
#define  EHCI_PORT_POWERED_ALWAYS                          0u
#define  EHCI_PORT_POWERED_INDIVIDUAL                      1u
#define  EHCI_MAX_PERIODIC_LIST_SIZE                   (256u * 2u)


#define  EHCI_TIMESTAMP_MICROSEC                           1u
#define  EHCI_TIMESTAMP_MILLISEC                           2u

                                                                /* ----------- EHCI QH LIST NUMBER DEFINES ------------ */
#define  EHCI_QH_LIST_256MS                                0u
#define  EHCI_QH_LIST_128MS                              256u
#define  EHCI_QH_LIST_64MS                               384u
#define  EHCI_QH_LIST_32MS                               448u
#define  EHCI_QH_LIST_16MS                               480u
#define  EHCI_QH_LIST_08MS                               496u
#define  EHCI_QH_LIST_04MS                               504u
#define  EHCI_QH_LIST_02MS                               508u
#define  EHCI_QH_LIST_01MS                               510u
#define  EHCI_INTR_QH_LIST_SIZE         (EHCI_QH_LIST_01MS + 1u)

#define  EHCI_MAX_BW_PER_MICRO_FRAME                    3072u

#define  EHCI_BW_FLAG_CONSUME                              1u
#define  EHCI_BW_FLAG_PRODUCE                              2u


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                               MACROS
*********************************************************************************************************
*/


#ifndef  EHCI_CFG_ONRESET_EN                                    /* En callback to setup auxiliary registers on reset.   */
#define  EHCI_CFG_ONRESET_EN                  DEF_DISABLED
#endif

#define  USBH_EHCI_CFG_PERIODIC_EN            DEF_ENABLED


/*
*********************************************************************************************************
*                                        MAXIMUM PACKET SIZES
*********************************************************************************************************
*/

#define  MPS_HS                                         1024u   /* Maximum Packet Size for High Speed device            */
#define  MPS_FS                                         1023u   /* Maximum Packet Size for Full Speed device            */
#define  MPS_LS                                           64u   /* Maximum Packet Size for Low  Speed device            */


/*
*********************************************************************************************************
*                                          TYPE DEFINITIONS
*********************************************************************************************************
*/

typedef  struct  ehci_isoc_ep_desc  EHCI_ISOC_EP_DESC;
typedef  struct  ehci_isoc_ep_urb   EHCI_ISOC_EP_URB;
typedef  struct  ehci_intr_info     EHCI_INTR_INFO;


typedef  struct  ehci_qh {
    CPU_REG32    QHHorLinkPtr;
    CPU_REG32    QHEpCapChar[2];
    CPU_REG32    QHCurQTDPtr;
    CPU_REG32    QHNxtQTDPtr;
    CPU_REG32    QHAltNxtQTDPtr;
    CPU_REG32    QHToken;
    CPU_REG32    QHBufPagePtrList[5];
                                                                /* Fields not part of qH struct defined in EHCI spec    */
    USBH_EP     *EPPtr;
    CPU_INT32U   QTDHead;
    CPU_INT08U   SMask;
    CPU_INT08U   BWStartFrame;
    CPU_INT16U   FrameInterval;
    CPU_INT08U   Rsvd[4];                                       /* Padding to align the struct on a 32-byte boundary    */
} EHCI_QH;


typedef  struct  ehci_qtd {
    CPU_REG32  QTDNxtPtr;
    CPU_REG32  QTDAltNxtPtr;
    CPU_REG32  QTDToken;
    CPU_REG32  QTDBufPagePtrList[5];
} EHCI_QTD;


typedef  struct  ehci_sitd {
    CPU_REG32    SITDNxtLinkPtr;
    CPU_REG32    SITDEpCapChar[2];
    CPU_REG32    SITDStsCtrl;
    CPU_REG32    SITDBufPagePtrList[2];
    CPU_REG32    SITDBackLinkPtr;
} EHCI_SITD;


typedef  struct  ehci_itd {
    CPU_REG32  ITDNxtLinkPtr;
    CPU_REG32  ITDStsAndCntrl[8];
    CPU_REG32  ITDBufPagePtrList[7];
} EHCI_ITD;


struct  ehci_isoc_ep_desc {
    void               *TDTailPtr;
    USBH_EP            *EPPtr;
    CPU_INT08U          SMask;
    CPU_INT08U          CMask;
    CPU_INT08U          TCnt;
    CPU_INT08U          AppStartFrame;
    CPU_INT08U          NbrFrame;
    CPU_INT16U          FrameInterval;
    EHCI_ISOC_EP_DESC  *NxtEPDesc;
};

struct  ehci_isoc_ep_urb {
    CPU_INT32U          iTD_Addr;
    CPU_INT08U          AppStartFrame;
    CPU_INT08U          NbrFrame;
};


typedef  struct  ehci_cap {
    CPU_INT08U  CapLen;
    CPU_INT16U  HCIVersion;
    CPU_INT32U  HCSParams;
    CPU_INT32U  HCCParams;
    CPU_INT08U  HCSPPortRoute[15];
} EHCI_CAP;


typedef  struct  ehci_cap_reg {
    CPU_REG32  CapLen_HCIVersion;
    CPU_REG32  HCSParams;
    CPU_REG32  HCCParams;
    CPU_REG08  HCSPPortRoute[15];
} EHCI_CAP_REG;


typedef  struct  ehci_oper_reg {
    CPU_REG32  USBCmd;
    CPU_REG32  USBSts;
    CPU_REG32  USBIntr;
    CPU_REG32  FrameIx;
    CPU_REG32  CtrlDSSeg;
    CPU_REG32  PeriodicListBase;
    CPU_REG32  AsyncListAddr;
    CPU_REG32  Rsvd[9];
    CPU_REG32  CfgFlag;
    CPU_REG32  PortSC[1];
} EHCI_OPER_REG;


typedef  struct  ehci_dma {
    EHCI_QTD    *QTDPtr;                                        /* DMA memory CTRL,BULK and INTR QTD                    */
    EHCI_QH     *QHPtr;                                         /* DMA memory for Queue Head (QH)                       */
    EHCI_ITD    *ITDPtr;
    CPU_INT08U  *BufPtr;
} EHCI_DMA;


#if (USBH_EHCI_CFG_PERIODIC_EN == DEF_ENABLED)
struct  ehci_intr_info {
    CPU_INT08U       IntrPlaceholderIx;                         /* Index of Intr placeholder in QHLists array.          */
    CPU_INT16U       FrameInterval;
    USBH_EP         *EpPtr;
    EHCI_INTR_INFO  *NxtIntrInfo;
};
#endif


typedef  struct  ehci_dev {                                     /* -------------------- EHCI Device ------------------- */
    EHCI_CAP            HcCap;                                  /* Pointer to Capability structure                      */
    EHCI_DMA            DMA_EHCI;
    CPU_INT08U          EHCI_HubBuf[sizeof(USBH_HUB_DESC)];
    EHCI_QH            *AsyncQHHead;                            /* Asynchronous list head                               */
    CPU_INT08U          NbrPorts;                               /* Number of Ports in RootHub                           */

    MEM_POOL            HC_QHPool;                              /* Memory pool for allocating HC QHs                    */
    MEM_POOL            HC_QTDPool;                             /* Memory pool for allocating HC QTDs                   */
    MEM_POOL            BufPool;

    CPU_INT32U          PortResetChng;                          /* Port Reset Change status variable                    */
    EHCI_CAP_REG       *HcCapReg;                               /* Pointer to Host Controller Capability  Registers     */
    EHCI_OPER_REG      *HcOperReg;                              /* Pointer to Host Controller Operational Registers     */

#if (USBH_EHCI_CFG_PERIODIC_EN == DEF_ENABLED)
    CPU_INT32U         *PeriodicListBase;

    EHCI_QH            *QHLists[EHCI_INTR_QH_LIST_SIZE];        /* HCD qH placeholder array for Intr ep.                */

    MEM_POOL            HC_ITDPool;                             /* Memory pool for allocating HC iTDs                   */

    MEM_POOL            IntrInfoPool;                           /* Memory pool for allocating Intr info struct.         */
    MEM_POOL            HC_Isoc_EP_DescPool;                    /* Memory pool for allocating HCD Isoc EP struct        */
    MEM_POOL            HC_Isoc_EP_URBPool;

    CPU_INT16U          MaxPeriodicBWArr[256][8];               /* Maximum Periodic Bandwidth                           */
    EHCI_ISOC_EP_DESC  *HeadIsocEPDesc;                         /* Isochronous list head pointer                        */
    EHCI_INTR_INFO     *HeadIntrInfo;                           /* Intr info list head pointer.                         */
#endif

    CPU_INT32U          FNOCnt;                                 /* Counter for Frame List Rollover                      */
    CPU_BOOLEAN         HC_Started;                             /* Indicate if EHCI HC is started.                      */
    CPU_INT08U          DrvType;                                /* Indicate which EHCI drv type.                        */
} EHCI_DEV;


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/

#ifdef   USBH_EHCI_MODULE
#define  USBH_EHCI_EXT
#else
#define  USBH_EHCI_EXT  extern
#endif

USBH_EHCI_EXT  USBH_HC_DRV_API  EHCI_DrvAPI;
USBH_EHCI_EXT  USBH_HC_DRV_API  EHCI_DrvAPI_Synopsys;
USBH_EHCI_EXT  USBH_HC_RH_API   EHCI_RH_API;


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/


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
