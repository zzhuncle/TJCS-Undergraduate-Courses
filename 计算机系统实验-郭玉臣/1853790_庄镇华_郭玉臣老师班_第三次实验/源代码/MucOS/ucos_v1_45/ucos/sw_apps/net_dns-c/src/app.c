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
*                                          SETUP INSTRUCTIONS
*
*   This demonstration project illustrate a basic uC/DNSc project.
*
*   By default some configuration steps are required to compile this example :
*
*   1. Include the require Micrium software components
*       In the BSP setting dialog in the "overview" section of the left pane the following libraries
*       should be added to the BSP :
*
*           ucos_common
*           ucos_osiii
*           ucos_standalone
*           ucos_tcpip
*           ucos_dhcp
*           ucos_dns-c
*
*   2. Kernel tick source - (Not required on the Zynq-7000 PS)
*       If a suitable timer is available in your FPGA design it can be used as the kernel tick source.
*       To do so, in the "ucos" section select a timer for the "kernel_tick_src" configuration option.
*
*   3. STDOUT configuration
*       Output from the print() and UCOS_Print() functions can be redirected to a supported UART. In
*       the "ucos_standalone" section the stdout configuration will list the available UARTs.
*
*   4. Network interface configuration
*       In the BSP network interface section selects the desired network intergace and ip configuration.
*       Leave dhcp enabled if you'd like the embbeded dhcp client to configure the interface.
*
*   Troubleshooting :
*       By default the Xilinx SDK may not have selected the Micrium drivers for the timer and UART.
*       If that is the case they must be manually selected in the drivers configuration section.
*
*       Finally make sure the FPGA is programmed before debugging.
*
*
*   Remember that this example is provided for evaluation purposes only. Commercial development requires
*   a valid license from Micrium.
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <stdio.h>
#include  <Source/os.h>
#include  <ucos_bsp.h>

#include  <Source/dns-c.h>
#include  <Source/net_util.h>
#include  <Source/net_ascii.h>
#include  <Source/net_sock.h>


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void  MainTask (void *p_arg);


/*
*********************************************************************************************************
*                                               main()
*
* Description : Entry point for C code.
*
*********************************************************************************************************
*/

int main()
{

    UCOSStartup(MainTask);

    return 0;
}


/*
*********************************************************************************************************
*                                             MainTask()
*
* Description : Startup task example code.
*
* Returns     : none.
*
* Created by  : main().
*********************************************************************************************************
*/

void  MainTask (void *p_arg)
{
    OS_ERR         os_err;
    DNSc_ADDR_OBJ  addrs[2u];
    CPU_INT08U     addr_nbr = 2u;
    DNSc_ERR       dns_err;
    NET_ERR        net_err;
    CPU_CHAR       p_addr_str[32];

    UCOS_Print("Application start\r\n");

    UCOS_Print("Delaying until the link is up.\r\n");
    NetIF_LinkStateWaitUntilUp (1, 1000, 500, &net_err);
    OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_HMSM_STRICT, &os_err);

    DNSc_GetServerByStr(p_addr_str, 32, &dns_err); /* Get current DNS server address. */

    UCOS_Printf("Resolving URL \"micrium.com\" using DNS server %s\r\n", p_addr_str);
    DNSc_GetHost("micrium.com", DEF_NULL, 0, addrs, &addr_nbr, DNSc_FLAG_NONE, DEF_NULL, &dns_err);
    if (dns_err != DNSc_ERR_NONE) {
        UCOS_Printf("Error performing DNS query. DNSc_GetHost() returned error code %d\r\n", dns_err);
    } else {
		if (addrs[0].Len == NET_IPv4_ADDR_LEN) {
#ifdef  NET_IPv4_MODULE_EN
			NET_IPv4_ADDR *p_addr = (NET_IPv4_ADDR *)addrs[0].Addr;
			NetASCII_IPv4_to_Str(*p_addr, p_addr_str, NET_ASCII_LEN_MAX_ADDR_IP, &net_err);
#endif
		} else {
#ifdef  NET_IPv6_MODULE_EN
			NET_IPv6_ADDR *p_addr = (NET_IPv6_ADDR *)addrs[0].Addr;
			NetASCII_IPv6_to_Str(p_addr, p_addr_str, DEF_NO, DEF_YES, &net_err);
#endif
		}
	    UCOS_Printf("Resolved IP: %s\r\n", p_addr_str);
    }

    while (DEF_TRUE) {
        OSTimeDlyHMSM(0, 0, 10, 0, OS_OPT_TIME_HMSM_STRICT, &os_err);
        UCOS_Print("Periodic output every 10 seconds from the main task\r\n");
    }

}

