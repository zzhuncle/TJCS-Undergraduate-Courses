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
*                                     DNS CLIENT TYPE DEFINITION
*
* Filename : dns-c_type.h
* Version  : V2.01.02
*********************************************************************************************************
* Note(s)  : None.
*********************************************************************************************************
*/

#ifndef DNSc_TYPE_PRESENT
#define DNSc_TYPE_PRESENT

#include  <cpu.h>
#include  <lib_def.h>
#include  <lib_mem.h>


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           DNS CFG DATA TYPES
*********************************************************************************************************
*********************************************************************************************************
*/

typedef  struct  DNSc_cfg_task {
    CPU_INT32U  Prio;
    CPU_INT16U  StkSizeBytes;
    CPU_ADDR    StkPtr;
} DNSc_CFG_TASK;


typedef  struct DNSc_cfg {
    MEM_SEG        *MemSegPtr;

    CPU_CHAR       *ServerDfltPtr;
    CPU_INT16U      HostNameLenMax;

    CPU_INT08U      CacheEntriesMaxNbr;

    CPU_INT08U      AddrIPv4MaxPerHost;
    CPU_INT08U      AddrIPv6MaxPerHost;

    CPU_INT08U      TaskDly_ms;
    CPU_INT08U      ReqRetryNbrMax;
    CPU_INT16U      ReqRetryTimeout_ms;
} DNSc_CFG;


#endif

