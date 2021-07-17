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

    ucos_log_put "TRACE" "Generating ucos_can."

    set ucos_src_base [ucos_get_source_base]
    set can_src_base [ucos_get_product_source_base can]

    set source_avail 1
    if {[llength $can_src_base] == 0} {
        set source_avail 0
        set can_src_base $ucos_src_base
    }

    file copy -force "$can_src_base/uC-CAN" "./src/uC-CAN"
    #file copy -force "$can_src_base/uC-CAN/Cfg/Template/can_cfg.h" "./src/can_cfg.h"
    #file copy -force "$can_src_base/uC-CAN/Cfg/Template/can_cfg.c" "./src/can_cfg.c"

}
