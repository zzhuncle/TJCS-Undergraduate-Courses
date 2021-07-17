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
    ucos_log_put "TRACE" "Generating ucos_usbh."

    set ucos_src_base [ucos_get_source_base]
    set usbh_src_base [ucos_get_product_source_base usbh]

    if {[llength $usbh_src_base] == 0} {
        set usbh_src_base $ucos_src_base
    }

    file copy -force "$usbh_src_base/uC-USB-Host/Cfg/Template/usbh_cfg.h" "./src"
    file copy -force "$usbh_src_base/uC-USB-Host/Cfg/Template/usbh_hc_cfg.c" "./src"
    file copy -force "$usbh_src_base/uC-USB-Host/Cfg/Template/usbh_hc_cfg.h" "./src"

    file copy -force "$usbh_src_base/uC-USB-Host" "./src/uC-USB-Host"

    file mkdir "../../include/Source"
    file mkdir "../../include/HCD/EHCI"
    file mkdir "../../include/HCD/OHCI"
    file mkdir "../../include/Class/CDC/ACM"
    file mkdir "../../include/Class/FTDI"
    file mkdir "../../include/Class/HID"
    file mkdir "../../include/Class/MSC"

    set_define "./src/usbh_cfg.h" "USBH_CFG_MAX_NBR_HC"            [format "%u" [get_property CONFIG.USBH_CFG_MAX_NBR_HC            $ucos_handle]]
    set_define "./src/usbh_cfg.h" "USBH_CFG_MAX_NBR_CLASS_DRVS"    [format "%u" [get_property CONFIG.USBH_CFG_MAX_NBR_CLASS_DRVS    $ucos_handle]]
    set_define "./src/usbh_cfg.h" "USBH_CFG_MAX_NBR_DEVS"          [format "%u" [get_property CONFIG.USBH_CFG_MAX_NBR_DEVS          $ucos_handle]]
    set_define "./src/usbh_cfg.h" "USBH_CFG_MAX_NBR_CFGS"          [format "%u" [get_property CONFIG.USBH_CFG_MAX_NBR_CFGS          $ucos_handle]]
    set_define "./src/usbh_cfg.h" "USBH_CFG_MAX_NBR_IFS"           [format "%u" [get_property CONFIG.USBH_CFG_MAX_NBR_IFS           $ucos_handle]]
    set_define "./src/usbh_cfg.h" "USBH_CFG_MAX_NBR_EPS"           [format "%u" [get_property CONFIG.USBH_CFG_MAX_NBR_EPS           $ucos_handle]]
    set_define "./src/usbh_cfg.h" "USBH_CFG_MAX_CFG_DATA_LEN"      [format "%u" [get_property CONFIG.USBH_CFG_MAX_CFG_DATA_LEN      $ucos_handle]]
    set_define "./src/usbh_cfg.h" "USBH_CFG_MAX_STR_LEN"           [format "%u" [get_property CONFIG.USBH_CFG_MAX_STR_LEN           $ucos_handle]]
    set_define "./src/usbh_cfg.h" "USBH_CFG_STD_REQ_TIMEOUT"       [format "%u" [get_property CONFIG.USBH_CFG_STD_REQ_TIMEOUT       $ucos_handle]]
    set_define "./src/usbh_cfg.h" "USBH_CFG_STD_REQ_RETRY"         [format "%u" [get_property CONFIG.USBH_CFG_STD_REQ_RETRY         $ucos_handle]]
    set_define "./src/usbh_cfg.h" "USBH_CFG_MAX_ISOC_DESC"         [format "%u" [get_property CONFIG.USBH_CFG_MAX_ISOC_DESC         $ucos_handle]]
    set_define "./src/usbh_cfg.h" "USBH_CFG_MAX_EXTRA_URB_PER_DEV" [format "%u" [get_property CONFIG.USBH_CFG_MAX_EXTRA_URB_PER_DEV $ucos_handle]]
    set_define "./src/usbh_cfg.h" "USBH_CFG_MAX_HUBS"              [format "%u" [get_property CONFIG.USBH_CFG_MAX_HUBS              $ucos_handle]]
    set_define "./src/usbh_cfg.h" "USBH_CFG_MAX_HUB_PORTS"         [format "%u" [get_property CONFIG.USBH_CFG_MAX_HUB_PORTS         $ucos_handle]]

    set_define "./src/usbh_cfg.h" "USBH_CDC_CFG_MAX_DEV"     [format "%u" [get_property CONFIG.USBH_CDC_CFG_MAX_DEV     $ucos_handle]]
    set_define "./src/usbh_cfg.h" "USBH_CDC_ACM_CFG_MAX_DEV" [format "%u" [get_property CONFIG.USBH_CDC_ACM_CFG_MAX_DEV $ucos_handle]]

    set_define "./src/usbh_cfg.h" "USBH_HID_CFG_MAX_DEV"             [format "%u" [get_property CONFIG.USBH_HID_CFG_MAX_DEV             $ucos_handle]]
    set_define "./src/usbh_cfg.h" "USBH_HID_CFG_MAX_NBR_APP_COLL"    [format "%u" [get_property CONFIG.USBH_HID_CFG_MAX_NBR_APP_COLL    $ucos_handle]]
    set_define "./src/usbh_cfg.h" "USBH_HID_CFG_MAX_NBR_REPORT_ID"   [format "%u" [get_property CONFIG.USBH_HID_CFG_MAX_NBR_REPORT_ID   $ucos_handle]]
    set_define "./src/usbh_cfg.h" "USBH_HID_CFG_MAX_NBR_REPORT_FMT"  [format "%u" [get_property CONFIG.USBH_HID_CFG_MAX_NBR_REPORT_FMT  $ucos_handle]]
    set_define "./src/usbh_cfg.h" "USBH_HID_CFG_MAX_NBR_USAGE"       [format "%u" [get_property CONFIG.USBH_HID_CFG_MAX_NBR_USAGE       $ucos_handle]]
    set_define "./src/usbh_cfg.h" "USBH_HID_CFG_MAX_TX_BUF_SIZE"     [format "%u" [get_property CONFIG.USBH_HID_CFG_MAX_TX_BUF_SIZE     $ucos_handle]]
    set_define "./src/usbh_cfg.h" "USBH_HID_CFG_MAX_RX_BUF_SIZE"     [format "%u" [get_property CONFIG.USBH_HID_CFG_MAX_RX_BUF_SIZE     $ucos_handle]]
    set_define "./src/usbh_cfg.h" "USBH_HID_CFG_MAX_NBR_RXCB"        [format "%u" [get_property CONFIG.USBH_HID_CFG_MAX_NBR_RXCB        $ucos_handle]]
    set_define "./src/usbh_cfg.h" "USBH_HID_CFG_MAX_REPORT_DESC_LEN" [format "%u" [get_property CONFIG.USBH_HID_CFG_MAX_REPORT_DESC_LEN $ucos_handle]]
    set_define "./src/usbh_cfg.h" "USBH_HID_CFG_MAX_ERR_CNT"         [format "%u" [get_property CONFIG.USBH_HID_CFG_MAX_ERR_CNT         $ucos_handle]]
    set_define "./src/usbh_cfg.h" "USBH_HID_CFG_MAX_GLOBAL"          [format "%u" [get_property CONFIG.USBH_HID_CFG_MAX_GLOBAL          $ucos_handle]]
    set_define "./src/usbh_cfg.h" "USBH_HID_CFG_MAX_COLL"            [format "%u" [get_property CONFIG.USBH_HID_CFG_MAX_COLL            $ucos_handle]]

    set_define "./src/usbh_cfg.h" "USBH_MSC_CFG_MAX_DEV" [format "%u" [get_property CONFIG.USBH_MSC_CFG_MAX_DEV $ucos_handle]]

    set_define "./src/usbh_cfg.h" "USBH_FTDI_CFG_MAX_DEV"           [format "%u" [get_property CONFIG.USBH_FTDI_CFG_MAX_DEV           $ucos_handle]]
    set_define "./src/usbh_cfg.h" "USBH_FTDI_CFG_ID_VENDOR_CUSTOM"  [format "%u" [get_property CONFIG.USBH_FTDI_CFG_ID_VENDOR_CUSTOM  $ucos_handle]]
    set_define "./src/usbh_cfg.h" "USBH_FTDI_CFG_ID_PRODUCT_CUSTOM" [format "%u" [get_property CONFIG.USBH_FTDI_CFG_ID_PRODUCT_CUSTOM $ucos_handle]]

    set_define "./src/usbh_cfg.h" "USBH_CFG_PRINT_LOG"   [expr ([get_property CONFIG.USBH_CFG_PRINT_LOG   $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/usbh_cfg.h" "USBH_CFG_PRINT_ERR"   [expr ([get_property CONFIG.USBH_CFG_PRINT_ERR   $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/usbh_cfg.h" "USBH_CFG_PRINT_FNAME" [expr ([get_property CONFIG.USBH_CFG_PRINT_FNAME $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/usbh_cfg.h" "USBH_PRINTF" "UCOS_Printf"

    set config_file [open_include_file "xparameters.h"]

    set async_prio [get_property CONFIG.USBH_TASK_CFG_ASYNC_PRIO $ucos_handle]
    set async_stack [get_property CONFIG.USBH_TASK_CFG_ASYNC_STACK_SIZE $ucos_handle]
    set hub_prio [get_property CONFIG.USBH_TASK_CFG_HUB_PRIO $ucos_handle]
    set hub_stack [get_property CONFIG.USBH_TASK_CFG_HUB_STACK_SIZE $ucos_handle]

    puts $config_file "#define USBH_TASK_CFG_ASYNC_PRIO $async_prio"
    puts $config_file "#define USBH_TASK_CFG_ASYNC_STACK_SIZE $async_stack"
    puts $config_file "#define USBH_TASK_CFG_HUB_PRIO $hub_prio"
    puts $config_file "#define USBH_TASK_CFG_HUB_STACK_SIZE $hub_stack"
}
