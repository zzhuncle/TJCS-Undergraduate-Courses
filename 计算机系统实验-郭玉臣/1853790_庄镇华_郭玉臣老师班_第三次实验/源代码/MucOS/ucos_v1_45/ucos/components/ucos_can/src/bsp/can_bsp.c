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
*                                        CAN ZC7xxx DRIVER BSP
*
*                                            ZYNQ 7000 EPP
*                                               on the
*                                       ZC702 development board
*
* File : can_bsp.c
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                              INCLUDES
*********************************************************************************************************
*/

#include  "drv_can.h"
#include  "drv_def.h"
#include  "can_bsp.h"

#if ((CANBUS_RX_HANDLER_EN > 0u) || \
     (CANBUS_TX_HANDLER_EN > 0u) || \
     (CANBUS_NS_HANDLER_EN > 0u))
#include  "can_bus.h"
#endif

#include  <ucos_int.h>


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/
                                                                /* -------------- CANx BASE ADDR POINTERS ------------- */
#if (CAN_MODULE_CHANNEL_0 == DEF_ENABLED)
    ZC7xxx_CAN_REG  *Can0_Reg_ptr;                              /* Pointer to the Base Address of the CAN0 Controller.  */
#endif
#if (CAN_MODULE_CHANNEL_1 == DEF_ENABLED)
    ZC7xxx_CAN_REG  *Can1_Reg_ptr;                              /* Pointer to the Base Address of the CAN1 Controller.  */
#endif


#define  ZYNQ_SLCR_LOCK_KEY    (0x767Bu)                        /* SLCR lock key.                                       */
#define  ZYNQ_SLCR_UNLOCK_KEY  (0xDF0Du)                        /* SLCR unlock key.                                     */


#define  ZYNQ_SLCR_REG         ((ZYNQ_REG_SLCR_PTR)(0xF8000000))

                                                                /* System Level Control Registers                       */
typedef  struct  zynq_reg_slcr {
    CPU_REG32  SCL;                                             /* Secure Configuration Lock.                           */
    CPU_REG32  SLCR_LOCK;                                       /* SLCR Write Protection Lock.                          */
    CPU_REG32  SLCR_UNLOCK;                                     /* SLCR Write Protection Unlock.                        */
    CPU_REG32  SLCR_LOCKSTA;                                    /* SLCR Write Protection Status.                        */
    CPU_REG32  RESERVED1[60];
    CPU_REG32  ARM_PLL_CTRL;                                    /* ARM PLL Control.                                     */
    CPU_REG32  DDR_PLL_CTRL;                                    /* DDR PLL Control.                                     */
    CPU_REG32  IO_PLL_CTRL;                                     /* IO PLL Control.                                      */
    CPU_REG32  PLL_STATUS;                                      /* PLL Status.                                          */
    CPU_REG32  ARM_PLL_CFG;                                     /* ARM PLL Configuration.                               */
    CPU_REG32  DDR_PLL_CFG;                                     /* DDR PLL Configuration.                               */
    CPU_REG32  IO_PLL_CFG;                                      /* IO PLL Configuration.                                */
    CPU_REG32  RESERVED2[1];
    CPU_REG32  ARM_CLK_CTRL;                                    /* ARM Clock Control.                                   */
    CPU_REG32  DDR_CLK_CTRL;                                    /* DDR Clock Control.                                   */
    CPU_REG32  DCI_CLK_CTRL;                                    /* DCI Clock Control.                                   */
    CPU_REG32  APER_CLK_CTRL;                                   /* AMBA Peripheral Clock Control.                       */
    CPU_REG32  USB0_CLK_CTRL;                                   /* USB 0 ULPI Clock Control.                            */
    CPU_REG32  USB1_CLK_CTRL;                                   /* USB 1 ULPI Clock Control.                            */
    CPU_REG32  GEM0_RCLK_CTRL;                                  /* GigE 0 Rx Clock and Rx Signals Select.               */
    CPU_REG32  GEM1_RCLK_CTRL;                                  /* GigE 1 Rx Clock and Rx Signals Select.               */
    CPU_REG32  GEM0_CLK_CTRL;                                   /* GigE 0 Ref Clock Control.                            */
    CPU_REG32  GEM1_CLK_CTRL;                                   /* GigE 1 Ref Clock Control.                            */
    CPU_REG32  SMC_CLK_CTRL;                                    /* SMC Ref Clock Control.                               */
    CPU_REG32  LQSPI_CLK_CTRL;                                  /* Quad SPI Ref Clock Control.                          */
    CPU_REG32  SDIO_CLK_CTRL;                                   /* SDIO Ref Clock Control.                              */
    CPU_REG32  UART_CLK_CTRL;                                   /* UART Ref Clock Control.                              */
    CPU_REG32  SPI_CLK_CTRL;                                    /* SPI Ref Clock Control.                               */
    CPU_REG32  CAN_CLK_CTRL;                                    /* CAN Ref Clock Control.                               */
    CPU_REG32  CAN_MIOCLK_CTRL;                                 /* CAN MIO Clock Control.                               */
    CPU_REG32  DBG_CLK_CTRL;                                    /* SoC Debug Clock Control.                             */
    CPU_REG32  PCAP_CLK_CTRL;                                   /* PCAP Clock Control.                                  */
    CPU_REG32  TOPSW_CLK_CTRL;                                  /* Central Interconnect Clock Control.                  */
    CPU_REG32  FPGA0_CLK_CTRL;                                  /* PL Clock 0 Output Control.                           */
    CPU_REG32  RESERVED3[3];
    CPU_REG32  FPGA1_CLK_CTRL;                                  /* PL Clock 1 Output Control.                           */
    CPU_REG32  RESERVED4[3];
    CPU_REG32  FPGA2_CLK_CTRL;                                  /* PL Clock 2 Output Control.                           */
    CPU_REG32  RESERVED5[3];
    CPU_REG32  FPGA3_CLK_CTRL;                                  /* PL Clock 3 Output Control.                           */
    CPU_REG32  RESERVED6[8];
    CPU_REG32  CLK_621_TRUE;                                    /* CPU Clock Ratio Mode Select.                         */
    CPU_REG32  RESERVED7[14];
    CPU_REG32  PSS_RST_CTRL;                                    /* PS Software Reset Control.                           */
    CPU_REG32  DDR_RST_CTRL;                                    /* DRR Software Reset Control.                          */
    CPU_REG32  TOPSW_RST_CTRL;                                  /* Central Interconnect Reset Control.                  */
    CPU_REG32  DMAC_RST_CTRL;                                   /* DMAC Software Reset Control.                         */
    CPU_REG32  USB_RST_CTRL;                                    /* USB Software Reset Control.                          */
    CPU_REG32  GEM_RST_CTRL;                                    /* Gigabit Ethernet SW Reset Control.                   */
    CPU_REG32  SDIO_RST_CTRL;                                   /* SDIO Software Reset Control.                         */
    CPU_REG32  SPI_RST_CTRL;                                    /* SPI Software Reset Control.                          */
    CPU_REG32  CAN_RST_CTRL;                                    /* CAN Software Reset Control.                          */
    CPU_REG32  I2C_RST_CTRL;                                    /* I2C Software Reset Control.                          */
    CPU_REG32  UART_RST_CTRL;                                   /* UART Software Reset Control.                         */
    CPU_REG32  GPIO_RST_CTRL;                                   /* GPIO Software Reset Control.                         */
    CPU_REG32  LQSPI_RST_CTRL;                                  /* Quad SPI Software Reset Control.                     */
    CPU_REG32  SMC_RST_CTRL;                                    /* SMC Software Reset Control.                          */
    CPU_REG32  OCM_RST_CTRL;                                    /* OMC Software Reset Control.                          */
    CPU_REG32  DEVCI_RST_CTRL;                                  /* Device Config Interface SW Reset Control.            */
    CPU_REG32  FPGA_RST_CTRL;                                   /* FPGA Software Reset Control.                         */
    CPU_REG32  A9_CPU_RST_CTRL;                                 /* CPU Reset and Clock Control.                         */
    CPU_REG32  RESERVED8[1];
    CPU_REG32  RS_AWDT_CTRL;                                    /* Watchdog Timer Reset Control.                        */
    CPU_REG32  RESERVED9[2];
    CPU_REG32  REBOOT_STATUS;                                   /* Reboot Status.                                       */
    CPU_REG32  BOOT_MODE;                                       /* Boot Mode Strapping Pins.                            */
    CPU_REG32  RESERVED10[40];
    CPU_REG32  APU_CTRL;                                        /* APU Control.                                         */
    CPU_REG32  WDT_CLK_SEL;                                     /* APU Watchdog Timer Clock Select.                     */
    CPU_REG32  RESERVED11[138];
    CPU_REG32  PSS_IDCODE;                                      /* PS IDCODE.                                           */
    CPU_REG32  RESERVED12[51];
    CPU_REG32  DDR_URGENT;                                      /* DDR Urgent Control.                                  */
    CPU_REG32  RESERVED13[2];
    CPU_REG32  DDR_CAL_START;                                   /* DDR Calibration Start Triggers.                      */
    CPU_REG32  RESERVED14[1];
    CPU_REG32  DDR_REF_START;                                   /* DDR Refresh Start Triggers.                          */
    CPU_REG32  DDR_CMD_STA;                                     /* DDR Command Store Status.                            */
    CPU_REG32  DDR_URGENT_SEL;                                  /* DDR Urgent Select.                                   */
    CPU_REG32  DDR_DFI_STATUS;                                  /* DDR DFI Status.                                      */
    CPU_REG32  RESERVED15[55];
    CPU_REG32  MIO_PIN_00;                                      /* MIO Pin 0 Control.                                   */
    CPU_REG32  MIO_PIN_01;                                      /* MIO Pin 1 Control.                                   */
    CPU_REG32  MIO_PIN_02;                                      /* MIO Pin 2 Control.                                   */
    CPU_REG32  MIO_PIN_03;                                      /* MIO Pin 3 Control.                                   */
    CPU_REG32  MIO_PIN_04;                                      /* MIO Pin 4 Control.                                   */
    CPU_REG32  MIO_PIN_05;                                      /* MIO Pin 5 Control.                                   */
    CPU_REG32  MIO_PIN_06;                                      /* MIO Pin 6 Control.                                   */
    CPU_REG32  MIO_PIN_07;                                      /* MIO Pin 7 Control.                                   */
    CPU_REG32  MIO_PIN_08;                                      /* MIO Pin 8 Control.                                   */
    CPU_REG32  MIO_PIN_09;                                      /* MIO Pin 9 Control.                                   */
    CPU_REG32  MIO_PIN_10;                                      /* MIO Pin 10 Control.                                  */
    CPU_REG32  MIO_PIN_11;                                      /* MIO Pin 11 Control.                                  */
    CPU_REG32  MIO_PIN_12;                                      /* MIO Pin 12 Control.                                  */
    CPU_REG32  MIO_PIN_13;                                      /* MIO Pin 13 Control.                                  */
    CPU_REG32  MIO_PIN_14;                                      /* MIO Pin 14 Control.                                  */
    CPU_REG32  MIO_PIN_15;                                      /* MIO Pin 15 Control.                                  */
    CPU_REG32  MIO_PIN_16;                                      /* MIO Pin 16 Control.                                  */
    CPU_REG32  MIO_PIN_17;                                      /* MIO Pin 17 Control.                                  */
    CPU_REG32  MIO_PIN_18;                                      /* MIO Pin 18 Control.                                  */
    CPU_REG32  MIO_PIN_19;                                      /* MIO Pin 19 Control.                                  */
    CPU_REG32  MIO_PIN_20;                                      /* MIO Pin 20 Control.                                  */
    CPU_REG32  MIO_PIN_21;                                      /* MIO Pin 21 Control.                                  */
    CPU_REG32  MIO_PIN_22;                                      /* MIO Pin 22 Control.                                  */
    CPU_REG32  MIO_PIN_23;                                      /* MIO Pin 23 Control.                                  */
    CPU_REG32  MIO_PIN_24;                                      /* MIO Pin 24 Control.                                  */
    CPU_REG32  MIO_PIN_25;                                      /* MIO Pin 25 Control.                                  */
    CPU_REG32  MIO_PIN_26;                                      /* MIO Pin 26 Control.                                  */
    CPU_REG32  MIO_PIN_27;                                      /* MIO Pin 27 Control.                                  */
    CPU_REG32  MIO_PIN_28;                                      /* MIO Pin 28 Control.                                  */
    CPU_REG32  MIO_PIN_29;                                      /* MIO Pin 29 Control.                                  */
    CPU_REG32  MIO_PIN_30;                                      /* MIO Pin 30 Control.                                  */
    CPU_REG32  MIO_PIN_31;                                      /* MIO Pin 31 Control.                                  */
    CPU_REG32  MIO_PIN_32;                                      /* MIO Pin 32 Control.                                  */
    CPU_REG32  MIO_PIN_33;                                      /* MIO Pin 33 Control.                                  */
    CPU_REG32  MIO_PIN_34;                                      /* MIO Pin 34 Control.                                  */
    CPU_REG32  MIO_PIN_35;                                      /* MIO Pin 35 Control.                                  */
    CPU_REG32  MIO_PIN_36;                                      /* MIO Pin 36 Control.                                  */
    CPU_REG32  MIO_PIN_37;                                      /* MIO Pin 37 Control.                                  */
    CPU_REG32  MIO_PIN_38;                                      /* MIO Pin 38 Control.                                  */
    CPU_REG32  MIO_PIN_39;                                      /* MIO Pin 39 Control.                                  */
    CPU_REG32  MIO_PIN_40;                                      /* MIO Pin 40 Control.                                  */
    CPU_REG32  MIO_PIN_41;                                      /* MIO Pin 41 Control.                                  */
    CPU_REG32  MIO_PIN_42;                                      /* MIO Pin 42 Control.                                  */
    CPU_REG32  MIO_PIN_43;                                      /* MIO Pin 43 Control.                                  */
    CPU_REG32  MIO_PIN_44;                                      /* MIO Pin 44 Control.                                  */
    CPU_REG32  MIO_PIN_45;                                      /* MIO Pin 45 Control.                                  */
    CPU_REG32  MIO_PIN_46;                                      /* MIO Pin 46 Control.                                  */
    CPU_REG32  MIO_PIN_47;                                      /* MIO Pin 47 Control.                                  */
    CPU_REG32  MIO_PIN_48;                                      /* MIO Pin 48 Control.                                  */
    CPU_REG32  MIO_PIN_49;                                      /* MIO Pin 49 Control.                                  */
    CPU_REG32  MIO_PIN_50;                                      /* MIO Pin 50 Control.                                  */
    CPU_REG32  MIO_PIN_51;                                      /* MIO Pin 51 Control.                                  */
    CPU_REG32  MIO_PIN_52;                                      /* MIO Pin 52 Control.                                  */
    CPU_REG32  MIO_PIN_53;                                      /* MIO Pin 53 Control.                                  */
    CPU_REG32  RESERVED16[11];
    CPU_REG32  MIO_LOOPBACK;                                    /* Loopback Function Within MIO.                        */
    CPU_REG32  RESERVED17[1];
    CPU_REG32  MIO_MST_TRI0;                                    /* MIO Pin Tri-state Enables 31:0.                      */
    CPU_REG32  MIO_MST_TRI1;                                    /* MIO Pin Tri-state Enables 53:32.                     */
    CPU_REG32  RESERVED18[7];
    CPU_REG32  SD0_WP_CD_SEL;                                   /* SDIO 0 WP CD Select.                                 */
    CPU_REG32  SD1_WP_CD_SEL;                                   /* SDIO 1 WP CD Select.                                 */
    CPU_REG32  RESERVED19[50];
    CPU_REG32  LVL_SHFTR_EN;                                    /* Level Shifters Enable.                               */
    CPU_REG32  RESERVED20[3];
    CPU_REG32  OCM_CFG;                                         /* OCM Address Mapping.                                 */
    CPU_REG32  RESERVED21[123];
    CPU_REG32  GPIOB_CTRL;                                      /* PS IO Buffer Control.                                */
    CPU_REG32  GPIOB_CFG_CMOS18;                                /* MIO GPIOB CMOS 1.8V Configuration.                   */
    CPU_REG32  GPIOB_CFG_CMOS25;                                /* MIO GPIOB CMOS 2.5V Configuration.                   */
    CPU_REG32  GPIOB_CFG_CMOS33;                                /* MIO GPIOB CMOS 3.3V Configuration.                   */
    CPU_REG32  GPIOB_CFG_LVTTL;                                 /* MIO GPIOB LVTTL Configuration.                       */
    CPU_REG32  GPIOB_CFG_HSTL;                                  /* MIO GPIOB HSTL Configuration.                        */
    CPU_REG32  GPIOB_DRVR_BIAS_CTRL;                            /* MIO GPIOB Driver Bias Control.                       */
    CPU_REG32  RESERVED22[9];
    CPU_REG32  DDRIOB_ADDR0;                                    /* DDR IOB Configuration for A[14:0], CKE and DRST_B.   */
    CPU_REG32  DDRIOB_ADDR1;                                    /* DDR IOB Configuration for BS[2:0], ODT, CS_B, WE_B, RAS_B and CAS_B. */
    CPU_REG32  DDRIOB_DATA0;                                    /* DDR IOB Configuration for Data 15:0.                 */
    CPU_REG32  DDRIOB_DATA1;                                    /* DDR IOB Configuration for Data 31:16.                */
    CPU_REG32  DDRIOB_DIFF0;                                    /* DDR IOB Configuration for DQS 1:0.                   */
    CPU_REG32  DDRIOB_DIFF1;                                    /* DDR IOB Configuration for DQS 3:2.                   */
    CPU_REG32  DDRIOB_CLOCK;                                    /* DDR IOB Configuration for Clock Output.              */
    CPU_REG32  DDRIOB_DRIVE_SLEW_ADDR;                          /* Drive and Slew control Address and Command.          */
    CPU_REG32  DDRIOB_DRIVE_SLEW_DATA;                          /* Drive and Slew control DQ.                           */
    CPU_REG32  DDRIOB_DRIVE_SLEW_DIFF;                          /* Drive and Slew control DQS.                          */
    CPU_REG32  DDRIOB_DRIVE_SLEW_CLOCK;                         /* Drive and Slew control Clock.                        */
    CPU_REG32  DDRIOB_DDR_CTRL;                                 /* DDR IOB Buffer Control.                              */
} ZYNQ_REG_SLCR, *ZYNQ_REG_SLCR_PTR;


/* -------------- MIO REG BIT DEFINITION -------------- */
#define  ZYNQ_BIT_MIO_DISABLE_RCVR    DEF_BIT_13                /* Disable HSTL Input Buffer.                           */
#define  ZYNQ_BIT_MIO_PULL_UP         DEF_BIT_12                /* Enable Pull-Up.                                      */
#define  ZYNQ_BIT_MIO_IO_TYPE_MSK (DEF_BIT_FIELD(3, 9))         /* IO Buffer Type.                                      */
#define  ZYNQ_BIT_MIO_IO_TYPE(cfg) (DEF_BIT_MASK(cfg, 9) & ZYNQ_BIT_MIO_IO_TYPE_MSK)
#define  ZYNQ_BIT_MIO_SPEED           DEF_BIT_08                /* IO Buffer Edge Rate.                                 */
#define  ZYNQ_BIT_MIO_L3_SEL_MSK (DEF_BIT_FIELD(3, 5))          /* Level 3 Mux Select.                                  */
#define  ZYNQ_BIT_MIO_L3_SEL(cfg) (DEF_BIT_MASK(cfg, 5) & ZYNQ_BIT_MIO_L3_SEL_MSK)
#define  ZYNQ_BIT_MIO_L2_SEL_MSK (DEF_BIT_FIELD(2, 3))          /* Level 2 Mux Select.                                  */
#define  ZYNQ_BIT_MIO_L2_SEL(cfg) (DEF_BIT_MASK(cfg, 3) & ZYNQ_BIT_MIO_L2_SEL_MSK)
#define  ZYNQ_BIT_MIO_L1_SEL          DEF_BIT_02                /* Level 1 Mux Select.                                  */
#define  ZYNQ_BIT_MIO_L0_SEL          DEF_BIT_01                /* Level 0 Mux Select.                                  */
#define  ZYNQ_BIT_MIO_TRI_ENABLE      DEF_BIT_00                /* Tri-State Enable.                                    */


/*
*********************************************************************************************************
*                                               MACROS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         INTERNAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             LOCAL DATA
*********************************************************************************************************
*/

static  CPU_INT08U  ZC7xxx_CAN_DevIds[ZC7xxx_CAN_N_DEV];        /* Array Holds Initialized Node ID of Module            */


/*
*********************************************************************************************************
*                                             GLOBAL DATA
*********************************************************************************************************
*/


/*
*********************************************************************************************************
                                               CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                              FUNCTIONS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                    Assign ISR handler for R-CAN
*
* Description: This function is used to assign ISR handlers for CAN functionality.
*
* Arguments  : none
*********************************************************************************************************
*/

void  ZC7xxx_CAN_BSP_IntVectSet (void)
{

#if ((CANBUS_RX_HANDLER_EN > 0u) || \
     (CANBUS_TX_HANDLER_EN > 0u) || \
     (CANBUS_NS_HANDLER_EN > 0u))
#if (CAN_MODULE_CHANNEL_0 == DEF_ENABLED)                       /* ------------ GLOBAL ISR HANDLER for CAN0 ----------- */
    UCOS_IntVectSet (ZC7xxx_BSP_CAN0_INT_ID,                    /* CAN0 Interrupt ID.                                   */
                     0u,                                        /* ISR Interrupt Priority. (0 Highest).                 */
                     0u,
                     ZC7xxx_CAN0_ISR_Handler,                   /* Designated ISR Handler.                              */
                     0u);
#endif

#if (CAN_MODULE_CHANNEL_1 == DEF_ENABLED)                       /* ------------ GLOBAL ISR HANDLER for CAN1 ----------- */
    UCOS_IntVectSet (ZC7xxx_BSP_CAN1_INT_ID,                    /* CAN1 Interrupt ID.                                   */
                     0u,                                        /* ISR Interrupt Priority. (0 Highest).                 */
                     0u,
                     ZC7xxx_CAN1_ISR_Handler,                   /* Designated ISR Handler.                              */
                     0u);
#endif
#endif
}


/*
*********************************************************************************************************
*                                       ZC7xxx_CAN_BSP_Start()
*
* Description : Starts the Appropriate CAN Module Channel. This Function will do a CAN Reset using
*               the Subsystem. ZC7xxx_CAN_BSP_Start should be used only with CAN RESET operating mode.
*
* Argument(s) : para_id     Selects the CAN Device [ZC7xxx_CAN_BUS_0, ZC7xxx_CAN_BUS_1].
*
* Return(s)   : Returns error value if improper CAN Device used, if not returns 'DEF_OK'.
*
* Caller(s)   : ZC7xxx_CAN_SetMode().
*               ZC7xxx_CAN_PinSetting().
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  ZC7xxx_CAN_BSP_Start (CPU_INT08U  para_id)
{
#if 0                                                           /* Set to '1' to use Subsystem Reset for CAN Module.    */
    CPU_BOOLEAN  bsp_err;


    bsp_err = DEF_OK;                                           /* Init Var(s).                                         */

                                                                /* ---------------- CAN SUBSYSTEM RESET --------------- */
    ZYNQ_SLCR_REG->SLCR_UNLOCK = ZYNQ_SLCR_UNLOCK_KEY;          /* Set the System Level Control Register Unlock Key     */
    CPU_MB();                                                   /* Constraint Ordering on Mem. Prevent Damage to chip   */

    switch (para_id) {
    #if (CAN_MODULE_CHANNEL_0 == DEF_ENABLED)
        case ZC7xxx_CAN_BUS_0:                                  /* Reset the CAN Module via Subsystem Reset.            */
             ZYNQ_SLCR_REG->CAN_RST_CTRL |=  ZC7xxx_BSP_CAN0_CPU1X_RST;
             ZYNQ_SLCR_REG->CAN_RST_CTRL &= ~ZC7xxx_BSP_CAN0_CPU1X_RST;
             break;
    #endif

    #if (CAN_MODULE_CHANNEL_1 == DEF_ENABLED)
        case ZC7xxx_CAN_BUS_1:
             ZYNQ_SLCR_REG->CAN_RST_CTRL |=  ZC7xxx_BSP_CAN1_CPU1X_RST;
             ZYNQ_SLCR_REG->CAN_RST_CTRL &= ~ZC7xxx_BSP_CAN1_CPU1X_RST;
             break;
    #endif

        default:
             bsp_err = DEF_FAIL;
             break;

    }

    CPU_MB();
    ZYNQ_SLCR_REG->SLCR_LOCK = ZYNQ_SLCR_LOCK_KEY;              /* Lock SLCR register once more.                        */

    return (bsp_err);
#else

    return (DEF_OK);
#endif
}


/*
*********************************************************************************************************
*                                       ZC7xxx_CAN_PinSetting()
*
* Description : This Function provides all the necessary Pin Settings for the required CAN Device.
*
* Argument(s) : para_id     Selects the CAN Device [ZC7xxx_CAN_BUS_0, ZC7xxx_CAN_BUS_1].
*
* Return(s)   : Returns error value if improper CAN Device used, if not returns 'DEF_OK'.
*
* Caller(s)   : ZC7xxx_CAN_Init().
*
* Note(s)     : (1) Refer to the I/O Interface section of the ZYNQ-7000 TRM. [Section 18.5]
*
*               (2) The PLL Clock configuration has been automatically created by Xilinx tools and is
*                   found in the 'zc702.ds' file under the ../../BSP/DS5 folder. The Clock configuration
*                   for both CAN1 & CAN0 have also been automatically created by Xilinx tools under the
*                   "Clock Configuration" section of the .ds file. After following the calculations of
*                   this .ds file, the following Clock #defines were set, as definite points of reference
*                   from a clearly'undefined' location. It is believed based on this clock configuration
*                   that the IO PLL is set to 1 GHz.
*                       The Maximum Clock Frequency for the CAN Module is 24 MHz.
*********************************************************************************************************
*/

CPU_BOOLEAN  ZC7xxx_CAN_PinSetting (CPU_INT08U  para_id)
{
    CPU_BOOLEAN  bsp_err;


    bsp_err = DEF_OK;                                           /* Init Var(s).                                         */

#if 1
                                                                /* ------------- CAN MIO PIN CONFIGURATION ------------ */
    ZYNQ_SLCR_REG->SLCR_UNLOCK = ZYNQ_SLCR_UNLOCK_KEY;          /* Set the System Level Control Register Unlock Key     */
    CPU_MB();                                                   /* Constraint Ordering on Mem. Prevent Damage to chip   */

    switch (para_id) {
    #if (CAN_MODULE_CHANNEL_0 == DEF_ENABLED)
        case ZC7xxx_CAN_BUS_0:
             Can0_Reg_ptr = (ZC7xxx_CAN_REG *)ZC7xxx_CAN0_ADDR; /* Set Base Address of CAN0 Register Set.               */

                                                                /*  - RX PIN CONFIG -                                   */
             ZYNQ_SLCR_REG->MIO_PIN_46 = (ZYNQ_BIT_MIO_DISABLE_RCVR |   /* Disable HSTL Receiver.                       */
                                          ZYNQ_BIT_MIO_PULL_UP      |   /* Enable Pull- Up Resistor on IO Buffer Pin.   */
                                          ZYNQ_BIT_MIO_IO_TYPE(1u)  |   /* Set IO Buffer Type: LVCMOS18.                */
                                          ZYNQ_BIT_MIO_L3_SEL(1u)   |   /* Set L3 Mux Select : CAN0 Rx, Input.          */
                                          ZYNQ_BIT_MIO_TRI_ENABLE);     /* Enable Tri-State  : Active High.             */

                                                                /*  - TX PIN CONFIG -                                   */
             ZYNQ_SLCR_REG->MIO_PIN_47 = (ZYNQ_BIT_MIO_DISABLE_RCVR |   /* Disable HSTL Receiver.                       */
                                          ZYNQ_BIT_MIO_PULL_UP      |   /* Enable Pull- Up Resistor on IO Buffer Pin.   */
                                          ZYNQ_BIT_MIO_IO_TYPE(1u)  |   /* Set IO Buffer Type: LVCMOS18.                */
                                          ZYNQ_BIT_MIO_L3_SEL(1u));     /* Set L3 Mux Select : CAN0 Tx, Output.         */
             break;
    #endif

    #if (CAN_MODULE_CHANNEL_1 == DEF_ENABLED)
        case ZC7xxx_CAN_BUS_1:
             Can1_Reg_ptr = (ZC7xxx_CAN_REG *)ZC7xxx_CAN1_ADDR; /* Set Base Address of CAN1 Register Set.               */

                                                                /*  - RX PIN CONFIG -                                   */
             ZYNQ_SLCR_REG->MIO_PIN_53 = (ZYNQ_BIT_MIO_DISABLE_RCVR |   /* Disable HSTL Receiver.                       */
                                          ZYNQ_BIT_MIO_PULL_UP      |   /* Enable Pull- Up Resistor on IO Buffer Pin.   */
                                          ZYNQ_BIT_MIO_IO_TYPE(1u)  |   /* Set IO Buffer Type: LVCMOS18.                */
                                          ZYNQ_BIT_MIO_L3_SEL(1u)   |   /* Set L3 Mux Select : CAN1 Rx, Input.          */
                                          ZYNQ_BIT_MIO_TRI_ENABLE);     /* Enable Tri-State  : Active High.             */

                                                                /*  - TX PIN CONFIG -                                   */
             ZYNQ_SLCR_REG->MIO_PIN_52 = (ZYNQ_BIT_MIO_DISABLE_RCVR |   /* Disable HSTL Receiver.                       */
                                          ZYNQ_BIT_MIO_PULL_UP      |   /* Enable Pull- Up Resistor on IO Buffer Pin.   */
                                          ZYNQ_BIT_MIO_IO_TYPE(1u)  |   /* Set IO Buffer Type: LVCMOS18.                */
                                          ZYNQ_BIT_MIO_L3_SEL(1u));     /* Set L3 Mux Select : CAN1 Tx, Output.         */
             break;
    #endif

        default:
             bsp_err = DEF_FAIL;
             return (bsp_err);
    }

                                                                /*  - STB B PIN CONFIG -                                */
    ZYNQ_SLCR_REG->MIO_PIN_09 = (ZYNQ_BIT_MIO_DISABLE_RCVR |    /* Disable HSTL Receiver.                               */
                                 ZYNQ_BIT_MIO_IO_TYPE(1u)  |    /* Set IO Buffer Type: LVCMOS18.                        */
                                 ZYNQ_BIT_MIO_L3_SEL(0u));      /* Set L3 Mux Select : GPIO 9 (Bank 0) [Set to Output]  */

    ZC7xxx_BSP_GPIO_DIRM0 |=  ZC7xxx_BSP_CAN_STB_B_GPIO;        /* Set CAN STB Pin (GPIO 9) to Output Direction.        */
    ZC7xxx_BSP_GPIO_OEN0  |=  ZC7xxx_BSP_CAN_STB_B_GPIO;        /* Enable Output for CAN STB Pin (GPIO 9).              */
    ZC7xxx_BSP_GPIO_DATA0 &= ~ZC7xxx_BSP_CAN_STB_B_GPIO;        /* Bring CAN STB Pin 'LOW', to Enable CAN Transceiver.  */

                                                                /* ------------------ CAN CLK CONFIG ------------------ */
    ZYNQ_SLCR_REG->CAN_MIOCLK_CTRL = 0u;                        /* Set CAN1 & CAN0 Reference Clks from Internal PLL.    */

    ZYNQ_SLCR_REG->CAN_CLK_CTRL = (ZC7xxx_BSP_CAN_CLK_DIVISOR1(0x03u) | /* IO PLL Clock Divisor 1. See Note (2).        */
                                   ZC7xxx_BSP_CAN_CLK_DIVISOR0(0x0Eu) | /* IO PLL Clock Divisor 0. See Note (2).        */
                                   ZC7xxx_BSP_CAN_CLK_SRCSEL_IO_PLL   | /* Clock Source : IO Peripheral PLL.            */
                                   ZC7xxx_BSP_CAN_CLK_CLKACT1         | /* Ref Clock    : Enable CAN 1 Clock.           */
                                   ZC7xxx_BSP_CAN_CLK_CLKACT0);         /* Ref Clock    : Enable CAN 0 Clock.           */

    CPU_MB();
    ZYNQ_SLCR_REG->SLCR_LOCK = ZYNQ_SLCR_LOCK_KEY;              /* Lock SLCR register once more.                        */
#endif

    return (bsp_err);
}


/*
*********************************************************************************************************
*                                     ZC7xxx_CAN_CalcTimingReg()
*
* Description : Calculates the Timing Register Values according to the given Baudrate Settings.
*
* Argument(s) : data        Pointer to the Baudrate Settings.
*
* Return(s)   : Returns error value of 'DEF_FAIL' if Timeout occurred, if not returns 'DEF_OK'.
*
* Caller(s)   : ZC7xxx_CAN_Init().
*               ZC7xxx_CAN_IoCtl().
*
* Note(s)     : (1) The PLL Clock configuration has been automatically created by Xilinx tools and is
*                   found in the 'zc702.ds' file under the ../../BSP/DS5 folder. The Clock configuration
*                   for both CAN1 & CAN0 have also been automatically created by Xilinx tools under the
*                   "Clock Configuration" section of the .ds file. After following the calculations of
*                   this .ds file, this clock #define was set, as definite points of reference
*                   from a clearly'undefined' location. It is believed based on this clock configuration
*                   that the IO PLL is set to 1 GHz.
*                       The Maximum Clock Frequency for the CAN Module is 24 MHz.
*
*               (2) Search a whole-numbered QuantaSum. 16 is the most commonly used number in Time Quantas
*                   therefore, begin with 16. Alternate between Increasing and Decreasing the number until
*                   a match is found.
*********************************************************************************************************
*/

CPU_BOOLEAN  ZC7xxx_CAN_CalcTimingReg (ZC7xxx_CAN_BAUD  *data)
{
    CPU_INT32U   time_slices;
    CPU_INT32U   pres_div;
    CPU_INT32U   quanta_tot;
    CPU_INT16S   i;
    CPU_INT32U   can_clk_freq;
    CPU_INT32U   bit_freq;
    CPU_INT16S   sign;
    CPU_BOOLEAN  err;
    CPU_INT08U   div;


    can_clk_freq = ZC7xxx_BSP_CANx_REF_CLK_FREQ;                /* Get Assumed CANx_REF_CLK. Derived from IO PLL.       */
    sign         = -1;                                          /* Initialize Variable(s)                               */
    div          =  0u;
    err          = DEF_FAIL;

    do {
        div++;
        bit_freq    = can_clk_freq / div;                       /* Divide by Try Counter to obtain BUS clk freq in ...  */
                                                                /* ... order to match for higher frequency              */
        time_slices = bit_freq / data->BaudRate;
        if (time_slices < 8u) {
            break;
        }
    } while (((bit_freq / time_slices) != data->BaudRate) ||
              (time_slices             >  25u));

    if ((bit_freq / time_slices) != data->BaudRate) {           /* Test if Baudrate can be Achieved, if not...          */
        err = DEF_FAIL;
    } else {

        i = 0;                                                  /* Calculate factor between CANCLK and Baudrate         */
        while (i < 14) {                                        /* See Note(2)...                                       */
            if (sign < 0) {
                i++;
            }
            sign       = -sign;                                 /* Calculate the total number of Time Quanta            */
            quanta_tot = (CPU_INT32U)(16 + (sign * i));
            pres_div   = time_slices % quanta_tot;
            if (pres_div == 0u) {                               /* Check if a Whole-Number is found                     */

                                                                /* Calculate Prescaler                                  */
                data->PrescalerDiv = (CPU_INT08U)((time_slices * div) / quanta_tot - 1u);
                                                                /* Calculate PhaseBufSeg2 with Selected Percentage...   */
                                                                /* of total Time Quanta                                 */
                data->PhaseBufSeg2 = (CPU_INT08U)(((1000u - data->SamplePoint) * quanta_tot ) / 1000u);

                                                                /* Calculate the TSEG1 Segment Time                     */
                data->PhaseBufSeg1 = (CPU_INT08U)((quanta_tot - data->PhaseBufSeg2) - 1u);

                if (data->PhaseBufSeg1 >= 16u) {                /* TSEG1/2 Registers allow only a Relation of           */
                                                                /* 1:2 but 75% sample point is a Relation of            */
                                                                /* 1:3 therefore move Sample Point in case that         */
                                                                /* maximum number of Sample Points is used              */
                    data->PhaseBufSeg1--;
                    data->PhaseBufSeg2++;
                }
                                                                /* Calculate SJW with Selected Percentage...            */
                                                                /* of Total Time Quanta                                 */
                data->SJW = (CPU_INT08U)((data->ReSynchJumpWidth * quanta_tot) / 1000u);

                if (data->PhaseBufSeg2 > 1u) {
                    data->PhaseBufSeg2 -= 1u;                   /* Register Values are Decremented by 1                 */
                }

                if (data->PhaseBufSeg1 > 1u) {
                    data->PhaseBufSeg1 -= 1u;                   /* Register Values are Decremented by 1                 */
                }

                err = DEF_OK;                                   /* Set Return Value to No Error                         */
                break;
            }
        }
    }

    return (err);                                               /* Return Function Error                                */
}


/*
*********************************************************************************************************
*                                       ZC7xxx_CAN_IntSetting()
*
* Description : This Function ENABLES the Interrupt Handlers for the ZC7xxx CAN Module based on the
*               CAN module. On different MCUs, this function serves to configure all the interrupt settings
*               required. Since this MCU does not require more settings, this is only used to set the
*               Handlers on the Interrupt Vector Table [IVT].
*
* Argument(s) : para_id     Selects the CAN Device [ZC7xxx_CAN_BUS_0, ZC7xxx_CAN_BUS_1].
*
* Return(s)   : none.
*
* Caller(s)   : ZC7xxx_CAN_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if ((CANBUS_RX_HANDLER_EN > 0u) || \
     (CANBUS_TX_HANDLER_EN > 0u) || \
     (CANBUS_NS_HANDLER_EN > 0u))
void  ZC7xxx_CAN_IntSetting (CPU_INT08U  para_id)
{
    switch (para_id) {
        case ZC7xxx_CAN_BUS_0:
            UCOS_IntSrcEn(ZC7xxx_BSP_CAN0_INT_ID);
             break;


        case ZC7xxx_CAN_BUS_1:
            UCOS_IntSrcEn(ZC7xxx_BSP_CAN1_INT_ID);
             break;


        default:
             break;
    }
}
#endif


/*
*********************************************************************************************************
*                                       ZC7xxx_CAN_SetDevIds()
*
* Description : This Function sets the Device IDs for the ISRs.
*
* Argument(s) : dev_id      CAN Device ID.
*
*               dev_name    CAN Device Name.
*
* Return(s)   : none.
*
* Caller(s)   : ZC7xxx_CAN_Open().
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if ((CANBUS_RX_HANDLER_EN > 0u) || \
     (CANBUS_TX_HANDLER_EN > 0u) || \
     (CANBUS_NS_HANDLER_EN > 0u))
void  ZC7xxx_CAN_SetDevIds (CPU_INT08U  dev_id,
                            CPU_INT08U  dev_name)
{
    ZC7xxx_CAN_DevIds[dev_name] = dev_id;
}
#endif


/*
*********************************************************************************************************
*                                      ZC7xxx_CAN0_ISR_Handler()
*
* Description : This holds the Generic CAN Interrupt Handler(s) for CAN0. Based on the ISR pin that is
*               enabled (Either for Tx, Rx, Err, BusOff, etc) the ISR bit will always hit this handler.
*               Thus this handler must deal with any ISR pin that occurs. Only Tx & Rx OK messages signify
*               the Tx/Rx of a new message. Any other ISR flag that get's set must be considered an error
*               in the CAN Controller.
*
* Argument(s) : cpu_id      Function pointer ID from CPU.
*
* Return(s)   : none.
*
* Caller(s)   : ZC7xxx_CAN_BSP_IntVectSet().
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if ((CANBUS_RX_HANDLER_EN > 0u) || \
     (CANBUS_TX_HANDLER_EN > 0u) || \
     (CANBUS_NS_HANDLER_EN > 0u))
#if (CAN_MODULE_CHANNEL_0 == DEF_ENABLED)
void  ZC7xxx_CAN0_ISR_Handler (CPU_INT32U  cpu_id)
{
    if (DEF_BIT_IS_SET(Can0_Reg_ptr->ISR, ZC7xxx_CAN_ISR_TXOK) == DEF_YES) {
        CanBusTxHandler(ZC7xxx_CAN_DevIds[0u]);                 /* Tx Message Transmitted Successfully.                 */
        DEF_BIT_SET(Can0_Reg_ptr->ICR, ZC7xxx_CAN_ICR_CTXOK);   /* Clear Tx OK Interrupt.                               */

    } else if (DEF_BIT_IS_SET(Can0_Reg_ptr->ISR, ZC7xxx_CAN_ISR_RXOK) == DEF_YES) {
        CanBusRxHandler(ZC7xxx_CAN_DevIds[0u]);                 /* New Rx Message was Received & needs to be Read.      */
        DEF_BIT_SET(Can0_Reg_ptr->ICR, ZC7xxx_CAN_ICR_CRXOK);   /* Clear Rx OK Interrupt.                               */

    } else {
        ZC7xxx_CAN_ErrCheck(ZC7xxx_CAN_BUS_0);                  /* Error Checking on CAN 0 Bus.                         */
    }
}
#endif


/*
*********************************************************************************************************
*                                      ZC7xxx_CAN1_ISR_Handler()
*
* Description : This holds the Generic CAN Interrupt Handler(s) for CAN1. Based on the ISR pin that is
*               enabled (Either for Tx, Rx, Err, BusOff, etc) the ISR bit will always hit this handler.
*               Thus this handler must deal with any ISR pin that occurs. Only Tx & Rx OK messages signify
*               the Tx/Rx of a new message. Any other ISR flag that get's set must be considered an error
*               in the CAN Controller.
*
* Argument(s) : cpu_id      Function pointer ID from CPU.
*
* Return(s)   : none.
*
* Caller(s)   : ZC7xxx_CAN_BSP_IntVectSet().
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if (CAN_MODULE_CHANNEL_1 == DEF_ENABLED)
void  ZC7xxx_CAN1_ISR_Handler (CPU_INT32U  cpu_id)
{
    if (DEF_BIT_IS_SET(Can1_Reg_ptr->ISR, ZC7xxx_CAN_ISR_TXOK) == DEF_YES) {
        CanBusTxHandler(ZC7xxx_CAN_DevIds[1u]);                 /* Tx Message Transmitted Successfully.                 */
        DEF_BIT_SET(Can1_Reg_ptr->ICR, ZC7xxx_CAN_ICR_CTXOK);   /* Clear Tx OK Interrupt.                               */

    } else if (DEF_BIT_IS_SET(Can1_Reg_ptr->ISR, ZC7xxx_CAN_ISR_RXOK) == DEF_YES) {
        CanBusRxHandler(ZC7xxx_CAN_DevIds[1u]);                 /* New Rx Message was Received & needs to be Read.      */
        DEF_BIT_SET(Can1_Reg_ptr->ICR, ZC7xxx_CAN_ICR_CRXOK);   /* Clear Rx OK Interrupt.                               */

    } else {
        ZC7xxx_CAN_ErrCheck(ZC7xxx_CAN_BUS_1);                  /* Error Checking on CAN 0 Bus.                         */
    }
}
#endif
#endif
