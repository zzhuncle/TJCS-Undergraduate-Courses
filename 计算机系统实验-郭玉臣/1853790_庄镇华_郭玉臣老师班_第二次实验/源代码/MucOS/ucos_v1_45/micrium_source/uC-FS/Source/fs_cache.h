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
*                                 FILE SYSTEM SUITE CACHE MANAGEMENT
*
* Filename : fs_cache.h
* Version  : V4.07.07
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE GUARDS
*********************************************************************************************************
*/

#ifndef  FS_CACHE_H
#define  FS_CACHE_H


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  "fs_cfg_fs.h"
#include  "fs_err.h"
#include  "fs_type.h"


/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) The following cache-module-present configuration value MUST be pre-#define'd in
*               'fs_cfg_fs.h' PRIOR to all other file system modules that require Cache Module
*               configuration (see 'fs_cfg_fs.h  FILE SYSTEM CONFIGURATION) :
*
*                   FS_CACHE_MODULE_PRESENT
*********************************************************************************************************
*/

#ifdef   FS_CACHE_MODULE_PRESENT                                /* See Note #1.                                         */


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/


#ifdef   FS_CACHE_MODULE
#define  FS_CACHE_EXT
#else
#define  FS_CACHE_EXT  extern
#endif


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
*                                     VOLUME CACHE API DATA TYPE
*********************************************************************************************************
*/

struct  fs_vol_cache_api {
    void  (*Create)    (FS_VOL       *p_vol,                    /* Create cache.                                        */
                        void         *p_cache_data,
                        CPU_INT32U    size,
                        FS_SEC_SIZE   sec_size,
                        CPU_INT08U    pct_mgmt,
                        CPU_INT08U    pct_dir,
                        FS_FLAGS      mode,
                        FS_ERR       *p_err);

    void  (*Del)       (FS_VOL       *p_vol,                    /* Delete cache.                                        */
                        FS_ERR       *p_err);

    void  (*Rd)        (FS_VOL       *p_vol,                    /* Read sectors using cache.                            */
                        void         *p_dest,
                        FS_SEC_NBR    start,
                        FS_SEC_QTY    cnt,
                        FS_FLAGS      sec_type,
                        FS_ERR       *p_err);

    void  (*Release)   (FS_VOL       *p_vol,                    /* Release sectors using cache.                         */
                        FS_SEC_NBR    start,
                        FS_SEC_QTY    cnt,
                        FS_ERR       *p_err);

#if (FS_CFG_RD_ONLY_EN == DEF_DISABLED)
    void  (*Wr)        (FS_VOL       *p_vol,                    /* Write sectors using cache.                           */
                        void         *p_src,
                        FS_SEC_NBR    start,
                        FS_SEC_QTY    cnt,
                        FS_FLAGS      sec_type,
                        FS_ERR       *p_err);
#endif

    void  (*Invalidate)(FS_VOL       *p_vol,                    /* Invalidate cache.                                    */
                        FS_ERR       *p_err);

    void  (*Flush)     (FS_VOL       *p_vol,                    /* Flush cache.                                         */
                        FS_ERR       *p_err);
};


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/

extern  const  FS_VOL_CACHE_API  FSCache_Dflt;


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
*
* Note(s) : (1) See 'MODULE  Note #1'.
*********************************************************************************************************
*/

#endif                                                          /* End of cache module include (see Note #1).           */
#endif
