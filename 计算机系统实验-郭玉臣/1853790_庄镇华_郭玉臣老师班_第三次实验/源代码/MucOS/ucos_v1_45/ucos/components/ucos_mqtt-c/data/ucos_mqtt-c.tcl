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
    ucos_log_put "TRACE" "Generating ucos_mqtt-c."

    set ucos_src_base [ucos_get_source_base]
    set mqttc_src_base [ucos_get_product_source_base mqttc]


    if {[llength $mqttc_src_base] == 0} {
        set mqttc_src_base $ucos_src_base
    }

    file copy -force "$mqttc_src_base/uC-MQTT/Client/Cfg/Template/mqtt-c_cfg.h" "./src"

    file copy -force "$mqttc_src_base/uC-MQTT" "./src/uC-MQTT"

    file mkdir "../../include/Common"
    file mkdir "../../include/Client/Source"


    set_define "./src/mqtt-c_cfg.h" "MQTTc_CFG_ARG_CHK_EXT_EN"     [expr ([get_property CONFIG.MQTTc_CFG_ARG_CHK_EXT_EN     $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/mqtt-c_cfg.h" "MQTTc_CFG_DBG_GLOBAL_BUF_EN"  [expr ([get_property CONFIG.MQTTc_CFG_DBG_GLOBAL_BUF_EN  $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/mqtt-c_cfg.h" "MQTTc_CFG_DBG_GLOBAL_BUF_LEN" [format "%u" [get_property CONFIG.MQTTc_CFG_DBG_GLOBAL_BUF_LEN   $ucos_handle]]


    set config_file [open_include_file "xparameters.h"]

    set task_prio       [get_property CONFIG.MQTTc_OS_CFG_TASK_PRIO $ucos_handle].
    set task_stack      [get_property CONFIG.MQTTc_OS_CFG_TASK_STK_SIZE $ucos_handle]
    set msg_qty         [get_property CONFIG.MQTTc_CFG_MSG_QTY $ucos_handle]
    set inac_timeout    [get_property CONFIG.MQTTc_CFG_INACTIVITY_TIMEOUT $ucos_handle]
    set internal_dly    [get_property CONFIG.MQTTc_CFG_INTERNAL_TASK_DLY $ucos_handle]

    puts $config_file "#define MQTTc_OS_CFG_TASK_PRIO $task_prio"
    puts $config_file "#define MQTTc_OS_CFG_TASK_STK_SIZE $task_stack"
    puts $config_file "#define MQTTc_CFG_MSG_QTY $msg_qty"
    puts $config_file "#define MQTTc_CFG_INACTIVITY_TIMEOUT $inac_timeout"
    puts $config_file "#define MQTTc_CFG_INTERNAL_TASK_DLY $internal_dly"
}
