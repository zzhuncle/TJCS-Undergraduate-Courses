/*
*********************************************************************************************************
*                                               uC/DNSc
*                                     Domain Name Server (client)
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
*                                       DNS CLIENT CACHE MODULE
*
* Filename : dns-c_cache.h
* Version  : V2.01.02
*********************************************************************************************************
* Note(s)  : (1) Assumes the following versions (or more recent) of software modules are included
*                in the project build :
*
*                (a) uC/LIB    V1.37.00
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               pre-processor macro definition.
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  DNSc_CACHE_PRESENT                                     /* See Note #1.                                         */
#define  DNSc_CACHE_PRESENT


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#include  "dns-c.h"


/*
*********************************************************************************************************
*********************************************************************************************************
*                                              DATA TYPES
*********************************************************************************************************
*********************************************************************************************************
*/

typedef  struct  DNSc_cache_item  DNSc_CACHE_ITEM;

struct DNSc_cache_item {
    DNSc_HOST_OBJ    *HostPtr;
    DNSc_CACHE_ITEM  *NextPtr;
};


/*
*********************************************************************************************************
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

void            DNScCache_Init          (const  DNSc_CFG        *p_cfg,
                                                DNSc_ERR        *p_err);

void            DNScCache_Clr           (       DNSc_ERR        *p_err);

void            DNScCache_HostInsert    (       DNSc_HOST_OBJ   *p_host,
                                                DNSc_ERR        *p_err);

void            DNScCache_HostRemove    (       DNSc_HOST_OBJ   *p_host);

void            DNScCache_HostSrchRemove(const  CPU_CHAR        *p_host_name,
                                                DNSc_ERR        *p_err);

DNSc_STATUS     DNScCache_Srch          (const  CPU_CHAR        *p_host_name,
                                                DNSc_HOST_OBJ  **p_host_obj,
                                                DNSc_ADDR_OBJ   *p_addrs,
                                                CPU_INT08U       addr_nbr_max,
                                                CPU_INT08U      *p_addr_nbr_rtn,
                                                DNSc_FLAGS       flags,
                                                DNSc_ERR        *p_err);

DNSc_HOST_OBJ  *DNScCache_HostObjGet    (const  CPU_CHAR        *p_host_name,
                                                DNSc_FLAGS       flags,
                                                DNSc_REQ_CFG    *p_cfg,
                                                DNSc_ERR        *p_err);

void            DNScCache_HostObjFree   (       DNSc_HOST_OBJ   *p_host);

void            DNScCache_HostAddrInsert(const  DNSc_CFG        *p_cfg,
                                                DNSc_HOST_OBJ   *p_host,
                                                DNSc_ADDR_OBJ   *p_addr,
                                                CPU_BOOLEAN      is_reverse,
                                                DNSc_ERR        *p_err);

DNSc_ADDR_OBJ  *DNScCache_AddrObjGet    (       DNSc_ERR        *p_err);

void            DNScCache_AddrObjFree   (       DNSc_ADDR_OBJ   *p_addr);

void            DNScCache_AddrObjSet    (       DNSc_ADDR_OBJ   *p_addr,
                                                CPU_CHAR        *p_str_addr,
                                                DNSc_ERR        *p_err);

DNSc_STATUS     DNScCache_ResolveHost   (const  DNSc_CFG        *p_cfg,
                                                DNSc_HOST_OBJ   *p_host,
                                                DNSc_ERR        *p_err);

CPU_INT16U      DNScCache_ResolveAll    (const  DNSc_CFG        *p_cfg,
                                                DNSc_ERR        *p_err);

#endif /* DNSc_CACHE_PRESENT */
