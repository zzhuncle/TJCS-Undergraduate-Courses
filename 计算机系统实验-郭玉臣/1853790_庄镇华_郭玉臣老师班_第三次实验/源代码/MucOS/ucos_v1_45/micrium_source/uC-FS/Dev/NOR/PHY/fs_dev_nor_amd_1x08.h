/*
*********************************************************************************************************
*                                                uC/FS
*                                      The Embedded File System
*
*                         (c) Copyright 2008-2019; Silicon Laboratories Inc.,
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
*                                      FILE SYSTEM DEVICE DRIVER
*
*                                          NOR FLASH DEVICES
*                              AMD-COMPATIBLE NOR PHYSICAL-LAYER DRIVER
*
* Filename : fs_dev_nor_amd_1x08.h
* Version  : V4.07.07
*********************************************************************************************************
* Note(s)  : (1) Supports CFI NOR flash implementing AMD command set, including :
*
*                (a) Most AMD & Spansion devices.
*                (b) Most ST/Numonyx M29 devices.
*                (c) #### List tested devices.
*
*            (2) Fast programming command "write to buffer & program", supported by many flash
*                implementing AMD command set, is used in this driver if the "Maximum number of bytes
*                in a multi-byte write" (in the CFI device geometry definition) is non-zero.
*
*                (a) Some flash implementing AMD command set have non-zero multi-byte write size but
*                    do NOT support the "write to buffer & program" command.  Often, these devices
*                    will support alternate fast programming methods (e.g., "quadruple byte program"
*                    or "octuple byte program").  This driver MUST be modified for those devices, to
*                    ignore the multi-byte write size in the CFI information (see 'FSDev_NOR_PHY_Open()
*                    Note #2'.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  FS_DEV_NOR_AMD_1X08_PRESENT
#define  FS_DEV_NOR_AMD_1X08_PRESENT


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   FS_DEV_NOR_AMD_1X08_MODULE
#define  FS_DEV_NOR_AMD_1X08_EXT
#else
#define  FS_DEV_NOR_AMD_1X08_EXT  extern
#endif


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  "../../../Source/fs_dev.h"
#include  "../fs_dev_nor.h"


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/

extern  const  FS_DEV_NOR_PHY_API  FSDev_NOR_AMD_1x08;


/*
*********************************************************************************************************
*                                               MACRO'S
*********************************************************************************************************
*/


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

#endif                                                          /* End of NOR AMD module include.                       */
