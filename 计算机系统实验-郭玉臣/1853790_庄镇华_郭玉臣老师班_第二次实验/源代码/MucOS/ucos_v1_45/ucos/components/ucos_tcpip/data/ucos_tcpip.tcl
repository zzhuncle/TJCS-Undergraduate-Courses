#############################################################################
#                               uCOS XSDK BSP                               #
#                                                                           #
#        (c) Copyright 2014-2020; Silicon Laboratories Inc.,                #
#               400 W. Cesar Chavez, Austin, TX 78701                       #
#                                                                           #
#  All rights reserved. Protected by international copyright laws.          #
#                                                                           #
# Your use of this software is subject to your acceptance of the terms      #
# of a Silicon Labs Micrium software license, which can be obtained by      #
# contacting info@micrium.com. If you do not agree to the terms of this     #
# license, you may not use this software.                                   #
#                                                                           #
# Please help us continue to provide the Embedded community with the finest #
# software available. Your honesty is greatly appreciated.                  #
#                                                                           #
#   You can find our product's documentation at: doc.micrium.com            #
#                                                                           #
#         For more information visit us at: www.micrium.com                 #
#############################################################################


source ../../../bsp/data/ucos_tcl_utils.tcl
source ../../../bsp/data/xil_tcl_utils.tcl

proc generate {ucos_handle} {
    ucos_log_put "TRACE" "Generating ucos_tcpip."

    set ucos_src_base [ucos_get_source_base]
    set tcp_src_base [ucos_get_product_source_base tcpip]

    if {[llength $tcp_src_base] == 0} {
        set tcp_src_base $ucos_src_base
    }

    file copy -force "$tcp_src_base/uC-TCPIP/Cfg/Template/net_cfg.h" "./src"

    file copy -force "./src/cfg/net_cfg.c" "./src"

    file copy -force "$tcp_src_base/uC-TCPIP" "./src/uC-TCPIP"
    file delete -force "./src/uC-TCPIP/Cfg/Template/net_cfg.h"

    file mkdir "../../include/Source"
    file mkdir "../../include/Cmd"
    file mkdir "../../include/IF"
    file mkdir "../../include/IP/IPv4"
    file mkdir "../../include/IP/IPv6"
    file mkdir "../../include/Dev/Ether/GEM"
    file mkdir "../../include/Dev/Ether/XIL_ETHER_LITE"
    file mkdir "../../include/Dev/Ether/XIL_ETHER"
    file mkdir "../../include/Dev/Ether/PHY/88E1111"
    file mkdir "../../include/Dev/Ether/PHY/DP83867IR"
    file mkdir "../../include/Dev/Ether/PHY/Generic"
    file mkdir "../../include/FS"
    file mkdir "../../include/Modules/Common"

    set_define "./src/net_cfg.h" "NET_CFG_IF_RX_Q_SIZE"         [format "%u" [get_property CONFIG.NET_CFG_IF_RX_Q_SIZE         $ucos_handle]]
    set_define "./src/net_cfg.h" "NET_CFG_IF_TX_DEALLOC_Q_SIZE" [format "%u" [get_property CONFIG.NET_CFG_IF_TX_DEALLOC_Q_SIZE $ucos_handle]]

    set_define "./src/net_cfg.h" "NET_DBG_CFG_MEM_CLR_EN"       [expr ([get_property CONFIG.NET_DBG_CFG_MEM_CLR_EN             $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/net_cfg.h" "NET_ERR_CFG_ARG_CHK_EXT_EN"   [expr ([get_property CONFIG.NET_ERR_CFG_ARG_CHK_EXT_EN         $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/net_cfg.h" "NET_ERR_CFG_ARG_CHK_DBG_EN"   [expr ([get_property CONFIG.NET_ERR_CFG_ARG_CHK_DBG_EN         $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/net_cfg.h" "NET_CTR_CFG_STAT_EN"          [expr ([get_property CONFIG.NET_CTR_CFG_STAT_EN                $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/net_cfg.h" "NET_CTR_CFG_ERR_EN"           [expr ([get_property CONFIG.NET_CTR_CFG_ERR_EN                 $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]

    set_define "./src/net_cfg.h" "NET_IF_CFG_MAX_NBR_IF"            [format "%u" [get_property CONFIG.NET_IF_CFG_MAX_NBR_IF             $ucos_handle]]
    set_define "./src/net_cfg.h" "NET_IF_CFG_LOOPBACK_EN"           [expr ([get_property CONFIG.NET_IF_CFG_LOOPBACK_EN                  $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/net_cfg.h" "NET_IF_CFG_ETHER_EN"              [expr ([get_property CONFIG.NET_IF_CFG_ETHER_EN                     $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/net_cfg.h" "NET_IF_CFG_WIFI_EN"               [expr ([get_property CONFIG.NET_IF_CFG_WIFI_EN                      $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/net_cfg.h" "NET_IF_CFG_TX_SUSPEND_TIMEOUT_MS" [format "%u" [get_property CONFIG.NET_IF_CFG_TX_SUSPEND_TIMEOUT_MS  $ucos_handle]]

    set_define "./src/net_cfg.h" "NET_ARP_CFG_CACHE_NBR"  [format "%u" [get_property CONFIG.NET_ARP_CFG_CACHE_NBR  $ucos_handle]]

    set_define "./src/net_cfg.h" "NET_NDP_CFG_CACHE_NBR"   [format "%u" [get_property CONFIG.NET_NDP_CFG_CACHE_NBR             $ucos_handle]]
    set_define "./src/net_cfg.h" "NET_NDP_CFG_DEST_NBR"    [format "%u" [get_property CONFIG.NET_NDP_CFG_DEST_NBR              $ucos_handle]]
    set_define "./src/net_cfg.h" "NET_NDP_CFG_PREFIX_NBR"  [format "%u" [get_property CONFIG.NET_NDP_CFG_PREFIX_NBR            $ucos_handle]]
    set_define "./src/net_cfg.h" "NET_NDP_CFG_ROUTER_NBR"  [format "%u" [get_property CONFIG.NET_NDP_CFG_ROUTER_NBR            $ucos_handle]]

    set_define "./src/net_cfg.h" "NET_IPv4_CFG_EN"               [expr ([get_property CONFIG.NET_IPv4_CFG_EN                     $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/net_cfg.h" "NET_IPv4_CFG_IF_MAX_NBR_ADDR"  [format "%u" [get_property CONFIG.NET_IPv4_CFG_IF_MAX_NBR_ADDR  $ucos_handle]]

    set_define "./src/net_cfg.h" "NET_IPv6_CFG_EN"                [expr ([get_property CONFIG.NET_IPv6_CFG_EN                $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/net_cfg.h" "NET_IPv6_CFG_ADDR_AUTO_CFG_EN"  [expr ([get_property CONFIG.NET_IPv6_CFG_ADDR_AUTO_CFG_EN  $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/net_cfg.h" "NET_IPv6_CFG_DAD_EN"            [expr ([get_property CONFIG.NET_IPv6_CFG_DAD_EN            $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/net_cfg.h" "NET_IPv6_CFG_IF_MAX_NBR_ADDR"   [format "%u" [get_property CONFIG.NET_IPv6_CFG_IF_MAX_NBR_ADDR  $ucos_handle]]

    set_define "./src/net_cfg.h" "NET_MCAST_CFG_IPv4_RX_EN"        [expr ([get_property CONFIG.NET_MCAST_CFG_IPv4_RX_EN  $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/net_cfg.h" "NET_MCAST_CFG_IPv4_TX_EN"        [expr ([get_property CONFIG.NET_MCAST_CFG_IPv4_TX_EN  $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/net_cfg.h" "NET_MCAST_CFG_HOST_GRP_NBR_MAX"  [format "%u" [get_property CONFIG.NET_MCAST_CFG_HOST_GRP_NBR_MAX  $ucos_handle]]

    set_define "./src/net_cfg.h" "NET_SOCK_CFG_SOCK_NBR_TCP"     [format "%u" [get_property CONFIG.NET_SOCK_CFG_SOCK_NBR_TCP  $ucos_handle]]
    set_define "./src/net_cfg.h" "NET_SOCK_CFG_SOCK_NBR_UDP"     [format "%u" [get_property CONFIG.NET_SOCK_CFG_SOCK_NBR_UDP  $ucos_handle]]
    set_define "./src/net_cfg.h" "NET_SOCK_CFG_SEL_EN"           [expr ([get_property CONFIG.NET_SOCK_CFG_SEL_EN  $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/net_cfg.h" "NET_SOCK_CFG_CONN_ACCEPT_Q_SIZE_MAX"  [format "%u" [get_property CONFIG.NET_SOCK_CFG_CONN_ACCEPT_Q_SIZE_MAX  $ucos_handle]]
    set_define "./src/net_cfg.h" "NET_SOCK_CFG_RX_Q_SIZE_OCTET"  [format "%u" [get_property CONFIG.NET_SOCK_CFG_RX_Q_SIZE_OCTET  $ucos_handle]]
    set_define "./src/net_cfg.h" "NET_SOCK_CFG_TX_Q_SIZE_OCTET"  [format "%u" [get_property CONFIG.NET_SOCK_CFG_TX_Q_SIZE_OCTET  $ucos_handle]]

    set_define "./src/net_cfg.h" "NET_TCP_CFG_EN" [expr ([get_property CONFIG.NET_TCP_CFG_EN  $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]

    set_define "./src/net_cfg.h" "NET_UDP_CFG_RX_CHK_SUM_DISCARD_EN" [expr ([get_property CONFIG.NET_UDP_CFG_RX_CHK_SUM_DISCARD_EN  $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/net_cfg.h" "NET_UDP_CFG_TX_CHK_SUM_EN" [expr ([get_property CONFIG.NET_UDP_CFG_TX_CHK_SUM_EN  $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]

    set_define "./src/net_cfg.h" "NET_TMR_CFG_NBR_TMR"    [format "%u" [get_property CONFIG.NET_TMR_CFG_NBR_TMR  $ucos_handle]]
    set_define "./src/net_cfg.h" "NET_TMR_CFG_TASK_FREQ"  [format "%u" [get_property CONFIG.NET_TMR_CFG_TASK_FREQ  $ucos_handle]]

    set dns_en [get_property CONFIG.NET_EXT_MODULE_CFG_DNS_EN $ucos_handle]

    if {$dns_en == "auto"} {
        if {[llength [get_libs -filter "NAME==ucos_dns-c"]] != 0} {
            set dns_en "true"
        } else {
            set dns_en "false"
        }
    }

    if {$dns_en == "true"} {
        set_define "./src/net_cfg.h" "NET_EXT_MODULE_CFG_DNS_EN" "DEF_ENABLED"
    } else {
        set_define "./src/net_cfg.h" "NET_EXT_MODULE_CFG_DNS_EN" "DEF_DISABLED"
    }

    set ethernet_if [get_ethernet_if_type]

    switch $ethernet_if {
        "ps7_ethernet" {
            set ipv4_rx_off_auto "true"
            set ipv4_tx_off_auto "true"
            set icmp_rx_off_auto "true"
            set icmp_tx_off_auto "false"
            set udp_rx_off_auto "true"
            set udp_tx_off_auto "true"
            set tcp_rx_off_auto "true"
            set tcp_tx_off_auto "true"
        }

        "default" {
            set ipv4_rx_off_auto "false"
            set ipv4_tx_off_auto "false"
            set icmp_rx_off_auto "false"
            set icmp_tx_off_auto "false"
            set udp_rx_off_auto "false"
            set udp_tx_off_auto "false"
            set tcp_rx_off_auto "false"
            set tcp_tx_off_auto "false"
        }
    }

    set ipv4_rx_off [get_property CONFIG.NET_IPV4_CFG_CHK_SUM_OFFLOAD_RX_EN  $ucos_handle]
    set ipv4_tx_off [get_property CONFIG.NET_IPV4_CFG_CHK_SUM_OFFLOAD_TX_EN  $ucos_handle]
    set icmp_rx_off [get_property CONFIG.NET_ICMP_CFG_CHK_SUM_OFFLOAD_RX_EN  $ucos_handle]
    set icmp_tx_off [get_property CONFIG.NET_ICMP_CFG_CHK_SUM_OFFLOAD_TX_EN  $ucos_handle]
    set udp_rx_off [get_property CONFIG.NET_UDP_CFG_CHK_SUM_OFFLOAD_RX_EN  $ucos_handle]
    set udp_tx_off [get_property CONFIG.NET_UDP_CFG_CHK_SUM_OFFLOAD_TX_EN  $ucos_handle]
    set tcp_rx_off [get_property CONFIG.NET_TCP_CFG_CHK_SUM_OFFLOAD_RX_EN  $ucos_handle]
    set tcp_tx_off [get_property CONFIG.NET_TCP_CFG_CHK_SUM_OFFLOAD_TX_EN  $ucos_handle]


    if {$ipv4_rx_off == "true" || ($ipv4_rx_off == "auto" && $ipv4_rx_off_auto == "true")} {
        set_define "./src/net_cfg.h" "NET_IPV4_CFG_CHK_SUM_OFFLOAD_RX_EN" "DEF_ENABLED"
    } else {
        set_define "./src/net_cfg.h" "NET_IPV4_CFG_CHK_SUM_OFFLOAD_RX_EN" "DEF_DISABLED"
    }

    if {$ipv4_tx_off == "true" || ($ipv4_tx_off == "auto" && $ipv4_tx_off_auto == "true")} {
        set_define "./src/net_cfg.h" "NET_IPV4_CFG_CHK_SUM_OFFLOAD_TX_EN" "DEF_ENABLED"
    } else {
        set_define "./src/net_cfg.h" "NET_IPV4_CFG_CHK_SUM_OFFLOAD_TX_EN" "DEF_DISABLED"
    }

    if {$icmp_rx_off == "true" || ($icmp_rx_off == "auto" && $icmp_rx_off_auto == "true")} {
        set_define "./src/net_cfg.h" "NET_ICMP_CFG_CHK_SUM_OFFLOAD_RX_EN" "DEF_ENABLED"
    } else {
        set_define "./src/net_cfg.h" "NET_ICMP_CFG_CHK_SUM_OFFLOAD_RX_EN" "DEF_DISABLED"
    }

    if {$icmp_tx_off == "true" || ($icmp_tx_off == "auto" && $icmp_tx_off_auto == "true")} {
        set_define "./src/net_cfg.h" "NET_ICMP_CFG_CHK_SUM_OFFLOAD_TX_EN" "DEF_ENABLED"
    } else {
        set_define "./src/net_cfg.h" "NET_ICMP_CFG_CHK_SUM_OFFLOAD_TX_EN" "DEF_DISABLED"
    }

    if {$udp_rx_off == "true" || ($udp_rx_off == "auto" && $udp_rx_off_auto == "true")} {
        set_define "./src/net_cfg.h" "NET_UDP_CFG_CHK_SUM_OFFLOAD_RX_EN" "DEF_ENABLED"
    } else {
        set_define "./src/net_cfg.h" "NET_UDP_CFG_CHK_SUM_OFFLOAD_RX_EN" "DEF_DISABLED"
    }

    if {$udp_tx_off == "true" || ($udp_tx_off == "auto" && $udp_tx_off_auto == "true")} {
        set_define "./src/net_cfg.h" "NET_UDP_CFG_CHK_SUM_OFFLOAD_TX_EN" "DEF_ENABLED"
    } else {
        set_define "./src/net_cfg.h" "NET_UDP_CFG_CHK_SUM_OFFLOAD_TX_EN" "DEF_DISABLED"
    }

    if {$tcp_rx_off == "true" || ($tcp_rx_off == "auto" && $tcp_rx_off_auto == "true")} {
        set_define "./src/net_cfg.h" "NET_TCP_CFG_CHK_SUM_OFFLOAD_RX_EN" "DEF_ENABLED"
    } else {
        set_define "./src/net_cfg.h" "NET_TCP_CFG_CHK_SUM_OFFLOAD_RX_EN" "DEF_DISABLED"
    }

    if {$tcp_tx_off == "true" || ($tcp_tx_off == "auto" && $tcp_tx_off_auto == "true")} {
        set_define "./src/net_cfg.h" "NET_TCP_CFG_CHK_SUM_OFFLOAD_TX_EN" "DEF_ENABLED"
    } else {
        set_define "./src/net_cfg.h" "NET_TCP_CFG_CHK_SUM_OFFLOAD_TX_EN" "DEF_DISABLED"
    }


    set config_file [open_include_file "xparameters.h"]
    set rx_prio [get_property CONFIG.NET_TASK_CFG_RX_PRIO $ucos_handle]
    set rx_stack [get_property CONFIG.NET_TASK_CFG_RX_STACK_SIZE $ucos_handle]
    set tx_prio [get_property CONFIG.NET_TASK_CFG_TXDEALLOC_PRIO $ucos_handle]
    set tx_stack [get_property CONFIG.NET_TASK_CFG_TXDEALLOC_STACK_SIZE $ucos_handle]
    set tmr_prio [get_property CONFIG.NET_TASK_CFG_TMR_PRIO $ucos_handle]
    set tmr_stack [get_property CONFIG.NET_TASK_CFG_TMR_STACK_SIZE $ucos_handle]

    puts $config_file "#define NET_TASK_CFG_RX_PRIO $rx_prio"
    puts $config_file "#define NET_TASK_CFG_RX_STACK_SIZE $rx_stack"
    puts $config_file "#define NET_TASK_CFG_TXDEALLOC_PRIO $tx_prio"
    puts $config_file "#define NET_TASK_CFG_TXDEALLOC_STACK_SIZE $tx_stack"
    puts $config_file "#define NET_TASK_CFG_TMR_PRIO $tmr_prio"
    puts $config_file "#define NET_TASK_CFG_TMR_STACK_SIZE $tmr_stack"
}
