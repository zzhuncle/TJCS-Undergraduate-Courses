/*
*********************************************************************************************************
*                                             uCOS XSDK BSP
*
*                      (c) Copyright 2014-2020; Silicon Laboratories Inc.,
*                             400 W. Cesar Chavez, Austin, TX 78701
*
*                All rights reserved. Protected by international copyright laws.
*
*               Your use of this software is subject to your acceptance of the terms
*               of a Silicon Labs Micrium software license, which can be obtained by
*               contacting info@micrium.com. If you do not agree to the terms of this
*               license, you may not use this software.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available. Your honesty is greatly appreciated.
*
*                 You can find our product's documentation at: doc.micrium.com
*
*                       For more information visit us at: www.micrium.com
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                      FILE SYSTEM DEVICE DRIVER
*
*                                             SD/MMC CARD
*                                              CARD MODE
*
*                                BOARD SUPPORT PACKAGE (BSP) FUNCTIONS
*
*                                          ZYNC-7000 - ZC702
*
* File : fs_dev_sd_card_bsp.c
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define    FS_DEV_SD_CARD_BSP_MODULE
#include  <cpu.h>
#include  <cpu_cache.h>

#include  <ucos_bsp.h>

#include  <Source/fs_type.h>
#include  <Source/fs_err.h>
#include  <Dev/SD/Card/fs_dev_sd_card.h>
#include  <OS/fs_os.h>

#include  <fs_cfg.h>

#include  <xparameters.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  SD_ROOT_CLK_FREQ  50000000

#define  SD_INTR_ID (56U)

/*
*********************************************************************************************************
*                                          REGISTER DEFINES
*********************************************************************************************************
*/

typedef  struct  sd_dev  {
    CPU_REG32  SDHC_DSADDR;                                     /* DMA system address.                                  */
    CPU_REG32  SDHC_BLKATTR;                                    /* Block attributes.                                    */
    CPU_REG32  SDHC_CMDARG;                                     /* Command argument.                                    */
    CPU_REG32  SDHC_XFERTYP;                                    /* Command transfer type.                               */
    CPU_REG32  SDHC_CMDRSP0;                                    /* Command response 0.                                  */
    CPU_REG32  SDHC_CMDRSP1;                                    /* Command response 1.                                  */
    CPU_REG32  SDHC_CMDRSP2;                                    /* Command response 2.                                  */
    CPU_REG32  SDHC_CMDRSP3;                                    /* Command response 3.                                  */
    CPU_REG32  SDHC_DATPORT;                                    /* Data buffer access port.                             */
    CPU_REG32  SDHC_PRSSTAT;                                    /* Present state.                                       */
    CPU_REG32  SDHC_PROCTL;                                     /* Protocol control.                                    */
    CPU_REG32  SDHC_SYSCTL;                                     /* System control.                                      */
    CPU_REG32  SDHC_IRQSTAT;                                    /* Interrupt status.                                    */
    CPU_REG32  SDHC_IRQSTATEN;                                  /* Interrupt status enable.                             */
    CPU_REG32  SDHC_IRQSIGEN;                                   /* Interrupt signal enable.                             */
    CPU_REG32  SDHC_AUTOC12ERR;                                 /* Auto CMD12 status.                                   */
    CPU_REG32  SDHC_HOSTCAPBLT;                                 /* Host capabilities.                                   */
    CPU_REG32  SDHC_WML;                                        /* Watermark level.                                     */
    CPU_REG32  RESERVED1[3];                                    /* Reserved.                                            */
    CPU_REG32  SDHC_FEVT;                                       /* Forced event.                                        */
    CPU_REG32  SDHC_ADMAES;                                     /* ADMA error status register.                          */
    CPU_REG32  SDHC_ADSADDR;                                    /* ADMA system address.                                 */
    CPU_REG32  RESERVED2[26];                                   /* Reserved.                                            */
    CPU_REG32  SDHC_VENDOR;                                     /* Vendor specific register.                            */
    CPU_REG32  SDHC_MMCBOOT;                                    /* MMC boot register.                                   */
    CPU_REG32  SDHC_HOSTVER;                                    /* Host controller version.                             */
} SD_DEV;


/*
*********************************************************************************************************
*                                        REGISTER BIT DEFINES
*********************************************************************************************************
*/

                                                                /* ------------------ SYSTEM CONTROL ------------------ */
#define  ZYNQ_BIT_SDHC_SYSCTL_INITA   (DEF_BIT_27)              /* Initialisation active.                               */
#define  ZYNQ_BIT_SDHC_SYSCTL_RSTD    (DEF_BIT_26)              /* DAT line software reset.                             */
#define  ZYNQ_BIT_SDHC_SYSCTL_RSTC    (DEF_BIT_25)              /* CMD line software reset.                             */
#define  ZYNQ_BIT_SDHC_SYSCTL_RSTA    (DEF_BIT_24)              /* Host controller software reset.                      */
#define  ZYNQ_BIT_SDHC_SYSCTL_DTOCV_MSK (DEF_BIT_FIELD(4, 16))  /* Data timeout counter value.                          */
#define  ZYNQ_BIT_SDHC_SYSCTL_DTOCV(cfg) (DEF_BIT_MASK(cfg, 16) & ZYNQ_BIT_SDHC_SYSCTL_DTOCV_MSK)
#define  ZYNQ_BIT_SDHC_SYSCTL_SDCLKFS_MSK (DEF_BIT_FIELD(8, 8)) /* SDCLK frequency select.                              */
#define  ZYNQ_BIT_SDHC_SYSCTL_SDCLKFS(cfg) (DEF_BIT_MASK(cfg, 8) & ZYNQ_BIT_SDHC_SYSCTL_SDCLKFS_MSK)
#define  ZYNQ_BIT_SDHC_SYSCTL_DVS_MSK (DEF_BIT_FIELD(4, 4))     /* SDCLK frequency divisor.                             */
#define  ZYNQ_BIT_SDHC_SYSCTL_DVS(cfg) (DEF_BIT_MASK(cfg, 4) & ZYNQ_BIT_SDHC_SYSCTL_DVS_MSK)
#define  ZYNQ_BIT_SDHC_SYSCTL_SDCLKEN (DEF_BIT_03)              /* SD clock enable.                                     */
#define  ZYNQ_BIT_SDHC_SYSCTL_PEREN   (DEF_BIT_02)              /* Peripheral clock enable.                             */
#define  ZYNQ_BIT_SDHC_SYSCTL_CLKSTB  (DEF_BIT_01)              /* Clock Stable.                                        */
#define  ZYNQ_BIT_SDHC_SYSCTL_IPGEN   (DEF_BIT_00)              /* IPGEN enable.                                        */


                                                                /* ----------------- BLOCK ATTRIBUTES ----------------- */
#define  ZYNQ_BIT_SDHC_BLKATTR_BLKCNT_MSK (DEF_BIT_FIELD(16, 16)) /* Block count.                                       */
#define  ZYNQ_BIT_SDHC_BLKATTR_BLKCNT(cnt) (DEF_BIT_MASK(cnt, 16) & ZYNQ_BIT_SDHC_BLKATTR_BLKCNT_MSK)

#define  ZYNQ_BIT_SDHC_BLKATTR_BUFSZ_MSK (DEF_BIT_FIELD(2, 12))   /* Buffer size.                                       */
#define  ZYNQ_BIT_SDHC_BLKATTR_BUFSZ(cnt) (DEF_BIT_MASK(cnt, 12) & ZYNQ_BIT_SDHC_BLKATTR_BUFSZ_MSK)

#define  ZYNQ_BIT_SDHC_BLKATTR_BLKSIZE_MSK (DEF_BIT_FIELD(12, 0)) /* Block size.                                        */
#define  ZYNQ_BIT_SDHC_BLKATTR_BLKSIZE(sz) (DEF_BIT_MASK(sz, 0) & ZYNQ_BIT_SDHC_BLKATTR_BLKSIZE_MSK)


                                                                /* -------------- COMMAND TRANSFER TYPE --------------- */
#define  ZYNQ_BIT_SDHC_XFERTYP_CMDINX_MSK (DEF_BIT_FIELD(6, 24))/* Command index.                                       */
#define  ZYNQ_BIT_SDHC_XFERTYP_CMDINX(inx) (DEF_BIT_MASK(inx, 24) & ZYNQ_BIT_SDHC_XFERTYP_CMDINX_MSK)
#define  ZYNQ_BIT_SDHC_XFERTYP_CMDTYP_MSK (DEF_BIT_FIELD(2, 22))/* Command type.                                        */
#define  ZYNQ_BIT_SDHC_XFERTYP_CMDTYP(typ) (DEF_BIT_MASK(typ, 22) & ZYNQ_BIT_SDHC_XFERTYP_CMDTYP_MSK)
#define  ZYNQ_BIT_SDHC_XFERTYP_DPSEL  (DEF_BIT_21)              /* Data present select.                                 */
#define  ZYNQ_BIT_SDHC_XFERTYP_CICEN  (DEF_BIT_20)              /* Command index check enable.                          */
#define  ZYNQ_BIT_SDHC_XFERTYP_CCCEN  (DEF_BIT_19)              /* Command CRC check enable.                            */
#define  ZYNQ_BIT_SDHC_XFERTYP_RSPTYP_MSK (DEF_BIT_FIELD(2, 16))/* Response type.                                       */
#define  ZYNQ_BIT_SDHC_XFERTYP_RSPTYP(typ) (DEF_BIT_MASK(typ, 16) & ZYNQ_BIT_SDHC_XFERTYP_RSPTYP_MSK)
#define  ZYNQ_BIT_SDHC_XFERTYP_MSBSEL (DEF_BIT_05)              /* Multi / Single block select.                         */
#define  ZYNQ_BIT_SDHC_XFERTYP_DTDSEL (DEF_BIT_04)              /* Data transfer direction select.                      */
#define  ZYNQ_BIT_SDHC_XFERTYP_AC12EN (DEF_BIT_02)              /* Auto CMD12 enable.                                   */
#define  ZYNQ_BIT_SDHC_XFERTYP_BCEN   (DEF_BIT_01)              /* Block count enable.                                  */
#define  ZYNQ_BIT_SDHC_XFERTYP_DMAEN  (DEF_BIT_00)              /* DMA enable.                                          */

#define  ZYNQ_BIT_SDHC_CMDTYP_ABORT    (3u)
#define  ZYNQ_BIT_SDHC_CMDTYP_RESUME   (2u)
#define  ZYNQ_BIT_SDHC_CMDTYP_SUSPEND  (1u)
#define  ZYNQ_BIT_SDHC_CMDTYP_NORMAL   (0u)

#define  ZYNQ_BIT_SDHC_RSPTYP_48B     (3u)
#define  ZYNQ_BIT_SDHC_RSPTYP_48      (2u)
#define  ZYNQ_BIT_SDHC_RSPTYP_136     (1u)
#define  ZYNQ_BIT_SDHC_RSPTYP_NONE    (0u)


                                                                /* ------------------ PRESENT STATE ------------------- */
#define  ZYNQ_BIT_SDHC_PRSSTAT_DLSL_MSK (DEF_BIT_FIELD(8, 24))  /* DAT line signal level.                               */
#define  ZYNQ_BIT_SDHC_PRSSTAT_DLSL(dat) (DEF_BIT_MASK(dat, 24) & ZYNQ_BIT_SDHC_PRSSTAT_DLSL_MSK)
#define  ZYNQ_BIT_SDHC_PRSSTAT_CLSL   (DEF_BIT_23)              /* CMD signal level.                                    */
#define  ZYNQ_BIT_SDHC_PRSSTAT_WPSPL  (DEF_BIT_19)              /* Write protect switch pin level.                      */
#define  ZYNQ_BIT_SDHC_PRSSTAT_CDPL   (DEF_BIT_18)              /* Card detect pin level.                               */
#define  ZYNQ_BIT_SDHC_PRSSTAT_CINS   (DEF_BIT_16)              /* Card inserted.                                       */
#define  ZYNQ_BIT_SDHC_PRSSTAT_BREN   (DEF_BIT_11)              /* Buffer read enable.                                  */
#define  ZYNQ_BIT_SDHC_PRSSTAT_BWEN   (DEF_BIT_10)              /* Buffer write enable.                                 */
#define  ZYNQ_BIT_SDHC_PRSSTAT_RTA    (DEF_BIT_09)              /* Read transfer active.                                */
#define  ZYNQ_BIT_SDHC_PRSSTAT_WTA    (DEF_BIT_08)              /* Write transfer active.                               */
#define  ZYNQ_BIT_SDHC_PRSSTAT_SDOFF  (DEF_BIT_07)              /* SD clock gated off internally.                       */
#define  ZYNQ_BIT_SDHC_PRSSTAT_PEROFF (DEF_BIT_06)              /* Peripheral clock gated internally.                   */
#define  ZYNQ_BIT_SDHC_PRSSTAT_HCLK   (DEF_BIT_05)              /* HCLK gated internally.                               */
#define  ZYNQ_BIT_SDHC_PRSSTAT_IPGOFF (DEF_BIT_04)              /* IPG clock gated internally.                          */
#define  ZYNQ_BIT_SDHC_PRSSTAT_SDSTB  (DEF_BIT_03)              /* SD clock enable.                                     */
#define  ZYNQ_BIT_SDHC_PRSSTAT_DLA    (DEF_BIT_02)              /* Data line active.                                    */
#define  ZYNQ_BIT_SDHC_PRSSTAT_CDIHB  (DEF_BIT_01)              /* Command inhibit(DAT).                                */
#define  ZYNQ_BIT_SDHC_PRSSTAT_CIHB   (DEF_BIT_00)              /* Command inhibit(CMD).                                */


                                                                /* -------------------- IRQ STATUS -------------------- */
#define  ZYNQ_BIT_SDHC_IRQSTAT_DMAE   (DEF_BIT_28)              /* DMA error.                                           */
#define  ZYNQ_BIT_SDHC_IRQSTAT_AC12E  (DEF_BIT_24)              /* Auto CMD12 error.                                    */
#define  ZYNQ_BIT_SDHC_IRQSTAT_DEBE   (DEF_BIT_22)              /* Data end bit error.                                  */
#define  ZYNQ_BIT_SDHC_IRQSTAT_DCE    (DEF_BIT_21)              /* Data CRC error.                                      */
#define  ZYNQ_BIT_SDHC_IRQSTAT_DTOE   (DEF_BIT_20)              /* Data timeout error.                                  */
#define  ZYNQ_BIT_SDHC_IRQSTAT_CIE    (DEF_BIT_19)              /* Command index error.                                 */
#define  ZYNQ_BIT_SDHC_IRQSTAT_CEBE   (DEF_BIT_18)              /* Command end bit error.                               */
#define  ZYNQ_BIT_SDHC_IRQSTAT_CCE    (DEF_BIT_17)              /* Command CRC error.                                   */
#define  ZYNQ_BIT_SDHC_IRQSTAT_CTOE   (DEF_BIT_16)              /* Command timeout error.                               */
#define  ZYNQ_BIT_SDHC_IRQSTAT_EI     (DEF_BIT_15)              /* Error interrupt.                                     */
#define  ZYNQ_BIT_SDHC_IRQSTAT_CINT   (DEF_BIT_08)              /* Card interrupt.                                      */
#define  ZYNQ_BIT_SDHC_IRQSTAT_CRM    (DEF_BIT_07)              /* Card removal.                                        */
#define  ZYNQ_BIT_SDHC_IRQSTAT_CINS   (DEF_BIT_06)              /* Card insertion.                                      */
#define  ZYNQ_BIT_SDHC_IRQSTAT_BRR    (DEF_BIT_05)              /* Buffer read ready.                                   */
#define  ZYNQ_BIT_SDHC_IRQSTAT_BWR    (DEF_BIT_04)              /* Buffer write ready.                                  */
#define  ZYNQ_BIT_SDHC_IRQSTAT_DINT   (DEF_BIT_03)              /* DMA interrupt.                                       */
#define  ZYNQ_BIT_SDHC_IRQSTAT_BGE    (DEF_BIT_02)              /* Block gap event.                                     */
#define  ZYNQ_BIT_SDHC_IRQSTAT_TC     (DEF_BIT_01)              /* Transfer complete.                                   */
#define  ZYNQ_BIT_SDHC_IRQSTAT_CC     (DEF_BIT_00)              /* Command complete.                                    */


                                                                /* -------------------- IRQ STATUS -------------------- */
#define  ZYNQ_BIT_SDHC_PROCTL_WECRM   (DEF_BIT_26)              /* Wakeup event on sd card removal.                     */
#define  ZYNQ_BIT_SDHC_PROCTL_WECINS  (DEF_BIT_25)              /* Wakeup event on sd card insertion.                   */
#define  ZYNQ_BIT_SDHC_PROCTL_WECINT  (DEF_BIT_24)              /* Wakeup event on sd card interrupt.                   */
#define  ZYNQ_BIT_SDHC_PROCTL_IABG    (DEF_BIT_19)              /* Interrupt at block gap.                              */
#define  ZYNQ_BIT_SDHC_PROCTL_RWCTL   (DEF_BIT_18)              /* Read wait control.                                   */
#define  ZYNQ_BIT_SDHC_PROCTL_CREQ    (DEF_BIT_17)              /* Continue request.                                    */
#define  ZYNQ_BIT_SDHC_PROCTL_SABGREQ (DEF_BIT_16)              /* Stop at block gap request.                           */
#define  ZYNQ_BIT_SDHC_PROCTL_DMAS_MSK (DEF_BIT_FIELD(2, 8))    /* DMA select.                                          */
#define  ZYNQ_BIT_SDHC_PROCTL_DMAS(typ) (DEF_BIT_MASK(typ, 8) & ZYNQ_BIT_SDHC_PROCTL_DMAS_MSK)
#define  ZYNQ_BIT_SDHC_PROCTL_CDRSS   (DEF_BIT_07)              /* Card detect signal detection.                        */
#define  ZYNQ_BIT_SDHC_PROCTL_CDTL    (DEF_BIT_06)              /* Card detect test level.                              */
#define  ZYNQ_BIT_SDHC_PROCTL_EMODE_MSK (DEF_BIT_FIELD(2, 4))   /* DMA select.                                          */
#define  ZYNQ_BIT_SDHC_PROCTL_EMODE(typ) (DEF_BIT_MASK(typ, 4) & ZYNQ_BIT_SDHC_PROCTL_EMODE_MSK)
#define  ZYNQ_BIT_SDHC_PROCTL_D3CD    (DEF_BIT_03)              /* DAT3 as card selection pin.                          */
#define  ZYNQ_BIT_SDHC_PROCTL_HSE     (DEF_BIT_02)              /* High speed enable.                                   */
#define  ZYNQ_BIT_SDHC_PROCTL_DTW     (DEF_BIT_01)              /* Transfer width.                                      */
#define  ZYNQ_BIT_SDHC_PROCTL_LED     (DEF_BIT_00)              /* LED control.                                         */


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

SD_DEV  *esdhc = (SD_DEV *)0xE0100000u;

FS_OS_SEM  FSDev_SD_Sem;


/*
*********************************************************************************************************
*                                            LOCAL MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void  FSDev_SD_Card_BSP_ISR_Handler (void  *p_arg);


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                                    FILE SYSTEM SD CARD FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                      FSDev_SD_Card_BSP_Open()
*
* Description : Open (initialize) SD/MMC card interface.
*
* Argument(s) : unit_nbr    Unit number of SD/MMC card.
*
* Return(s)   : DEF_OK,   if interface was opened.
*               DEF_FAIL, otherwise.
*
* Caller(s)   : FSDev_SD_Card_Refresh().
*
* Note(s)     : (1) This function will be called EVERY time the device is opened.
*********************************************************************************************************
*/

CPU_BOOLEAN  FSDev_SD_Card_BSP_Open (FS_QTY  unit_nbr)
{
#if 0
                                                                /* Pin config.                                          */
    ZYNQ_SLCR_REG->SLCR_UNLOCK = ZYNQ_SLCR_UNLOCK_KEY;
    CPU_MB();
    ZYNQ_SLCR_REG->MIO_PIN_40 = ZYNQ_BIT_MIO_PULL_UP     |
                                ZYNQ_BIT_MIO_IO_TYPE(1u) |
                                ZYNQ_BIT_MIO_L3_SEL(4u);

    ZYNQ_SLCR_REG->MIO_PIN_41 = ZYNQ_BIT_MIO_PULL_UP     |
                                ZYNQ_BIT_MIO_IO_TYPE(1u) |
                                ZYNQ_BIT_MIO_L3_SEL(4u);

    ZYNQ_SLCR_REG->MIO_PIN_42 = ZYNQ_BIT_MIO_PULL_UP     |
                                ZYNQ_BIT_MIO_IO_TYPE(1u) |
                                ZYNQ_BIT_MIO_L3_SEL(4u);

    ZYNQ_SLCR_REG->MIO_PIN_43 = ZYNQ_BIT_MIO_PULL_UP     |
                                ZYNQ_BIT_MIO_IO_TYPE(1u) |
                                ZYNQ_BIT_MIO_L3_SEL(4u);

    ZYNQ_SLCR_REG->MIO_PIN_44 = ZYNQ_BIT_MIO_PULL_UP     |
                                ZYNQ_BIT_MIO_IO_TYPE(1u) |
                                ZYNQ_BIT_MIO_L3_SEL(4u);

    ZYNQ_SLCR_REG->MIO_PIN_45 = ZYNQ_BIT_MIO_PULL_UP     |
                                ZYNQ_BIT_MIO_IO_TYPE(1u) |
                                ZYNQ_BIT_MIO_L3_SEL(4u);
    CPU_MB();
    ZYNQ_SLCR_REG->SLCR_LOCK  = ZYNQ_SLCR_LOCK_KEY;
#endif

    FS_OS_SemCreate(&FSDev_SD_Sem, 0u);


    esdhc->SDHC_SYSCTL = ZYNQ_BIT_SDHC_SYSCTL_RSTA;            /* Ctrl soft reset.                                     */

    CPU_MB();

    FSDev_SD_Card_BSP_SetClkFreq(unit_nbr, 400000u);

    esdhc->SDHC_IRQSTATEN = ZYNQ_BIT_SDHC_IRQSTAT_CC   |
                            ZYNQ_BIT_SDHC_IRQSTAT_TC   |
                            ZYNQ_BIT_SDHC_IRQSTAT_EI   |
                            ZYNQ_BIT_SDHC_IRQSTAT_CTOE |
                            ZYNQ_BIT_SDHC_IRQSTAT_CCE  |
                            ZYNQ_BIT_SDHC_IRQSTAT_CEBE |
                            ZYNQ_BIT_SDHC_IRQSTAT_CIE  |
                            ZYNQ_BIT_SDHC_IRQSTAT_DTOE |
                            ZYNQ_BIT_SDHC_IRQSTAT_DCE  |
                            ZYNQ_BIT_SDHC_IRQSTAT_DEBE |
                            ZYNQ_BIT_SDHC_IRQSTAT_DINT |
                            ZYNQ_BIT_SDHC_IRQSTAT_DMAE;

    esdhc->SDHC_SYSCTL |= ZYNQ_BIT_SDHC_SYSCTL_DTOCV(8);

    esdhc->SDHC_PROCTL |= DEF_BIT_06 | DEF_BIT_07;

    esdhc->SDHC_SYSCTL |=   ZYNQ_BIT_SDHC_SYSCTL_PEREN        |
                            ZYNQ_BIT_SDHC_SYSCTL_IPGEN;

    esdhc->SDHC_PROCTL |= DEF_BIT_08;


    UCOS_IntVectSet(SD_INTR_ID,
                    0u,
                    0u,
                    FSDev_SD_Card_BSP_ISR_Handler,
                    DEF_NULL);

    UCOS_IntSrcEn(SD_INTR_ID);


    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                      FSDev_SD_Card_BSP_Close()
*
* Description : Close (unitialize) SD/MMC card interface.
*
* Argument(s) : unit_nbr    Unit number of SD/MMC card.
*
* Return(s)   : none.
*
* Caller(s)   : FSDev_SD_Card_Close().
*
* Note(s)     : (1) This function will be called EVERY time the device is closed.
*********************************************************************************************************
*/

void  FSDev_SD_Card_BSP_Close (FS_QTY  unit_nbr)
{
    FS_OS_SemDel(&FSDev_SD_Sem);
}


/*
*********************************************************************************************************
*                                      FSDev_SD_Card_BSP_Lock()
*
* Description : Acquire SD/MMC card bus lock.
*
* Argument(s) : unit_nbr    Unit number of SD/MMC card.
*
* Return(s)   : none.
*
* Caller(s)   : various.
*
* Note(s)     : (1) This function will be called before the SD/MMC driver begins to access the SD/MMC
*                   card bus.  The application should NOT use the same bus to access another device until
*                   the matching call to 'FSDev_SD_Card_BSP_Unlock()' has been made.
*********************************************************************************************************
*/

void  FSDev_SD_Card_BSP_Lock (FS_QTY  unit_nbr)
{

}


/*
*********************************************************************************************************
*                                     FSDev_SD_Card_BSP_Unlock()
*
* Description : Release SD/MMC card bus lock.
*
* Argument(s) : unit_nbr    Unit number of SD/MMC card.
*
* Return(s)   : none.
*
* Caller(s)   : various.
*
* Note(s)     : (1) 'FSDev_SD_Card_BSP_Lock()' will be called before the SD/MMC driver begins to access
*                   the SD/MMC card bus.  The application should NOT use the same bus to access another
*                   device until the matching call to this function has been made.
*********************************************************************************************************
*/

void  FSDev_SD_Card_BSP_Unlock (FS_QTY  unit_nbr)
{

}


/*
*********************************************************************************************************
*                                      FSDev_SD_Card_BSP_CmdStart()
*
* Description : Start a command.
*
* Argument(s) : unit_nbr    Unit number of SD/MMC card.
*
*               p_cmd       Pointer to command to transmit (see Note #2).
*
*               p_data      Pointer to buffer address for DMA transfer (see Note #3).
*
*               p_err       Pointer to variable that will receive the return error code from this function :
*
*                               FS_DEV_SD_CARD_ERR_NONE       No error.
*                               FS_DEV_SD_CARD_ERR_NO_CARD    No card present.
*                               FS_DEV_SD_CARD_ERR_BUSY       Controller is busy.
*                               FS_DEV_SD_CARD_ERR_UNKNOWN    Unknown or other error.
*
* Return(s)   : none.
*
* Caller(s)   : various.
*
* Note(s)     : (1) The command start will be followed by zero, one or two additional BSP function calls,
*                   depending on whether data should be transferred and on whether any errors occur.
*
*                   (a) 'FSDev_SD_Card_BSP_CmdStart()' starts execution of the command.  It may also set
*                       up the DMA transfer (if necessary).
*
*                   (b) 'FSDev_SD_Card_BSP_CmdWaitEnd()' waits for the execution of the command to end,
*                       getting the command response (if any).
*
*                   (c) If data should be transferred from the card to the host, 'FSDev_SD_Card_BSP_CmdDataRd()'
*                       will read that data; if data should be transferred from the host to the card,
*                       'FSDev_SD_Card_BSP_CmdDataWr()' will write that data.
*
*                   (d) If an error is returned at any point, the sequence will be aborted.
*
*               (2) The command 'p_cmd' has the following parameters :
*
*                   (a) 'p_cmd->Cmd' is the command index.
*
*                   (b) 'p_cmd->Arg' is the 32-bit argument (or 0 if there is no argument).
*
*                   (c) 'p_cmd->Flags' is a bit-mapped variable with zero or more command flags :
*
*                           FS_DEV_SD_CARD_CMD_FLAG_INIT          Initialization sequence before command.
*                           FS_DEV_SD_CARD_CMD_FLAG_BUSY          Busy signal expected after command.
*                           FS_DEV_SD_CARD_CMD_FLAG_CRC_VALID     CRC valid after command.
*                           FS_DEV_SD_CARD_CMD_FLAG_IX_VALID      Index valid after command.
*                           FS_DEV_SD_CARD_CMD_FLAG_OPEN_DRAIN    Command line is open drain.
*                           FS_DEV_SD_CARD_CMD_FLAG_DATA_START    Data start command.
*                           FS_DEV_SD_CARD_CMD_FLAG_DATA_STOP     Data stop command.
*                           FS_DEV_SD_CARD_CMD_FLAG_RESP          Response expected.
*                           FS_DEV_SD_CARD_CMD_FLAG_RESP_LONG     Long response expected.
*
*                   (d) 'p_cmd->DataDir' indicates the direction of any data transfer that should follow
*                       this command, if any :
*
*                           FS_DEV_SD_CARD_DATA_DIR_NONE            No data transfer.
*                           FS_DEV_SD_CARD_DATA_DIR_HOST_TO_CARD    Transfer host-to-card (write).
*                           FS_DEV_SD_CARD_DATA_DIR_CARD_TO_HOST    Transfer card-to-host (read).
*
*                   (e) 'p_cmd->DataType' indicates the type of the data transfer that should follow this
*                       command, if any :
*
*                           FS_DEV_SD_CARD_DATA_TYPE_NONE            No data transfer.
*                           FS_DEV_SD_CARD_DATA_TYPE_SINGLE_BLOCK    Single data block.
*                           FS_DEV_SD_CARD_DATA_TYPE_MULTI_BLOCK     Multiple data blocks.
*                           FS_DEV_SD_CARD_DATA_TYPE_STREAM          Stream data.
*
*                   (f) 'p_cmd->RespType' indicates the type of the response that should be expected from
*                       this command :
*
*                           FS_DEV_SD_CARD_RESP_TYPE_NONE    No  response.
*                           FS_DEV_SD_CARD_RESP_TYPE_R1      R1  response: Normal Response Command.
*                           FS_DEV_SD_CARD_RESP_TYPE_R1B     R1b response.
*                           FS_DEV_SD_CARD_RESP_TYPE_R2      R2  response: CID, CSD Register.
*                           FS_DEV_SD_CARD_RESP_TYPE_R3      R3  response: OCR Register.
*                           FS_DEV_SD_CARD_RESP_TYPE_R4      R4  response: Fast I/O Response (MMC).
*                           FS_DEV_SD_CARD_RESP_TYPE_R5      R5  response: Interrupt Request Response (MMC).
*                           FS_DEV_SD_CARD_RESP_TYPE_R5B     R5B response.
*                           FS_DEV_SD_CARD_RESP_TYPE_R6      R6  response: Published RCA Response.
*                           FS_DEV_SD_CARD_RESP_TYPE_R7      R7  response: Card Interface Condition.
*
*                   (g) 'p_cmd->BlkSize' and 'p_cmd->BlkCnt' are the block size and block count of the
*                       data transfer that should follow this command, if any.
*
*               (3) The pointer to the data buffer that will receive the data transfer that should follow
*                   this command is given so that a DMA transfer can be set up.
*********************************************************************************************************
*/

void  FSDev_SD_Card_BSP_CmdStart (FS_QTY               unit_nbr,
                                  FS_DEV_SD_CARD_CMD  *p_cmd,
                                  void                *p_data,
                                  FS_DEV_SD_CARD_ERR  *p_err)
{
    CPU_INT32U   xfertyp = 0u;

    esdhc->SDHC_IRQSTAT = 0xFFFFFFFF;                            /* Clear all interrupts.                                */

    esdhc->SDHC_IRQSIGEN = ZYNQ_BIT_SDHC_IRQSTAT_CIE  |
                           ZYNQ_BIT_SDHC_IRQSTAT_CEBE |
                           ZYNQ_BIT_SDHC_IRQSTAT_CCE  |
                           ZYNQ_BIT_SDHC_IRQSTAT_CTOE |
                           ZYNQ_BIT_SDHC_IRQSTAT_CC;

    while(DEF_BIT_IS_SET(esdhc->SDHC_PRSSTAT, ZYNQ_BIT_SDHC_PRSSTAT_CIHB) == DEF_YES ||
          DEF_BIT_IS_SET(esdhc->SDHC_PRSSTAT, ZYNQ_BIT_SDHC_PRSSTAT_CDIHB) == DEF_YES)
    {
        ;
    }

    while(DEF_BIT_IS_SET(esdhc->SDHC_PRSSTAT, ZYNQ_BIT_SDHC_PRSSTAT_DLA) == DEF_YES)
    {
        ;
    }

    xfertyp |= ZYNQ_BIT_SDHC_XFERTYP_CMDINX(p_cmd->Cmd);

    if (DEF_BIT_IS_SET(p_cmd->Flags, FS_DEV_SD_CARD_CMD_FLAG_CRC_VALID) == DEF_YES) {
        xfertyp |= ZYNQ_BIT_SDHC_XFERTYP_CCCEN;                 /* Check CRC.                                           */
    }

    if (DEF_BIT_IS_SET(p_cmd->Flags, FS_DEV_SD_CARD_CMD_FLAG_IX_VALID) == DEF_YES) {
        xfertyp |= ZYNQ_BIT_SDHC_XFERTYP_CICEN;                 /* Check index.                                         */
    }

    if (DEF_BIT_IS_SET(p_cmd->Flags, FS_DEV_SD_CARD_CMD_FLAG_RESP_LONG) == DEF_YES) {
        xfertyp |= ZYNQ_BIT_SDHC_XFERTYP_RSPTYP(ZYNQ_BIT_SDHC_RSPTYP_136);
    } else if (DEF_BIT_IS_SET(p_cmd->Flags, FS_DEV_SD_CARD_CMD_FLAG_RESP) == DEF_YES) {
        if (DEF_BIT_IS_SET(p_cmd->Flags, FS_DEV_SD_CARD_CMD_FLAG_BUSY) == DEF_YES) {
            xfertyp |= ZYNQ_BIT_SDHC_XFERTYP_RSPTYP(ZYNQ_BIT_SDHC_RSPTYP_48B);
        } else {
            xfertyp |= ZYNQ_BIT_SDHC_XFERTYP_RSPTYP(ZYNQ_BIT_SDHC_RSPTYP_48);
        }
    }

    esdhc->SDHC_CMDARG = p_cmd->Arg;

    if(DEF_BIT_IS_SET(p_cmd->Flags, FS_DEV_SD_CARD_CMD_FLAG_STOP_DATA_TX) == DEF_YES) {
        xfertyp |= ZYNQ_BIT_SDHC_XFERTYP_CMDTYP(ZYNQ_BIT_SDHC_CMDTYP_ABORT);
        xfertyp |= ZYNQ_BIT_SDHC_XFERTYP_MSBSEL |
                   ZYNQ_BIT_SDHC_XFERTYP_BCEN;
    }

    switch (p_cmd->DataDir) {
         case FS_DEV_SD_CARD_DATA_DIR_CARD_TO_HOST:

             esdhc->SDHC_DSADDR = (CPU_INT32U)p_data;

             xfertyp |= ZYNQ_BIT_SDHC_XFERTYP_DMAEN;
             xfertyp |= ZYNQ_BIT_SDHC_XFERTYP_DTDSEL;           /* Card to host.                                        */
             xfertyp |= ZYNQ_BIT_SDHC_XFERTYP_DPSEL;

             if(p_cmd->BlkCnt > 1u) {                           /* Multi block transfer.                                */
                 xfertyp |= ZYNQ_BIT_SDHC_XFERTYP_MSBSEL |
                            ZYNQ_BIT_SDHC_XFERTYP_BCEN;
             }

             esdhc->SDHC_BLKATTR  = ZYNQ_BIT_SDHC_BLKATTR_BLKCNT(p_cmd->BlkCnt) |
                                    ZYNQ_BIT_SDHC_BLKATTR_BUFSZ(7u)             |
                                    ZYNQ_BIT_SDHC_BLKATTR_BLKSIZE(p_cmd->BlkSize);

             CPU_DCACHE_RANGE_FLUSH(p_data, p_cmd->BlkCnt * p_cmd->BlkSize);

             break;

         case FS_DEV_SD_CARD_DATA_DIR_HOST_TO_CARD:

             esdhc->SDHC_DSADDR = (CPU_INT32U)p_data;

             xfertyp |= ZYNQ_BIT_SDHC_XFERTYP_DMAEN;
             xfertyp |= ZYNQ_BIT_SDHC_XFERTYP_DPSEL;

             if(p_cmd->BlkCnt > 1u) {                           /* Multi block transfer.                                */
                 xfertyp |= ZYNQ_BIT_SDHC_XFERTYP_MSBSEL |
                            ZYNQ_BIT_SDHC_XFERTYP_BCEN;
             }

             esdhc->SDHC_BLKATTR  = ZYNQ_BIT_SDHC_BLKATTR_BLKCNT(p_cmd->BlkCnt) |
                                    ZYNQ_BIT_SDHC_BLKATTR_BUFSZ(7u)             |
                                    ZYNQ_BIT_SDHC_BLKATTR_BLKSIZE(p_cmd->BlkSize);

             CPU_DCACHE_RANGE_FLUSH(p_data, p_cmd->BlkCnt * p_cmd->BlkSize);

             break;

        case FS_DEV_SD_CARD_DATA_DIR_NONE:

             esdhc->SDHC_DSADDR  = 0u;
             esdhc->SDHC_BLKATTR = 0u;

             break;

        default:

            break;
    }

#if 1
    esdhc->SDHC_SYSCTL |=   ZYNQ_BIT_SDHC_SYSCTL_PEREN        |
                            ZYNQ_BIT_SDHC_SYSCTL_IPGEN;

    esdhc->SDHC_PROCTL |= DEF_BIT_08;
#endif
    esdhc->SDHC_XFERTYP = xfertyp;


    esdhc->SDHC_PROCTL |= DEF_BIT_06 | DEF_BIT_07;

    *p_err = FS_DEV_SD_CARD_ERR_NONE;

    return;
}


/*
*********************************************************************************************************
*                                   FSDev_SD_Card_BSP_CmdWaitEnd()
*
* Description : Wait for command to end & get command response.
*
* Argument(s) : unit_nbr    Unit number of SD/MMC card.
*
*               p_cmd       Pointer to command that is ending.
*
*               p_resp      Pointer to buffer that will receive command response, if any.
*
*               p_err       Pointer to variable that will receive the return error code from this function :
*
*                               FS_DEV_SD_CARD_ERR_NONE            No error.
*                               FS_DEV_SD_CARD_ERR_NO_CARD         No card present.
*                               FS_DEV_SD_CARD_ERR_UNKNOWN         Unknown or other error.
*                               FS_DEV_SD_CARD_ERR_WAIT_TIMEOUT    Timeout in waiting for command response.
*                               FS_DEV_SD_CARD_ERR_RESP_TIMEOUT    Timeout in receiving command response.
*                               FS_DEV_SD_CARD_ERR_RESP_CHKSUM     Error in response checksum.
*                               FS_DEV_SD_CARD_ERR_RESP_CMD_IX     Response command index error.
*                               FS_DEV_SD_CARD_ERR_RESP_END_BIT    Response end bit error.
*                               FS_DEV_SD_CARD_ERR_RESP            Other response error.
*                               FS_DEV_SD_CARD_ERR_DATA            Other data err.
*
* Return(s)   : none.
*
* Caller(s)   : various.
*
* Note(s)     : (1) This function will be called even if no response is expected from the command.
*
*               (2) This function will NOT be called if 'FSDev_SD_Card_BSP_CmdStart()' returned an error.
*
*               (3) (a) For a command with a normal response, a  4-byte response should be stored in
*                       'p_resp'.
*
*                   (b) For a command with a long   response, a 16-byte response should be stored in
*                       'p_resp'.  The first  4-byte word should hold bits 127..96 of the response.
*                                  The second 4-byte word should hold bits  95..64 of the response.
*                                  The third  4-byte word should hold bits  63..32 of the response.
*                                  The fourth 4-byte word should hold bits  31.. 0 of the reponse.
*********************************************************************************************************
*/

void  FSDev_SD_Card_BSP_CmdWaitEnd (FS_QTY               unit_nbr,
                                    FS_DEV_SD_CARD_CMD  *p_cmd,
                                    CPU_INT32U          *p_resp,
                                    FS_DEV_SD_CARD_ERR  *p_err)
{
    CPU_INT32U  resp[4];
    CPU_INT32U  irqstat;

    FS_OS_SemPend(&FSDev_SD_Sem, 0u);

    irqstat = esdhc->SDHC_IRQSTAT;
    if(irqstat & ZYNQ_BIT_SDHC_IRQSTAT_EI) {
        if(DEF_BIT_IS_SET(irqstat, ZYNQ_BIT_SDHC_IRQSTAT_CTOE)) {
           *p_err = FS_DEV_SD_CARD_ERR_WAIT_TIMEOUT;
        } else if(DEF_BIT_IS_SET(irqstat, ZYNQ_BIT_SDHC_IRQSTAT_DMAE)) {
            *p_err = FS_DEV_SD_CARD_ERR_DATA;
        } else if(DEF_BIT_IS_SET(irqstat, ZYNQ_BIT_SDHC_IRQSTAT_DCE)) {
            *p_err = FS_DEV_SD_CARD_ERR_DATA_CHKSUM;
        } else if(DEF_BIT_IS_SET(irqstat, ZYNQ_BIT_SDHC_IRQSTAT_DTOE)) {
            *p_err = FS_DEV_SD_CARD_ERR_WAIT_TIMEOUT;
        } else {
            *p_err = FS_DEV_SD_CARD_ERR_UNKNOWN;
        }
    } else {
        *p_err = FS_DEV_SD_CARD_ERR_NONE;
    }

    if (DEF_BIT_IS_SET(p_cmd->Flags, FS_DEV_SD_CARD_CMD_FLAG_BUSY) == DEF_YES) {
        *p_err = FS_DEV_SD_CARD_ERR_NONE;
    }

    if (DEF_BIT_IS_SET(p_cmd->Flags, FS_DEV_SD_CARD_CMD_FLAG_RESP_LONG) == DEF_YES) {
        resp[3] = esdhc->SDHC_CMDRSP3;
        resp[2] = esdhc->SDHC_CMDRSP2;
        resp[1] = esdhc->SDHC_CMDRSP1;
        resp[0] = esdhc->SDHC_CMDRSP0;

       *p_resp = resp[3] << 8 | ((resp[2] >> 24) & 0xFF);
        p_resp++;
       *p_resp = resp[2] << 8 | ((resp[1] >> 24) & 0xFF);
        p_resp++;
       *p_resp = resp[1] << 8 | ((resp[0] >> 24) & 0xFF);
        p_resp++;
       *p_resp = resp[0] << 8;
    } else if (DEF_BIT_IS_SET(p_cmd->Flags, FS_DEV_SD_CARD_CMD_FLAG_RESP) == DEF_YES) {
       *p_resp = esdhc->SDHC_CMDRSP0;
    } else {
        ;
    }


    if(DEF_BIT_IS_SET_ANY(irqstat, ZYNQ_BIT_SDHC_IRQSTAT_CIE  |
                                   ZYNQ_BIT_SDHC_IRQSTAT_CEBE |
                                   ZYNQ_BIT_SDHC_IRQSTAT_CCE  |
                                   ZYNQ_BIT_SDHC_IRQSTAT_CTOE)) {
        esdhc->SDHC_SYSCTL |= ZYNQ_BIT_SDHC_SYSCTL_RSTC;
        while(DEF_BIT_IS_SET(esdhc->SDHC_SYSCTL, ZYNQ_BIT_SDHC_SYSCTL_RSTC)) {
            ;
        }

        esdhc->SDHC_IRQSTAT = ZYNQ_BIT_SDHC_IRQSTAT_CIE  |
                              ZYNQ_BIT_SDHC_IRQSTAT_CEBE |
                              ZYNQ_BIT_SDHC_IRQSTAT_CCE  |
                              ZYNQ_BIT_SDHC_IRQSTAT_CTOE;
    }

    if(DEF_BIT_IS_SET_ANY(irqstat, ZYNQ_BIT_SDHC_IRQSTAT_DEBE |
                                   ZYNQ_BIT_SDHC_IRQSTAT_DCE  |
                                   ZYNQ_BIT_SDHC_IRQSTAT_DTOE)) {
        esdhc->SDHC_SYSCTL |= ZYNQ_BIT_SDHC_SYSCTL_RSTD;
        while(DEF_BIT_IS_SET(esdhc->SDHC_SYSCTL, ZYNQ_BIT_SDHC_SYSCTL_RSTD)) {
            ;
        }

        esdhc->SDHC_IRQSTAT = ZYNQ_BIT_SDHC_IRQSTAT_DEBE |
                              ZYNQ_BIT_SDHC_IRQSTAT_DCE  |
                              ZYNQ_BIT_SDHC_IRQSTAT_DTOE;
    }

    if(DEF_BIT_IS_SET(p_cmd->Flags, FS_DEV_SD_CARD_CMD_FLAG_STOP_DATA_TX) == DEF_YES) {
        esdhc->SDHC_SYSCTL |= ZYNQ_BIT_SDHC_SYSCTL_RSTC;
        esdhc->SDHC_SYSCTL |= ZYNQ_BIT_SDHC_SYSCTL_RSTD;
        while(DEF_BIT_IS_SET(esdhc->SDHC_SYSCTL, ZYNQ_BIT_SDHC_SYSCTL_RSTC)) {
            ;
        }
        while(DEF_BIT_IS_SET(esdhc->SDHC_SYSCTL, ZYNQ_BIT_SDHC_SYSCTL_RSTD)) {
            ;
        }

       *p_err = FS_DEV_SD_CARD_ERR_WAIT_TIMEOUT;
    }

    CPU_MB();

    esdhc->SDHC_IRQSTAT = ZYNQ_BIT_SDHC_IRQSTAT_CC;

    return;
}


/*
*********************************************************************************************************
*                                    FSDev_SD_Card_BSP_CmdDataRd()
*
* Description : Read data following command.
*
* Argument(s) : unit_nbr    Unit number of SD/MMC card.
*
*               p_cmd       Pointer to command that was started.
*
*               p_dest      Pointer to destination buffer.
*
*               p_err       Pointer to variable that will receive the return error code from this function :
*
*                               FS_DEV_SD_CARD_ERR_NONE              No error.
*                               FS_DEV_SD_CARD_ERR_NO_CARD           No card present.
*                               FS_DEV_SD_CARD_ERR_UNKNOWN           Unknown or other error.
*                               FS_DEV_SD_CARD_ERR_WAIT_TIMEOUT      Timeout in waiting for data.
*                               FS_DEV_SD_CARD_ERR_DATA_OVERRUN      Data overrun.
*                               FS_DEV_SD_CARD_ERR_DATA_TIMEOUT      Timeout in receiving data.
*                               FS_DEV_SD_CARD_ERR_DATA_CHKSUM       Error in data checksum.
*                               FS_DEV_SD_CARD_ERR_DATA_START_BIT    Data start bit error.
*                               FS_DEV_SD_CARD_ERR_DATA              Other data error.
*
* Return(s)   : none.
*
* Caller(s)   : FSDev_SD_Card_RdData().
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  FSDev_SD_Card_BSP_CmdDataRd (FS_QTY               unit_nbr,
                                   FS_DEV_SD_CARD_CMD  *p_cmd,
                                   void                *p_dest,
                                   FS_DEV_SD_CARD_ERR  *p_err)
{
    CPU_INT32U  irqstat;


    CPU_MB();
#if 0
    esdhc->SDHC_SYSCTL |=   ZYNQ_BIT_SDHC_SYSCTL_PEREN |
                            ZYNQ_BIT_SDHC_SYSCTL_IPGEN;

    esdhc->SDHC_PROCTL |= DEF_BIT_08;
#endif

    esdhc->SDHC_IRQSIGEN = ZYNQ_BIT_SDHC_IRQSTAT_TC   |
                           ZYNQ_BIT_SDHC_IRQSTAT_DMAE |
                           ZYNQ_BIT_SDHC_IRQSTAT_DINT |
                           ZYNQ_BIT_SDHC_IRQSTAT_DTOE |
                           ZYNQ_BIT_SDHC_IRQSTAT_CRM;

    FS_OS_SemPend(&FSDev_SD_Sem, 0u);

    irqstat = esdhc->SDHC_IRQSTAT;
    if(DEF_BIT_IS_SET(irqstat, ZYNQ_BIT_SDHC_IRQSTAT_EI)) {
        if(DEF_BIT_IS_SET(irqstat, ZYNQ_BIT_SDHC_IRQSTAT_DMAE)) {
            *p_err = FS_DEV_SD_CARD_ERR_DATA;
        } else if(DEF_BIT_IS_SET(irqstat, ZYNQ_BIT_SDHC_IRQSTAT_DCE)) {
            *p_err = FS_DEV_SD_CARD_ERR_DATA_CHKSUM;
        } else if(DEF_BIT_IS_SET(irqstat, ZYNQ_BIT_SDHC_IRQSTAT_DTOE)) {
            *p_err = FS_DEV_SD_CARD_ERR_WAIT_TIMEOUT;
        } else {
            *p_err = FS_DEV_SD_CARD_ERR_UNKNOWN;
        }
    } else {
        *p_err = FS_DEV_SD_CARD_ERR_NONE;
    }

    if(DEF_BIT_IS_CLR(irqstat, ZYNQ_BIT_SDHC_IRQSTAT_TC)) {
        *p_err = FS_DEV_SD_CARD_ERR_WAIT_TIMEOUT;
    }

    CPU_DCACHE_RANGE_FLUSH(p_dest, p_cmd->BlkCnt * p_cmd->BlkSize);
    CPU_DCACHE_RANGE_INV(p_dest, p_cmd->BlkCnt * p_cmd->BlkSize);

    if(DEF_BIT_IS_SET_ANY(irqstat, ZYNQ_BIT_SDHC_IRQSTAT_DEBE |
                                   ZYNQ_BIT_SDHC_IRQSTAT_DCE  |
                                   ZYNQ_BIT_SDHC_IRQSTAT_DTOE)) {
        esdhc->SDHC_SYSCTL |= ZYNQ_BIT_SDHC_SYSCTL_RSTD;
        while(DEF_BIT_IS_SET(esdhc->SDHC_SYSCTL, ZYNQ_BIT_SDHC_SYSCTL_RSTD)) {
            ;
        }

        esdhc->SDHC_IRQSTAT = ZYNQ_BIT_SDHC_IRQSTAT_DEBE |
                              ZYNQ_BIT_SDHC_IRQSTAT_DCE  |
                              ZYNQ_BIT_SDHC_IRQSTAT_DTOE;
    }

    esdhc->SDHC_IRQSTAT = 0xFFFFFFFF;

    CPU_MB();

    return;
}


/*
*********************************************************************************************************
*                                    FSDev_SD_Card_BSP_CmdDataWr()
*
* Description : Write data following command.
*
* Argument(s) : unit_nbr    Unit number of SD/MMC card.
*
*               p_cmd       Pointer to command that was started.
*
*               p_src       Pointer to source buffer.
*
*               p_err       Pointer to variable that will receive the return error code from this function :
*
*                               FS_DEV_SD_CARD_ERR_NONE              No error.
*                               FS_DEV_SD_CARD_ERR_NO_CARD           No card present.
*                               FS_DEV_SD_CARD_ERR_UNKNOWN           Unknown or other error.
*                               FS_DEV_SD_CARD_ERR_WAIT_TIMEOUT      Timeout in waiting for data.
*                               FS_DEV_SD_CARD_ERR_DATA_UNDERRUN     Data underrun.
*                               FS_DEV_SD_CARD_ERR_DATA_CHKSUM       Err in data checksum.
*                               FS_DEV_SD_CARD_ERR_DATA_START_BIT    Data start bit error.
*                               FS_DEV_SD_CARD_ERR_DATA              Other data error.
*
* Return(s)   : none.
*
* Caller(s)   : FSDev_SD_Card_WrData().
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  FSDev_SD_Card_BSP_CmdDataWr (FS_QTY               unit_nbr,
                                   FS_DEV_SD_CARD_CMD  *p_cmd,
                                   void                *p_src,
                                   FS_DEV_SD_CARD_ERR  *p_err)
{
    CPU_INT32U  irqstat;

    CPU_MB();
#if 0
    esdhc->SDHC_SYSCTL |=   ZYNQ_BIT_SDHC_SYSCTL_PEREN        |
                            ZYNQ_BIT_SDHC_SYSCTL_IPGEN;

    esdhc->SDHC_PROCTL |= DEF_BIT_08;
#endif
    esdhc->SDHC_IRQSIGEN = ZYNQ_BIT_SDHC_IRQSTAT_TC   |
                           ZYNQ_BIT_SDHC_IRQSTAT_DMAE |
                           ZYNQ_BIT_SDHC_IRQSTAT_DINT |
                           ZYNQ_BIT_SDHC_IRQSTAT_DTOE |
                           ZYNQ_BIT_SDHC_IRQSTAT_EI;

    FS_OS_SemPend(&FSDev_SD_Sem, 0u);

    irqstat = esdhc->SDHC_IRQSTAT;
    if(DEF_BIT_IS_SET(irqstat, ZYNQ_BIT_SDHC_IRQSTAT_EI)) {
        if(DEF_BIT_IS_SET(irqstat, ZYNQ_BIT_SDHC_IRQSTAT_DMAE)) {
            *p_err = FS_DEV_SD_CARD_ERR_DATA;
        } else if(DEF_BIT_IS_SET(irqstat, ZYNQ_BIT_SDHC_IRQSTAT_DCE)) {
            *p_err = FS_DEV_SD_CARD_ERR_DATA_CHKSUM;
        } else if(DEF_BIT_IS_SET(irqstat, ZYNQ_BIT_SDHC_IRQSTAT_DTOE)) {
            *p_err = FS_DEV_SD_CARD_ERR_WAIT_TIMEOUT;
        } else {
            *p_err = FS_DEV_SD_CARD_ERR_UNKNOWN;
        }
    } else {
        *p_err = FS_DEV_SD_CARD_ERR_NONE;
    }

    if(DEF_BIT_IS_SET_ANY(irqstat, ZYNQ_BIT_SDHC_IRQSTAT_DEBE |
                                   ZYNQ_BIT_SDHC_IRQSTAT_DCE  |
                                   ZYNQ_BIT_SDHC_IRQSTAT_DTOE)) {
        esdhc->SDHC_SYSCTL |= ZYNQ_BIT_SDHC_SYSCTL_RSTD;
        while(DEF_BIT_IS_SET(esdhc->SDHC_SYSCTL, ZYNQ_BIT_SDHC_SYSCTL_RSTD)) {
            ;
        }

        esdhc->SDHC_IRQSTAT = ZYNQ_BIT_SDHC_IRQSTAT_DEBE |
                              ZYNQ_BIT_SDHC_IRQSTAT_DCE  |
                              ZYNQ_BIT_SDHC_IRQSTAT_DTOE;
    }

    esdhc->SDHC_IRQSTAT = 0xFFFFFFFF;

    CPU_MB();

    return;
}


/*
*********************************************************************************************************
*                                  FSDev_SD_Card_BSP_GetBlkCntMax()
*
* Description : Get maximum number of blocks that can be transferred with a multiple read or multiple
*               write command.
*
* Argument(s) : unit_nbr    Unit number of SD/MMC card.
*
*               blk_size    Block size, in octets.
*
* Return(s)   : Maximum number of blocks.
*
* Caller(s)   : FSDev_SD_Card_Refresh().
*
* Note(s)     : (1) The DMA region from which data is read or written may be a limited size.  The count
*                   returned by this function should be the number of blocks of size 'blk_size' that can
*                   fit into this region.
*
*               (2) If the controller is not capable of multiple block reads or writes, 1 should be
*                   returned.
*
*               (3) If the controller has no limit on the number of blocks in a multiple block read or
*                   write, 'DEF_INT_32U_MAX_VAL' should be returned.
*
*               (4) This function SHOULD always return the same value.  If hardware constraints change
*                   at run-time, the device MUST be closed & re-opened for any changes to be effective.
*********************************************************************************************************
*/

CPU_INT32U  FSDev_SD_Card_BSP_GetBlkCntMax (FS_QTY      unit_nbr,
                                            CPU_INT32U  blk_size)
{
    return (65535u);
}


/*
*********************************************************************************************************
*                                 FSDev_SD_Card_BSP_GetBusWidthMax()
*
* Description : Get maximum bus width, in bits.
*
* Argument(s) : unit_nbr    Unit number of SD/MMC card.
*
* Return(s)   : Maximum bus width.
*
* Caller(s)   : FSDev_SD_Card_Refresh().
*
* Note(s)     : (1) Legal values are typically 1, 4 & 8.
*
*               (2) This function SHOULD always return the same value.  If hardware constraints change
*                   at run-time, the device MUST be closed & re-opened for any changes to be effective.
*********************************************************************************************************
*/

CPU_INT08U  FSDev_SD_Card_BSP_GetBusWidthMax (FS_QTY  unit_nbr)
{
    return (4u);
}


/*
*********************************************************************************************************
*                                   FSDev_SD_Card_BSP_SetBusWidth()
*
* Description : Set bus width.
*
* Argument(s) : unit_nbr    Unit number of SD/MMC card.
*
*               width       Bus width, in bits.
*
* Return(s)   : none.
*
* Caller(s)   : FSDev_SD_Card_Refresh(),
*               FSDev_SD_Card_SetBusWidth().
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  FSDev_SD_Card_BSP_SetBusWidth (FS_QTY      unit_nbr,
                                     CPU_INT08U  width)
{

    if(width == 1u) {
        esdhc->SDHC_PROCTL &= ~ZYNQ_BIT_SDHC_PROCTL_DTW;
    } else {
        esdhc->SDHC_PROCTL |= ZYNQ_BIT_SDHC_PROCTL_DTW;
    }

    return;
}


/*
*********************************************************************************************************
*                                   FSDev_SD_Card_BSP_SetClkFreq()
*
* Description : Set clock frequency.
*
* Argument(s) : unit_nbr    Unit number of SD/MMC card.
*
*               freq        Clock frequency, in Hz.
*
* Return(s)   : none.
*
* Caller(s)   : FSDev_SD_Card_Refresh().
*
* Note(s)     : (1) The effective clock frequency MUST be no more than 'freq'.  If the frequency cannot be
*                   configured equal to 'freq', it should be configured less than 'freq'.
*********************************************************************************************************
*/

void  FSDev_SD_Card_BSP_SetClkFreq (FS_QTY      unit_nbr,
                                    CPU_INT32U  freq)
{
    CPU_INT32U   div;


    div = ((SD_ROOT_CLK_FREQ + freq - 1u) / (freq)) - 1u;

    esdhc->SDHC_PROCTL |= DEF_BIT_08;

    esdhc->SDHC_SYSCTL &= ~ZYNQ_BIT_SDHC_SYSCTL_SDCLKEN;

    esdhc->SDHC_SYSCTL =    ZYNQ_BIT_SDHC_SYSCTL_DTOCV(8)     |
                            ZYNQ_BIT_SDHC_SYSCTL_SDCLKFS(div) |
                            ZYNQ_BIT_SDHC_SYSCTL_PEREN        |
                            ZYNQ_BIT_SDHC_SYSCTL_IPGEN;

    //while(DEF_BIT_IS_CLR(esdhc->SDHC_SYSCTL, ZYNQ_BIT_SDHC_SYSCTL_CLKSTB)) {
    //    ;
   // }

    esdhc->SDHC_SYSCTL |= ZYNQ_BIT_SDHC_SYSCTL_SDCLKEN;

    return;
}


/*
*********************************************************************************************************
*                                 FSDev_SD_Card_BSP_SetTimeoutData()
*
* Description : Set data timeout.
*
* Argument(s) : unit_nbr    Unit number of SD/MMC card.
*
*               to_clks     Timeout, in clocks.
*
* Return(s)   : none.
*
* Caller(s)   : FSDev_SD_Card_Refresh().
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  FSDev_SD_Card_BSP_SetTimeoutData (FS_QTY      unit_nbr,
                                        CPU_INT32U  to_clks)
{

}


/*
*********************************************************************************************************
*                                 FSDev_SD_Card_BSP_SetTimeoutResp()
*
* Description : Set response timeout.
*
* Argument(s) : unit_nbr    Unit number of SD/MMC card.
*
*               to_ms       Timeout, in milliseconds.
*
* Return(s)   : none.
*
* Caller(s)   : FSDev_SD_Card_Refresh().
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  FSDev_SD_Card_BSP_SetTimeoutResp (FS_QTY      unit_nbr,
                                        CPU_INT32U  to_ms)
{

}


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                    FSDev_SD_Card_BSP_ISR_Handler()
*
* Description : Interrupt handler for the SDIO.
*
* Argument(s) : source  Interrupt source undefined/ignored for peripheral interrupts.
*
* Return(s)   : none.
*
* Caller(s)   : FSDev_SD_Card_BSP_Open().
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

void  FSDev_SD_Card_BSP_ISR_Handler (void  *p_arg)
{
                                                                /* DMA interrupt, continue transfer.                    */
    if((DEF_BIT_IS_SET(esdhc->SDHC_IRQSTAT, ZYNQ_BIT_SDHC_IRQSTAT_DINT) == DEF_YES) &&
       (DEF_BIT_IS_SET(esdhc->SDHC_IRQSTAT, ZYNQ_BIT_SDHC_IRQSTAT_EI) == DEF_NO)) {
        esdhc->SDHC_IRQSTAT= ZYNQ_BIT_SDHC_IRQSTAT_DINT;
        esdhc->SDHC_DSADDR = esdhc->SDHC_DSADDR;

        return;
    }


    esdhc->SDHC_IRQSIGEN = 0x0;                                 /* Disable all interrupt signals.                       */

    FS_OS_SemPost(&FSDev_SD_Sem);                               /* Wake up the pending task.                            */

    return;
}
