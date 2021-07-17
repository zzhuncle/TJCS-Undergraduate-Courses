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
*                                        uC/TCP-IP INIT CODE
*
* File : ucos_tcpip.init.c
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
#include  <Source/net.h>
#include  <Source/net_ascii.h>
#include  <Source/net_sock.h>
#include  <IF/net_if.h>
#include  <IF/net_if_ether.h>

#include  <IP/IPv4/net_ipv4.h>
#include  <IP/IPv6/net_ipv6.h>

#if (APP_DHCPC_ENABLED == DEF_ENABLED)
#include  <Source/dhcp-c.h>
#endif

CPU_BOOLEAN UCOS_TCPIP_InitGEM (void);
CPU_BOOLEAN UCOS_TCPIP_InitAXIEthLite (void);
CPU_BOOLEAN UCOS_TCPIP_InitAXIEth (void);
CPU_BOOLEAN UCOS_TCPIP_IF_Cfg (CPU_INT32U if_nbr);
#endif /* #if (APP_TCPIP_ENABLED == DEF_ENABLED) */


#if (APP_TCPIP_ENABLED == DEF_ENABLED)
#if (UCOS_ETHERNET_DRIVER == UCOS_ETHERNET_EMACPS)
#if (UCOS_CPU_TYPE == UCOS_CPU_TYPE_PS7)
#include <net_zynq_gem_bsp.h>
#else
#include <net_ultrascale_gem_bsp.h>
#endif

#include <Dev/Ether/GEM/net_dev_gem.h>
#if (UCOS_CPU_TYPE == UCOS_CPU_TYPE_PS7)
#include <xil_mmu.h>
#endif
#if (UCOS_CPU_TYPE == UCOS_CPU_TYPE_PSUR5)
#include <xreg_cortexr5.h>
#include <xil_mpu.h>
#endif
#if (UCOS_CPU_TYPE == UCOS_CPU_TYPE_PSUA53)
#include <xil_mmu.h>
#endif
#endif

#if (UCOS_ETHERNET_DRIVER == UCOS_ETHERNET_AXIETHLITE)
#include <ucos_axiethernetlite.h>
#include <net_xil_ether_lite_bsp.h>
#include <net_xil_ether_lite_dev_cfg.h>
#include <Dev/Ether/XIL_ETHER_LITE/net_dev_xil_ether_lite.h>
#endif

#if (UCOS_ETHERNET_DRIVER == UCOS_ETHERNET_AXIETH)
#include <ucos_axiethernet.h>
#include <net_xil_ether_bsp.h>
#include <Dev/Ether/XIL_ETHER/net_dev_xil_ether.h>
#endif

#include <Dev/Ether/PHY/88E1111/net_phy_88e1111.h>
#include <Dev/Ether/PHY/DP83867IR/net_phy_dp83867ir.h>
#endif

#if (APP_TCPIP_ENABLED == DEF_ENABLED)
NET_DEV_CFG_ETHER NetDevCfg;
NET_PHY_CFG_ETHER NetPhyCfg;
#endif


CPU_BOOLEAN UCOS_TCPIP_Init (void)
{
#if (APP_TCPIP_ENABLED == DEF_ENABLED)
    NET_ERR         err_net;
#endif /* #if (APP_TCPIP_ENABLED == DEF_ENABLED) */
    CPU_BOOLEAN     res = DEF_OK;

#if (APP_TCPIP_ENABLED == DEF_ENABLED)

#if (UCOS_START_DEBUG_TRACE == DEF_ENABLED)
    UCOS_Print("UCOS - Initializing uC/TCP-IP.\r\n");
#endif

#if (APP_TCPIP_EXP_ENABLED == DEF_DISABLED)
    err_net = Net_Init(&NetRxTaskCfg,
                       &NetTxDeallocTaskCfg,
                       &NetTmrTaskCfg);
#else
    err_net = Net_Init(&NetTaskCfg,
                        DEF_NULL,
                       &err_net);
#endif
    if (err_net != NET_ERR_NONE) {
        UCOS_Printf("UCOS - Error initializing uC/TCP-IP. Error code %d\r\n", err_net);
        return (DEF_FAIL);
    }

#if (APP_DHCPC_ENABLED == DEF_ENABLED)
    UCOS_DHCPc_Init();
#endif

#if (APP_DNSC_ENABLED == DEF_ENABLED)
    UCOS_DNSc_Init();
#endif

#if (APP_HTTPC_ENABLED == DEF_ENABLED)
    UCOS_HTTPc_Init();
#endif

#if (APP_MQTTC_ENABLED == DEF_ENABLED)
    UCOS_MQTTc_Init();
#endif

#if (APP_TELNETS_ENABLED == DEF_ENABLED)
    UCOS_TELNETs_Init();
#endif

#if (APP_IPERF_ENABLED == DEF_ENABLED)
    UCOS_IPerf_Init();
#endif

#if (UCOS_ETHERNET_DRIVER == UCOS_ETHERNET_EMACPS)
    res = UCOS_TCPIP_InitGEM();
#elif (UCOS_ETHERNET_DRIVER == UCOS_ETHERNET_AXIETHLITE)
    res = UCOS_TCPIP_InitAXIEthLite();
#else
    UCOS_Print("UCOS - No Interface to initialize.\r\n");
#endif

#endif /* #if (APP_TCPIP_ENABLED == DEF_ENABLED) */

    return (res);
}


#if (APP_TCPIP_ENABLED == DEF_ENABLED)
CPU_BOOLEAN UCOS_TCPIP_IF_Cfg (CPU_INT32U if_nbr)
{
    NET_ERR         err_net;
#ifdef NET_IPv4_MODULE_EN
    NET_IPv4_ADDR   addr_ipv4;
    NET_IPv4_ADDR   msk_ipv4;
    NET_IPv4_ADDR   gateway_ipv4;
#endif
    NET_IPv6_ADDR      addr_ip;
    NET_IP_ADDR_FAMILY addr_family;
    CPU_BOOLEAN        cfg_success;
    NET_IPv4_ADDR      addr_tbl[NET_IPv4_CFG_IF_MAX_NBR_ADDR];
    NET_IP_ADDRS_QTY   addr_ip_tbl_qty;
    CPU_CHAR           ip_str[20];


#if ((APP_DHCPC_ENABLED == DEF_ENABLED) && (UCOS_ETHERNET_DHCP == DEF_ENABLED))
    cfg_success = UCOS_DHCPc_IF_Init(if_nbr);
    if (cfg_success == DEF_OK) {
        goto cfg_done;
    }
#endif /* #if ((APP_DHCPC_ENABLED == DEF_ENABLED) && (UCOS_ETHERNET_DHCP == DEF_ENABLED)) */

    addr_family = NetASCII_Str_to_IP(UCOS_ETHERNET_ADDRESS,
                                     &addr_ip,
                                     NET_IPv6_ADDR_SIZE,
                                     &err_net);
    if (err_net != NET_ASCII_ERR_NONE) {
        if ((err_net == NET_ASCII_ERR_IP_FAMILY_NOT_PRESENT) && (addr_family == NET_IP_ADDR_FAMILY_IPv6)) {
            UCOS_Printf("UCOS - Error configuring IP address. IPv6 address specified but IPv6 support disabled. Check ucos_tcpip configuration.\r\n", err_net);
        } else {
            UCOS_Printf("UCOS - Error configuring IP address. Error code %d.\r\n", err_net);
        }
        return (DEF_FAIL);
    }

    if (addr_family == NET_IP_ADDR_FAMILY_IPv4) {
        NetASCII_Str_to_IP(UCOS_ETHERNET_ADDRESS,
                           &addr_ipv4,
                           NET_IPv4_ADDR_SIZE,
                           &err_net);

        NetASCII_Str_to_IP(UCOS_ETHERNET_SUBMASK,
                                         &msk_ipv4,
                                         NET_IPv4_ADDR_SIZE,
                                         &err_net);

        if (err_net != NET_ASCII_ERR_NONE) {
            UCOS_Printf("UCOS - Error configuring subnet mask. Error code %d.\r\n", err_net);
            return (DEF_FAIL);
        }

        NetASCII_Str_to_IP(UCOS_ETHERNET_GATEWAY,
                           &gateway_ipv4,
                           NET_IPv4_ADDR_SIZE,
                          &err_net);

        if (err_net != NET_ASCII_ERR_NONE) {
            UCOS_Printf("UCOS - Error configuring default gateway. Error code %d.\r\n", err_net);
            return (DEF_FAIL);
        }

        NetIPv4_CfgAddrAdd(if_nbr,
                           addr_ipv4,
                           msk_ipv4,
                           gateway_ipv4,
                          &err_net);
        if (err_net != NET_IPv4_ERR_NONE) {
            UCOS_Printf("UCOS - Error configuring gem interface. Error code %d.\r\n", err_net);
            return (DEF_FAIL);
        }
    } else {
#ifdef NET_IPv6_MODULE_EN
        NetASCII_Str_to_IP(UCOS_ETHERNET_ADDRESS,
                           &addr_ip,
                            NET_IPv6_ADDR_SIZE,
                           &err_net);

        NetIPv6_CfgAddrAdd(if_nbr,
                          &addr_ip,
                           64u,
                           0u,
                          &err_net);
        if (err_net != NET_IPv6_ERR_NONE) {
            UCOS_Printf("UCOS - Error configuring gem interface. Error code %d.\r\n", err_net);
            return (DEF_FAIL);
        }
#endif
    }


cfg_done:
    addr_ip_tbl_qty = sizeof(addr_tbl) / sizeof(NET_IPv4_ADDR);
    (void)NetIPv4_GetAddrHost( if_nbr,
                              &addr_tbl[0],
                              &addr_ip_tbl_qty,
                              &err_net);
    if (err_net != NET_IPv4_ERR_NONE) {
        return (DEF_FAIL);
    }

    UCOS_Printf("UCOS - Interface %d configured.\r\n", if_nbr);

    NetASCII_IPv4_to_Str( addr_tbl[0],
                          ip_str,
                          DEF_NO,
                         &err_net);
    UCOS_Printf("UCOS - IP: %s\r\n", ip_str);

    gateway_ipv4 = NetIPv4_GetAddrDfltGateway(addr_tbl[0], &err_net);
    NetASCII_IPv4_to_Str( gateway_ipv4,
                          ip_str,
                          DEF_NO,
                         &err_net);
    UCOS_Printf("UCOS - Gateway: %s\r\n", ip_str);

    msk_ipv4 = NetIPv4_GetAddrSubnetMask(addr_tbl[0], &err_net);
    NetASCII_IPv4_to_Str( msk_ipv4,
                          ip_str,
                          DEF_NO,
                         &err_net);
    UCOS_Printf("UCOS - Subnet: %s\r\n", ip_str);

    return (DEF_OK);
}
#endif

#if (APP_TCPIP_ENABLED == DEF_ENABLED)
CPU_BOOLEAN UCOS_TCPIP_IfCfgInit (NET_DEV_CFG_ETHER *Cfg)
{
    CPU_INT32U rx_desc_nbr;
    CPU_INT32U tx_desc_nbr;

    Cfg->RxBufPoolType = NET_IF_MEM_TYPE_MAIN;
    Cfg->RxBufLargeSize = 1536;
    Cfg->RxBufLargeNbr = UCOS_IF_RX_BUF_NBR;
    Cfg->RxBufAlignOctets = 32;
    Cfg->RxBufIxOffset = 0;
    Cfg->TxBufPoolType = NET_IF_MEM_TYPE_MAIN;
    Cfg->TxBufLargeSize = 1632;
    Cfg->TxBufLargeNbr = UCOS_IF_TX_LARGE_BUF_NBR;
    Cfg->TxBufSmallSize = 64u;
    Cfg->TxBufSmallNbr = UCOS_IF_TX_SMALL_BUF_NBR;
    Cfg->TxBufAlignOctets = 32;
    Cfg->TxBufIxOffset = 0;
    Cfg->Flags = 0;

#if (UCOS_IF_RX_DESC_NBR != 0)
    Cfg->RxDescNbr = UCOS_IF_RX_DESC_NBR;
#else
    rx_desc_nbr = UCOS_IF_RX_BUF_NBR / 2;

    if (rx_desc_nbr < 2) {
        rx_desc_nbr = 2;
    }

    if (rx_desc_nbr > 64) {
        rx_desc_nbr = 64;
    }

    Cfg->RxDescNbr = rx_desc_nbr;
#endif

#if (UCOS_IF_TX_DESC_NBR != 0)
    Cfg->TxDescNbr = UCOS_IF_TX_DESC_NBR;
#else
    tx_desc_nbr = UCOS_IF_TX_LARGE_BUF_NBR / 2;

    if (tx_desc_nbr < 2) {
        tx_desc_nbr = 2;
    }

    if (tx_desc_nbr > 32) {
        tx_desc_nbr = 32;
    }

    Cfg->TxDescNbr = tx_desc_nbr;
#endif

#if (UCOS_IF_DEDIC_MEM_ADDR != 0)
    Cfg->MemAddr = UCOS_IF_DEDIC_MEM_ADDR;
    Cfg->MemSize = UCOS_IF_DEDIC_MEM_SIZE;
#else
    Cfg->MemAddr = 0u;
    Cfg->MemSize = 0u;
#endif

    Cfg->BaseAddr = 0u;
    Cfg->DataBusSizeNbrBits = 0;

    Str_Copy_N(Cfg->HW_AddrStr, UCOS_IF_HW_ADDR, 17);

    return (DEF_OK);
}
#endif /* #if (APP_TCPIP_ENABLED == DEF_ENABLED) */


#if (APP_TCPIP_ENABLED == DEF_ENABLED)
CPU_BOOLEAN UCOS_TCPIP_PhyCfgInit (NET_PHY_CFG_ETHER *Cfg)
{
    if (UCOS_PHY_BUS_ADDR == 255)
        Cfg->BusAddr = NET_PHY_ADDR_AUTO;
    else
        Cfg->BusAddr = UCOS_PHY_BUS_ADDR;

    switch (UCOS_PHY_BUS_MODE) {
        case UCOS_NET_PHY_BUS_MODE_MII:
            Cfg->BusMode = NET_PHY_BUS_MODE_MII;
            break;

        case UCOS_NET_PHY_BUS_MODE_RMII:
            Cfg->BusMode = NET_PHY_BUS_MODE_RMII;
            break;

        case UCOS_NET_PHY_BUS_MODE_SMII:
            Cfg->BusMode = NET_PHY_BUS_MODE_SMII;
            break;

        case UCOS_NET_PHY_BUS_MODE_GMII:
            Cfg->BusMode = NET_PHY_BUS_MODE_GMII;
            break;

        default:
            Cfg->BusMode = NET_PHY_BUS_MODE_GMII;
            break;
    }

    if (UCOS_PHY_TYPE == UCOS_NET_PHY_TYPE_INT)
        Cfg->Type = NET_PHY_TYPE_INT;
    else
        Cfg->Type = NET_PHY_TYPE_EXT;

    switch (UCOS_PHY_SPEED) {
        case UCOS_NET_PHY_SPD_10:
            Cfg->Spd = NET_PHY_SPD_10;
            break;

        case UCOS_NET_PHY_SPD_100:
            Cfg->Spd = NET_PHY_SPD_100;
            break;

        case UCOS_NET_PHY_SPD_1000:
            Cfg->Spd = NET_PHY_SPD_1000;
            break;

        case UCOS_NET_PHY_SPD_AUTO:
        default:
            Cfg->Spd = UCOS_NET_PHY_SPD_AUTO;
            break;
    }

    if (UCOS_PHY_DUPLEX == UCOS_NET_PHY_DUPLEX_AUTO)
        Cfg->Duplex = NET_PHY_DUPLEX_AUTO;
    else if (UCOS_PHY_DUPLEX == UCOS_NET_PHY_DUPLEX_HALF)
        Cfg->Duplex = NET_PHY_DUPLEX_HALF;
    else
        Cfg->Duplex = NET_PHY_DUPLEX_FULL;
}
#endif /* #if (APP_TCPIP_ENABLED == DEF_ENABLED) */

#if (APP_TCPIP_ENABLED == DEF_ENABLED)
#if (UCOS_ETHERNET_DRIVER == UCOS_ETHERNET_EMACPS)
CPU_BOOLEAN UCOS_TCPIP_InitGEM (void)
{
    NET_IF_NBR        if_nbr;
    NET_ERR           err_net;
    CPU_BOOLEAN       if_init_res;


    UCOS_Print("UCOS - Initializing GEM Interface.\r\n");

    UCOS_TCPIP_IfCfgInit(&NetDevCfg);

#if (UCOS_CPU_TYPE == UCOS_CPU_TYPE_PS7)
#if (UCOS_IF_DEDIC_MEM_ADDR != 0)
    NetDevCfg.MemAddr = UCOS_IF_DEDIC_MEM_ADDR;
    NetDevCfg.MemSize = UCOS_IF_DEDIC_MEM_SIZE;
#else
    NetDevCfg.MemAddr = 0xFFFF0000u;
    NetDevCfg.MemSize = 0x4000;

	Xil_SetTlbAttributes(0xFFFF0000u, NORM_NONCACHE);
#endif

#if (UCOS_ETHERNET_DEVICE_ID == 0)                              /* Base address is returned as 0 from HSI, hardcode for now.*/
    NetDevCfg.BaseAddr = 0xE000B000u;
#else
    NetDevCfg.BaseAddr = 0xE000C000u;
#endif

    UCOS_TCPIP_PhyCfgInit(&NetPhyCfg);

    if_nbr = NetIF_Add((void    *)&NetIF_API_Ether,
                       (void    *)&NetDev_API_GEM,
                       (void    *)&NetDev_BSP_GEM_0,
                       (void    *)&NetDevCfg,
                       (void    *)&NetPhy_API_88E1111,
                       (void    *)&NetPhyCfg,
#if (APP_TCPIP_EXP_ENABLED == DEF_ENABLED)
                                  DEF_NULL,
#endif
                                  &err_net);
#else

#if (UCOS_IF_DEDIC_MEM_ADDR != 0)
    NetDevCfg.MemAddr = UCOS_IF_DEDIC_MEM_ADDR;
    NetDevCfg.MemSize = UCOS_IF_DEDIC_MEM_SIZE;
#else
    NetDevCfg.MemAddr = 0xFFFC0000u;
    NetDevCfg.MemSize = 0x40000;
#if (UCOS_CPU_TYPE == UCOS_CPU_TYPE_PSUR5)
    Xil_DisableMPURegionByRegNum(9);
    Xil_SetMPURegionByRegNum(9, 0xFFFC0000u, 0x40000, NORM_NSHARED_NCACHE | PRIV_RW_USER_RW);
#else
	Xil_SetTlbAttributes(0xFFFC0000u, NORM_NONCACHE);
#endif
#endif

    NetDevCfg.BaseAddr = 0xFF0E0000u;
    NetDevCfg.RxBufPoolType = NET_IF_MEM_TYPE_DEDICATED;
    NetDevCfg.TxBufPoolType = NET_IF_MEM_TYPE_DEDICATED;


    UCOS_TCPIP_PhyCfgInit(&NetPhyCfg);

    if_nbr = NetIF_Add((void    *)&NetIF_API_Ether,

#if (UCOS_CPU_TYPE == UCOS_CPU_TYPE_PSUR5)
                       (void    *)&NetDev_API_GEM64_32,
#else
                       (void    *)&NetDev_API_GEM64,
#endif
                       (void    *)&NetDev_BSP_Ultrascale_GEM_3,
                       (void    *)&NetDevCfg,
                       (void    *)&NetPhy_API_DP83867IR,
                       (void    *)&NetPhyCfg,
#if (APP_TCPIP_EXP_ENABLED == DEF_ENABLED)
                                  DEF_NULL,
#endif
                                  &err_net);
#endif

    if (err_net != NET_IF_ERR_NONE) {
        UCOS_Printf("UCOS - Error initializing GEM Interface. Error code %d\r\n", err_net);
        return (DEF_FAIL);
    }

    UCOS_Printf("UCOS - GEM Interface initialized successfully with if number %d.\r\n", if_nbr);

    UCOS_Print("UCOS - Starting GEM Interface.\r\n");
    NetIF_Start(if_nbr, &err_net);
    if (err_net != NET_IF_ERR_NONE) {
        UCOS_Printf("UCOS - Error starting GEM Interface. Error code %d\r\n", err_net);
        return (DEF_FAIL);
    }

    UCOS_Print("UCOS - GEM Interface started successfully.\r\n");

#if (UCOS_CPU_TYPE == UCOS_CPU_TYPE_PSUA53 || UCOS_CPU_TYPE == UCOS_CPU_TYPE_PSUR5)
                                                                /* Enable Tx and Rx clock shift for the PHY.            */
    NetPhy_DP83867IR_DelayCfg( if_nbr,
                               DEF_YES,
                               0xA,                             /* Tx Clock Shift: 2.75 nS.                             */
                               DEF_YES,
                               0x8,                             /* Rx Clock Shift: 2.25 nS.                             */
                              &err_net);

    if (err_net != NET_PHY_ERR_NONE) {
        UCOS_Printf("UCOS - Error configuring DP83867IR PHY. Error code %d\r\n", err_net);
        return (DEF_FAIL);
    }
#endif

    if_init_res = UCOS_TCPIP_IF_Cfg(if_nbr);

    return (if_init_res);
}
#endif

#if (UCOS_ETHERNET_DRIVER == UCOS_ETHERNET_AXIETHLITE)

extern UCOS_AXIETHERNETLITE_Config UCOS_AXIETHERNETLITE_ConfigTable[];

CPU_BOOLEAN UCOS_TCPIP_InitAXIEthLite (void)
{
    NET_IF_NBR if_nbr;
    NET_ERR err_net;
#ifdef NET_IPv4_MODULE_EN
    NET_IPv4_ADDR addr_ipv4;
    NET_IPv4_ADDR msk_ipv4;
    NET_IPv4_ADDR gateway_ipv4;
#endif
    NET_IPv6_ADDR addr_ip;
    NET_IP_ADDR_FAMILY addr_family;
    CPU_BOOLEAN if_init_res;


    UCOS_Print("UCOS - Initializing Ethernet Lite Interface.\r\n");

    UCOS_TCPIP_IfCfgInit(&NetDevCfg);

    NetDevCfg.MemAddr = 0;
    NetDevCfg.MemSize = 0;

    if (UCOS_AXIETHERNETLITE_ConfigTable[UCOS_ETHERNET_DEVICE_ID].RxPong != 0) {
        NetDevCfg.RxDescNbr = 2;
    } else {
        NetDevCfg.RxDescNbr = 1;
    }

    if (UCOS_AXIETHERNETLITE_ConfigTable[UCOS_ETHERNET_DEVICE_ID].TxPong != 0) {
        NetDevCfg.TxDescNbr = 2;
    } else {
        NetDevCfg.TxDescNbr = 1;
    }

    NetDevCfg.BaseAddr = UCOS_AXIETHERNETLITE_ConfigTable[UCOS_ETHERNET_DEVICE_ID].BaseAddress;

    UCOS_TCPIP_PhyCfgInit(&NetPhyCfg);

    NetPhyCfg.BusMode = NET_PHY_BUS_MODE_MII;

    if_nbr = NetIF_Add((void    *)&NetIF_API_Ether,
                       (void    *)&NetDev_API_XIL_ETHER_LITE,
                       (void    *)&NetDev_BSP_AXIEthernetLite_0,
                       (void    *)&NetDevCfg,
                       (void    *)&NetPhy_API_88E1111,
                       (void    *)&NetPhyCfg,
                                  &err_net);

    if (err_net != NET_IF_ERR_NONE) {
        UCOS_Printf("UCOS - Error initializing Ethernet Lite Interface. Error code %d\r\n", err_net);
        return (DEF_FAIL);
    }

    UCOS_Printf("UCOS - Ethernet Lite Interface initialized successfully with if number %d.\r\n", if_nbr);

    UCOS_Print("UCOS - Starting Ethernet Lite Interface.\r\n");
    NetIF_Start(if_nbr, &err_net);
    if (err_net != NET_IF_ERR_NONE) {
        UCOS_Printf("UCOS - Error starting Ethernet Lite Interface. Error code %d\r\n", err_net);
        return (DEF_FAIL);
    }

    UCOS_Print("UCOS - Ethernet Lite Interface started successfully.\r\n");

    if_init_res = UCOS_TCPIP_IF_Cfg(if_nbr);

    return (if_init_res);
}
#endif


#if (UCOS_ETHERNET_DRIVER == UCOS_ETHERNET_AXIETH)

extern UCOS_AXIETHERNET_Config UCOS_AXIETHERNET_ConfigTable[];

CPU_BOOLEAN UCOS_TCPIP_InitAXIEth (void)
{
    NET_IF_NBR if_nbr;
    NET_ERR err_net;
#ifdef NET_IPv4_MODULE_EN
    NET_IPv4_ADDR addr_ipv4;
    NET_IPv4_ADDR msk_ipv4;
    NET_IPv4_ADDR gateway_ipv4;
#endif
    NET_IPv6_ADDR addr_ip;
    NET_IP_ADDR_FAMILY addr_family;
    CPU_BOOLEAN if_init_res;


    UCOS_Print("UCOS - Initializing Ethernet Interface.\r\n");

    UCOS_TCPIP_IfCfgInit(&NetDevCfg);

    NetDevCfg.BaseAddr = UCOS_AXIETHERNET_ConfigTable[UCOS_ETHERNET_DEVICE_ID].BaseAddress;

    UCOS_TCPIP_PhyCfgInit(&NetPhyCfg);

    NetPhyCfg.BusMode = NET_PHY_BUS_MODE_MII;

    if_nbr = NetIF_Add((void    *)&NetIF_API_Ether,
                       (void    *)&NetDev_API_XIL_ETHER,
                       (void    *)&NetDev_BSP_AXIEthernet_0,
                       (void    *)&NetDevCfg,
                       (void    *)&NetPhy_API_88E1111,
                       (void    *)&NetPhyCfg,
                                  &err_net);

    if (err_net != NET_IF_ERR_NONE) {
        UCOS_Printf("UCOS - Error initializing Ethernet Interface. Error code %d\r\n", err_net);
        return (DEF_FAIL);
    }

    UCOS_Printf("UCOS - Ethernet Interface initialized successfully with if number %d.\r\n", if_nbr);

    UCOS_Print("UCOS - Starting Ethernet Interface.\r\n");
    NetIF_Start(if_nbr, &err_net);
    if (err_net != NET_IF_ERR_NONE) {
        UCOS_Printf("UCOS - Error starting Ethernet Interface. Error code %d\r\n", err_net);
        return (DEF_FAIL);
    }

    UCOS_Print("UCOS - Ethernet Interface started successfully.\r\n");

    if_init_res = UCOS_TCPIP_IF_Cfg(if_nbr);

    return (if_init_res);
}
#endif
#endif
