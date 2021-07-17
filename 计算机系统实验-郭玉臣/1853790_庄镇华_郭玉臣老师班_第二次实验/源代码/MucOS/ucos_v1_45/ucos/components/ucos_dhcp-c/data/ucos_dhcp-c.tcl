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
    ucos_log_put "TRACE" "Generating ucos_dhcp-c."

    set ucos_src_base [ucos_get_source_base]
    set dhcpc_src_base [ucos_get_product_source_base dhcpc]

    if {[llength $dhcpc_src_base] == 0} {
        set dhcpc_src_base $ucos_src_base
    }

    file copy -force "$dhcpc_src_base/uC-DHCPc/Cfg/Template/dhcp-c_cfg.h" "./src"

    file copy -force "$dhcpc_src_base/uC-DHCPc" "./src/uC-DHCPc"

    file mkdir "../../include/Source"


    set_define "./src/dhcp-c_cfg.h" "DHCPc_OS_CFG_TASK_PRIO" [format "%u" [get_property CONFIG.DHCPc_OS_CFG_TASK_PRIO $ucos_handle]]
    set_define "./src/dhcp-c_cfg.h" "DHCPc_OS_CFG_TASK_STK_SIZE" [format "%u" [get_property CONFIG.DHCPc_OS_CFG_TASK_STK_SIZE $ucos_handle]]

    set_define "./src/dhcp-c_cfg.h" "DHCPc_OS_CFG_TMR_TASK_PRIO" [format "%u" [get_property CONFIG.DHCPc_OS_CFG_TMR_TASK_PRIO $ucos_handle]]
    set_define "./src/dhcp-c_cfg.h" "DHCPc_OS_CFG_TMR_TASK_STK_SIZE" [format "%u" [get_property CONFIG.DHCPc_OS_CFG_TMR_TASK_STK_SIZE $ucos_handle]]

    set_define "./src/dhcp-c_cfg.h" "DHCPc_CFG_IP_PORT_SERVER" [format "%u" [get_property CONFIG.DHCPc_CFG_IP_PORT_SERVER $ucos_handle]]
    set_define "./src/dhcp-c_cfg.h" "DHCPc_CFG_IP_PORT_CLIENT" [format "%u" [get_property CONFIG.DHCPc_CFG_IP_PORT_CLIENT $ucos_handle]]

    set_define "./src/dhcp-c_cfg.h" "DHCPc_CFG_MAX_RX_TIMEOUT_MS" [format "%u" [get_property CONFIG.DHCPc_CFG_MAX_RX_TIMEOUT_MS $ucos_handle]]
    set_define "./src/dhcp-c_cfg.h" "DHCPc_CFG_PARAM_REQ_TBL_SIZE" [format "%u" [get_property CONFIG.DHCPc_CFG_PARAM_REQ_TBL_SIZE $ucos_handle]]
    set_define "./src/dhcp-c_cfg.h" "DHCPc_CFG_MAX_NBR_IF" [format "%u" [get_property CONFIG.DHCPc_CFG_MAX_NBR_IF $ucos_handle]]

    set_define "./src/dhcp-c_cfg.h" "DHCPc_CFG_ADDR_VALIDATE_EN" [expr ([get_property CONFIG.DHCPc_CFG_ADDR_VALIDATE_EN $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/dhcp-c_cfg.h" "DHCPc_CFG_DYN_LOCAL_LINK_ADDR_EN" [expr ([get_property CONFIG.DHCPc_CFG_DYN_LOCAL_LINK_ADDR_EN $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]

    set_define "./src/dhcp-c_cfg.h" "DHCPc_CFG_LOCAL_LINK_MAX_RETRY" [format "%u" [get_property CONFIG.DHCPc_CFG_LOCAL_LINK_MAX_RETRY $ucos_handle]]

    set_define "./src/dhcp-c_cfg.h" "DHCPc_CFG_ARG_CHK_EXT_EN" [expr ([get_property CONFIG.DHCPc_CFG_ARG_CHK_EXT_EN $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/dhcp-c_cfg.h" "DHCPc_CFG_ARG_CHK_DBG_EN" [expr ([get_property CONFIG.DHCPc_CFG_ARG_CHK_DBG_EN $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/dhcp-c_cfg.h" "DHCPc_DBG_CFG_MEM_CLR_EN" [expr ([get_property CONFIG.DHCPc_DBG_CFG_MEM_CLR_EN $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
}
