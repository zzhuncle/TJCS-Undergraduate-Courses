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
    ucos_log_put "TRACE" "Generating ucos_http-c."

    set ucos_src_base [ucos_get_source_base]
    set httpc_src_base [ucos_get_product_source_base httpc]


    if {[llength $httpc_src_base] == 0} {
        set httpc_src_base $ucos_src_base
    }

    file copy -force "$httpc_src_base/uC-HTTP/Client/Cfg/Template/http-c_cfg.h" "./src"
    file copy -force "./src/cfg/http-c_cfg.c" "./src"

    file copy -force "$httpc_src_base/uC-HTTP" "./src/uC-HTTP"

    file mkdir "../../include/Common"
    file mkdir "../../include/Client/Source"


    set_define "./src/http-c_cfg.h" "HTTPc_CFG_ARG_CHK_EXT_EN"     [expr ([get_property CONFIG.HTTPc_CFG_ARG_CHK_EXT_EN     $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/http-c_cfg.h" "HTTPc_CFG_MODE_ASYNC_TASK_EN" [expr ([get_property CONFIG.HTTPc_CFG_MODE_ASYNC_TASK_EN $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/http-c_cfg.h" "HTTPc_CFG_MODE_BLOCK_EN"      [expr ([get_property CONFIG.HTTPc_CFG_MODE_BLOCK_EN      $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/http-c_cfg.h" "HTTPc_CFG_PERSISTENT_EN"      [expr ([get_property CONFIG.HTTPc_CFG_PERSISTENT_EN      $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/http-c_cfg.h" "HTTPc_CFG_CHUNK_TX_EN"        [expr ([get_property CONFIG.HTTPc_CFG_CHUNK_TX_EN        $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/http-c_cfg.h" "HTTPc_CFG_QUERY_STR_EN"       [expr ([get_property CONFIG.HTTPc_CFG_QUERY_STR_EN       $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/http-c_cfg.h" "HTTPc_CFG_HDR_RX_EN"          [expr ([get_property CONFIG.HTTPc_CFG_HDR_RX_EN          $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/http-c_cfg.h" "HTTPc_CFG_HDR_TX_EN"          [expr ([get_property CONFIG.HTTPc_CFG_HDR_TX_EN          $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/http-c_cfg.h" "HTTPc_CFG_FORM_EN"            [expr ([get_property CONFIG.HTTPc_CFG_FORM_EN            $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/http-c_cfg.h" "HTTPc_CFG_USER_DATA_EN"       [expr ([get_property CONFIG.HTTPc_CFG_USER_DATA_EN       $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/http-c_cfg.h" "HTTPc_CFG_WEBSOCKET_EN"       [expr ([get_property CONFIG.HTTPc_CFG_WEBSOCKET_EN       $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]

    set config_file [open_include_file "xparameters.h"]

    set task_prio       [get_property CONFIG.HTTPc_OS_CFG_TASK_PRIO $ucos_handle]
    set task_stack      [get_property CONFIG.HTTPc_OS_CFG_TASK_STK_SIZE $ucos_handle]
    set task_delay      [get_property CONFIG.UCOS_HTTPc_OS_CFG_TASK_DELAY $ucos_handle]
    set task_q_size     [get_property CONFIG.UCOS_HTTPc_OS_CFG_MSG_Q_SIZE $ucos_handle]
    set task_timeout    [get_property CONFIG.UCOS_HTTPc_OS_CFG_TIMEOUT $ucos_handle]
    set task_inactivity [get_property CONFIG.UCOS_HTTPc_OS_CFG_INACTIVITY_TIMEOUT $ucos_handle]


    puts $config_file "#define HTTPc_OS_CFG_TASK_PRIO $task_prio"
    puts $config_file "#define HTTPc_OS_CFG_TASK_STK_SIZE $task_stack"
    puts $config_file "#define UCOS_HTTPc_OS_CFG_TASK_DELAY $task_delay"
    puts $config_file "#define UCOS_HTTPc_OS_CFG_MSG_Q_SIZE $task_q_size"
    puts $config_file "#define UCOS_HTTPc_OS_CFG_TIMEOUT $task_timeout"
    puts $config_file "#define UCOS_HTTPc_OS_CFG_INACTIVITY_TIMEOUT $task_inactivity"

}
