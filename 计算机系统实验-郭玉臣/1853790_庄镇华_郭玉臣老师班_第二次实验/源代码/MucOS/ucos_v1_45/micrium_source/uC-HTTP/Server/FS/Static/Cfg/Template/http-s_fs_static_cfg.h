/*
*********************************************************************************************************
*                                               uC/HTTP
*                                     Hypertext Transfer Protocol
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
*                                     NET FS STATIC CONFIGURATION
*
* Filename : http-s_fs_static_cfg.h
* Version  : V3.00.08
*********************************************************************************************************
*/

#ifndef HTTPs_FS_STATIC_CFG_MODULE_PRESENT
#define HTTPs_FS_STATIC_CFG_MODULE_PRESENT


/*
*********************************************************************************************************
*                                             NET FS CFG
*********************************************************************************************************
*/

                                                                /* Configure external argument check feature ...        */
#define  HTTPs_FS_CFG_ARG_CHK_EXT_EN                     DEF_ENABLED
                                                                /*   DEF_DISABLED     External argument check DISABLED  */
                                                                /*   DEF_ENABLED      External argument check ENABLED   */


#define  HTTPs_FS_CFG_MAX_FILE_NAME_LEN                  256    /* Configure maximum file name length.                  */
#define  HTTPs_FS_CFG_NBR_FILES                           15    /* Configure number of files.                           */
#define  HTTPs_FS_CFG_NBR_DIRS                             1    /* Configure number of directories.                     */


#endif
