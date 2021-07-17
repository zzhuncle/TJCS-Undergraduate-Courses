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
*                                         FILE SYSTEM DEFINES
*
* Filename : fs_def.h
* Version  : V4.07.07
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  FS_DEF_H
#define  FS_DEF_H


/*
*********************************************************************************************************
*                                         FILE SYSTEM DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            BUILD DEFINES
*********************************************************************************************************
*/

#define  FS_BUILD_FULL                                     1u
#define  FS_BUILD_DEV_ONLY                                 2u


/*
*********************************************************************************************************
*                                        SYSTEM DRIVER DEFINES
*********************************************************************************************************
*/

#define  FS_SYS_DRV_SEL_FAT                                1u


/*
*********************************************************************************************************
*                                          TIME DEFINES
*********************************************************************************************************
*/

#define  FS_TIME_FMT              CLK_STR_FMT_DAY_MONTH_DD_HH_MM_SS_YYYY
#define  FS_TIME_STR_MIN_LEN      CLK_STR_FMT_DAY_MONTH_DD_HH_MM_SS_YYYY_LEN

#define  FS_TIME_TS_INVALID       ((CLK_TS_SEC)-1)


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of fs_def.h include.                         */
