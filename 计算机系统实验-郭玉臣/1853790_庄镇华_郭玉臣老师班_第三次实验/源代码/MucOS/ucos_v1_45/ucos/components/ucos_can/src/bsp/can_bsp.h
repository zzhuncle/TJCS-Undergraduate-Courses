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
* File : can_bsp.h
*********************************************************************************************************
*/

#ifndef  _DRV_CAN_BSP_H_
#define  _DRV_CAN_BSP_H_


/*
*********************************************************************************************************
*                                              INCLUDES
*********************************************************************************************************
*/

#include  "cpu.h"
#include  "drv_can_reg.h"
#include  <ucos_bsp.h>
#include  <ucos_int.h>


/*
*********************************************************************************************************
*                                               DEFINES
*
* Note(s) : (1) The PLL Clock configuration has been automatically created by Xilinx tools and is
*               found in the 'zc702.ds' file under the ../../BSP/DS5 folder. The Clock configuration
*               for both CAN1 & CAN0 have also been automatically created by Xilinx tools under the
*               "Clock Configuration" section of the .ds file. After following the calculations of
*               this .ds file, the following Clock #defines were set, as definite points of reference
*               from a clearly'undefined' location.
*
*               The Maximum Clock Frequency for the CAN Module is 24 MHz.
*********************************************************************************************************
*/
                                                                /* ------------- PERIPHERAL INTERRUPT IDs ------------- */
#define  ZC7xxx_BSP_CAN0_INT_ID                         60u
#define  ZC7xxx_BSP_CAN1_INT_ID                         83u

                                                                /* -------------- MIO PIN CONFIG DEFINES -------------- */
#define  ZC7xxx_BSP_CAN0_RX_PIN                         46u
#define  ZC7xxx_BSP_CAN0_TX_PIN                         47u

#define  ZC7xxx_BSP_CAN1_RX_PIN                         53u
#define  ZC7xxx_BSP_CAN1_TX_PIN                         52u

                                                                /* -------------- SLCR CAN RESET DEFINES -------------- */
#define  ZC7xxx_BSP_CAN0_CPU1X_RST              DEF_BIT_00
#define  ZC7xxx_BSP_CAN1_CPU1X_RST              DEF_BIT_01


                                                                /* ---------------- CLOCK CONFIGURATION --------------- */
#define  ZC7xxx_BSP_CANx_REF_CLK_FREQ            (24000000uL)   /* See Note (1).                                        */
#define  ZC7xxx_BSP_IO_PLL_FREQ                (1000000000uL)


/*
*********************************************************************************************************
*                                        REGISTER DEFINITIONS
*********************************************************************************************************
*/
                                                                /* ----------------- SLCR CAN CLK CTRL ---------------- */
#define  ZC7xxx_BSP_CAN_CLK_SRCSEL_IO_PLL      (0u << 4u)
#define  ZC7xxx_BSP_CAN_CLK_SRCSEL_ARM_PLL     (2u << 4u)
#define  ZC7xxx_BSP_CAN_CLK_SRCSEL_DDR_PLL     (3u << 4u)

#define  ZC7xxx_BSP_CAN_CLK_CLKACT1             DEF_BIT_01
#define  ZC7xxx_BSP_CAN_CLK_CLKACT0             DEF_BIT_00

                                                                /* --------------- SLCR CAN MIOCLK CTRL --------------- */
#define  ZC7xxx_BSP_CAN_MIOCLK_CAN1_REF_SEL     DEF_BIT_22
#define  ZC7xxx_BSP_CAN_MIOCLK_CAN0_REF_SEL     DEF_BIT_06

                                                                /* ------------------ CAN STB B GPIO ------------------ */
#define  ZC7xxx_BSP_CAN_STB_B_GPIO              DEF_BIT_09


/*
*********************************************************************************************************
*                                              REGISTERS
*********************************************************************************************************
*/
                                                                /* ---------------- CAN STB B GPIO REGs --------------- */
#define  ZC7xxx_BSP_GPIO_BASE_ADDR              0xE000A000u

#define  ZC7xxx_BSP_GPIO_DATA0                 (*((CPU_REG32 *)(ZC7xxx_BSP_GPIO_BASE_ADDR + 0x040u)))
#define  ZC7xxx_BSP_GPIO_DIRM0                 (*((CPU_REG32 *)(ZC7xxx_BSP_GPIO_BASE_ADDR + 0x204u)))
#define  ZC7xxx_BSP_GPIO_OEN0                  (*((CPU_REG32 *)(ZC7xxx_BSP_GPIO_BASE_ADDR + 0x208u)))


/*
*********************************************************************************************************
*                                               MACROS
*********************************************************************************************************
*/
                                                                /* ----------------- SLCR CAN CLK CTRL ---------------- */
#define  ZC7xxx_BSP_CAN_CLK_DIVISOR1(x)        (((x) & 0x3Fu) << 20u)

#define  ZC7xxx_BSP_CAN_CLK_DIVISOR0(x)        (((x) & 0x3Fu) <<  8u)


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

CPU_BOOLEAN  ZC7xxx_CAN_BSP_Start     (CPU_INT08U        para_id);

CPU_BOOLEAN  ZC7xxx_CAN_PinSetting    (CPU_INT08U        para_id);

CPU_BOOLEAN  ZC7xxx_CAN_CalcTimingReg (ZC7xxx_CAN_BAUD  *data);


#if ((CANBUS_RX_HANDLER_EN > 0) || \
     (CANBUS_TX_HANDLER_EN > 0) || \
     (CANBUS_NS_HANDLER_EN > 0))
void         ZC7xxx_CAN_BSP_IntVectSet(void);

void         ZC7xxx_CAN_IntSetting    (CPU_INT08U        para_id);

void         ZC7xxx_CAN_SetDevIds     (CPU_INT08U        dev_id,
                                       CPU_INT08U        dev_name);

#if (CAN_MODULE_CHANNEL_0 == DEF_ENABLED)
void         ZC7xxx_CAN0_ISR_Handler  (CPU_INT32U        cpu_id);
#endif

#if (CAN_MODULE_CHANNEL_1 == DEF_ENABLED)
void         ZC7xxx_CAN1_ISR_Handler  (CPU_INT32U        cpu_id);
#endif
#endif


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif
