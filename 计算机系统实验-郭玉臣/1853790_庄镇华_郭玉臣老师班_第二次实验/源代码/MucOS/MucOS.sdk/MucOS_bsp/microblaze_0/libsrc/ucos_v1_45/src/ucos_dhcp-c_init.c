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
*                                         uC/DHCP-C INIT CODE
*
* File : ucos_dhcp-c_init.c
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <app_cfg.h>
#include <cpu.h>
#include <ucos_bsp.h>
#include <ucos_int.h>
#include <xparameters.h>
#include <KAL/kal.h>

#if (APP_TCPIP_ENABLED == DEF_ENABLED)
#include  <IP/IPv4/net_ipv4.h>
#include  <Source/net_type.h>
#include  <Source/net_ip.h>

#if (APP_DHCPC_ENABLED == DEF_ENABLED)
#include  <Source/dhcp-c.h>
#endif


#if (APP_DHCPC_ENABLED == DEF_ENABLED)
CPU_BOOLEAN UCOS_DHCPc_Init (void)
{
    DHCPc_ERR err_dhcp;


#if (UCOS_START_DEBUG_TRACE == DEF_ENABLED)
    UCOS_Print("UCOS - Initializing uC/DHCPc.\r\n");
#endif

    err_dhcp = DHCPc_Init();

    if (err_dhcp != DHCPc_ERR_NONE) {
#if (UCOS_START_DEBUG_TRACE == DEF_ENABLED)
        UCOS_Printf("UCOS - Error initializing uC/DHCPc. Error code %d\r\n", err_dhcp);
#endif
        return (DEF_FAIL);
    }

    return (DEF_OK);
}


CPU_BOOLEAN UCOS_DHCPc_IF_Init (CPU_INT32U if_nbr)
{
    DHCPc_STATUS      status;
    CPU_BOOLEAN       done;
    NET_IPv4_ADDR    *p_addr_dns = DEF_NULL;
    DHCPc_OPT_CODE    req_param[DHCPc_CFG_PARAM_REQ_TBL_SIZE];
    CPU_INT08U        req_param_qty;
    CPU_INT16U        size;
    DHCPc_ERR         err_dhcp;


    UCOS_Printf("UCOS - Trying dhcp configuration for interface %d\r\n", if_nbr);

    req_param[0]   = DHCP_OPT_DOMAIN_NAME_SERVER;
    req_param_qty  = 1u;

    DHCPc_Start(if_nbr, req_param, req_param_qty, &err_dhcp);
    if (err_dhcp != DHCPc_ERR_NONE) {
        UCOS_Printf("UCOS - Error starting dhcp for interface %d, error code %d\r\n", if_nbr, err_dhcp);
        return (DEF_FAIL);
    }

    done = DEF_NO;
    while (done == DEF_NO) {
        KAL_Dly(100);

        status = DHCPc_ChkStatus(if_nbr, &err_dhcp);
        switch (status) {
            case DHCP_STATUS_CFG_IN_PROGRESS:
                 break;


            case DHCP_STATUS_CFGD:
            case DHCP_STATUS_CFGD_NO_TMR:
            case DHCP_STATUS_CFGD_LOCAL_LINK:

                 if ((status == DHCP_STATUS_CFGD) ||
                     (status == DHCP_STATUS_CFGD_NO_TMR)) {
                     UCOS_Print("UCOS - DHCP configuration successful.\r\n");
                 } else {
                     UCOS_Print("UCOS - DHCP Link-Local configuration successful.\r\n");
                 }

                 size = sizeof(NET_IPv4_ADDR);
                 DHCPc_GetOptVal(             if_nbr,
                                              DHCP_OPT_DOMAIN_NAME_SERVER,
                                (CPU_INT08U *)p_addr_dns,
                                             &size,
                                             &err_dhcp);


                 done = DEF_YES;
                 break;


            case DHCP_STATUS_FAIL:
                 DHCPc_Stop(if_nbr, &err_dhcp);
                 return (DEF_FAIL);


            default:
                 break;
        }
    }

    return (DEF_OK);
}
#endif /* #if (APP_TCPIP_ENABLED == DEF_ENABLED) */
#endif /* #if (APP_DHCPC_ENABLED == DEF_ENABLED) */
