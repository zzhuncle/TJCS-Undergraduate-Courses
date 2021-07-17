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
    ucos_log_put "TRACE" "Generating ucos_osiii."

    set ucos_src_base [ucos_get_source_base]

    file copy -force "$ucos_src_base/uCOS-III/Cfg/Template/os_cfg.h" "./src"
    file copy -force "$ucos_src_base/uCOS-III/Cfg/Template/os_cfg_app.h" "./src"

    file copy -force "$ucos_src_base/uCOS-III" "./src/uCOS-III"
    file delete -force "./src/uCOS-III/Cfg/Template/os_cfg.h"
    file delete -force "./src/uCOS-III/Cfg/Template/os_cfg_app.h"

    file mkdir "../../include/Source"


    set sw_proc_handle [get_sw_processor]
    set hw_proc_handle [get_cells [get_property HW_INSTANCE $sw_proc_handle] ]

    set proctype [get_property IP_NAME $hw_proc_handle]

    switch $proctype {
    "microblaze" {
        file copy -force "./src/mb/Makefile.inc" "./src/Makefile.inc"
    }

    "ps7_cortexa9" {
        file copy -force "./src/ps7/Makefile.inc" "./src/Makefile.inc"
    }

    "psu_cortexa53" {
        file copy -force "./src/psua53/Makefile.inc" "./src/Makefile.inc"
    }

    "psu_cortexr5" {
        file copy -force "./src/psur5/Makefile.inc" "./src/Makefile.inc"
    }

    "default" {error "Unknown processor type $proctype"}
    }

    set_define "./src/os_cfg.h" "OS_CFG_APP_HOOKS_EN"            [expr ([get_property CONFIG.OS_CFG_APP_HOOKS_EN             $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_ARG_CHK_EN"              [expr ([get_property CONFIG.OS_CFG_ARG_CHK_EN               $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_CALLED_FROM_ISR_CHK_EN"  [expr ([get_property CONFIG.OS_CFG_CALLED_FROM_ISR_CHK_EN   $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_DBG_EN"                  [expr ([get_property CONFIG.OS_CFG_DBG_EN                   $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_TICK_EN"                 [expr ([get_property CONFIG.OS_CFG_TICK_EN                  $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_DYN_TICK_EN"             [expr ([get_property CONFIG.OS_CFG_DYN_TICK_EN              $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_INVALID_OS_CALLS_CHK_EN" [expr ([get_property CONFIG.OS_CFG_INVALID_OS_CALLS_CHK_EN  $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_OBJ_TYPE_CHK_EN"         [expr ([get_property CONFIG.OS_CFG_OBJ_TYPE_CHK_EN          $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_TS_EN"                   [expr ([get_property CONFIG.OS_CFG_TS_EN                    $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_PRIO_MAX"                [format "%u" [get_property CONFIG.OS_CFG_PRIO_MAX           $ucos_handle]]
    set_define "./src/os_cfg.h" "OS_CFG_SCHED_LOCK_TIME_MEAS_EN" [expr ([get_property CONFIG.OS_CFG_SCHED_LOCK_TIME_MEAS_EN  $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_SCHED_ROUND_ROBIN_EN"    [expr ([get_property CONFIG.OS_CFG_SCHED_ROUND_ROBIN_EN     $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_STK_SIZE_MIN"            [format "%u" [get_property CONFIG.OS_CFG_STK_SIZE_MIN       $ucos_handle]]


    set_define "./src/os_cfg.h" "OS_CFG_FLAG_EN"            [expr ([get_property CONFIG.OS_CFG_FLAG_EN            $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_FLAG_DEL_EN"        [expr ([get_property CONFIG.OS_CFG_FLAG_DEL_EN        $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_FLAG_MODE_CLR_EN"   [expr ([get_property CONFIG.OS_CFG_FLAG_MODE_CLR_EN   $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_FLAG_PEND_ABORT_EN" [expr ([get_property CONFIG.OS_CFG_FLAG_PEND_ABORT_EN $ucos_handle] == true)?"1":"0"]


    set_define "./src/os_cfg.h" "OS_CFG_MEM_EN" [expr ([get_property CONFIG.OS_CFG_MEM_EN $ucos_handle] == true)?"1":"0"]


    set_define "./src/os_cfg.h" "OS_CFG_MUTEX_EN"            [expr ([get_property CONFIG.OS_CFG_MUTEX_EN            $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_MUTEX_DEL_EN"        [expr ([get_property CONFIG.OS_CFG_MUTEX_DEL_EN        $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_MUTEX_PEND_ABORT_EN" [expr ([get_property CONFIG.OS_CFG_MUTEX_PEND_ABORT_EN $ucos_handle] == true)?"1":"0"]


    set_define "./src/os_cfg.h" "OS_CFG_Q_EN"            [expr ([get_property CONFIG.OS_CFG_Q_EN            $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_Q_DEL_EN"        [expr ([get_property CONFIG.OS_CFG_Q_DEL_EN        $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_Q_FLUSH_EN"      [expr ([get_property CONFIG.OS_CFG_Q_FLUSH_EN      $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_Q_PEND_ABORT_EN" [expr ([get_property CONFIG.OS_CFG_Q_PEND_ABORT_EN $ucos_handle] == true)?"1":"0"]


    set_define "./src/os_cfg.h" "OS_CFG_SEM_EN"            [expr ([get_property CONFIG.OS_CFG_SEM_EN            $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_SEM_DEL_EN"        [expr ([get_property CONFIG.OS_CFG_SEM_DEL_EN        $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_SEM_PEND_ABORT_EN" [expr ([get_property CONFIG.OS_CFG_SEM_PEND_ABORT_EN $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_SEM_SET_EN"        [expr ([get_property CONFIG.OS_CFG_SEM_SET_EN        $ucos_handle] == true)?"1":"0"]

    set_define "./src/os_cfg.h" "OS_CFG_STAT_TASK_EN"         [expr ([get_property CONFIG.OS_CFG_STAT_TASK_EN         $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_STAT_TASK_STK_CHK_EN" [expr ([get_property CONFIG.OS_CFG_STAT_TASK_STK_CHK_EN $ucos_handle] == true)?"1":"0"]

    set_define "./src/os_cfg.h" "OS_CFG_TASK_CHANGE_PRIO_EN"    [expr ([get_property CONFIG.OS_CFG_TASK_CHANGE_PRIO_EN     $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_TASK_DEL_EN"            [expr ([get_property CONFIG.OS_CFG_TASK_DEL_EN             $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_TASK_IDLE_EN"           [expr ([get_property CONFIG.OS_CFG_TASK_IDLE_EN            $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_TASK_Q_EN"              [expr ([get_property CONFIG.OS_CFG_TASK_Q_EN               $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_TASK_Q_PEND_ABORT_EN"   [expr ([get_property CONFIG.OS_CFG_TASK_Q_PEND_ABORT_EN    $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_TASK_PROFILE_EN"        [expr ([get_property CONFIG.OS_CFG_TASK_PROFILE_EN         $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_TASK_REG_TBL_SIZE"      [format "%u" [get_property CONFIG.OS_CFG_TASK_REG_TBL_SIZE $ucos_handle]]
    set_define "./src/os_cfg.h" "OS_CFG_TASK_STK_REDZONE_EN"    [expr ([get_property CONFIG.OS_CFG_TASK_STK_REDZONE_EN     $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_TASK_STK_REDZONE_DEPTH" [format "%u" [get_property CONFIG.OS_CFG_TASK_STK_REDZONE_DEPTH $ucos_handle]]
    set_define "./src/os_cfg.h" "OS_CFG_TASK_SEM_PEND_ABORT_EN" [expr ([get_property CONFIG.OS_CFG_TASK_SEM_PEND_ABORT_EN  $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_TASK_SUSPEND_EN"        [expr ([get_property CONFIG.OS_CFG_TASK_SUSPEND_EN         $ucos_handle] == true)?"1":"0"]


    set_define "./src/os_cfg.h" "OS_CFG_TIME_DLY_HMSM_EN"   [expr ([get_property CONFIG.OS_CFG_TIME_DLY_HMSM_EN   $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_TIME_DLY_RESUME_EN" [expr ([get_property CONFIG.OS_CFG_TIME_DLY_RESUME_EN $ucos_handle] == true)?"1":"0"]


    set_define "./src/os_cfg.h" "OS_CFG_TLS_TBL_SIZE" [format "%u" [get_property CONFIG.OS_CFG_TLS_TBL_SIZE $ucos_handle]]


    set_define "./src/os_cfg.h" "OS_CFG_TMR_EN"     [expr ([get_property CONFIG.OS_CFG_TMR_EN     $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_TMR_DEL_EN" [expr ([get_property CONFIG.OS_CFG_TMR_DEL_EN $ucos_handle] == true)?"1":"0"]


    set_define "./src/os_cfg.h" "OS_CFG_TRACE_EN" [expr ([get_property CONFIG.OS_CFG_TRACE_EN                     $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_TRACE_API_ENTER_EN" [expr ([get_property CONFIG.OS_CFG_TRACE_API_ENTER_EN $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CFG_TRACE_API_EXIT_EN" [expr ([get_property CONFIG.OS_CFG_TRACE_API_EXIT_EN   $ucos_handle] == true)?"1":"0"]



    set_define "./src/os_cfg_app.h" "OS_CFG_ISR_STK_SIZE" [format "%u" [get_property CONFIG.OS_CFG_ISR_STK_SIZE $ucos_handle]]

    set_define "./src/os_cfg_app.h" "OS_CFG_MSG_POOL_SIZE" [format "%u" [get_property CONFIG.OS_CFG_MSG_POOL_SIZE $ucos_handle]]

    set_define "./src/os_cfg_app.h" "OS_CFG_TASK_STK_LIMIT_PCT_EMPTY" [format "%u" [get_property CONFIG.OS_CFG_TASK_STK_LIMIT_PCT_EMPTY $ucos_handle]]

    set_define "./src/os_cfg_app.h" "OS_CFG_IDLE_TASK_STK_SIZE" [format "%u" [get_property CONFIG.OS_CFG_IDLE_TASK_STK_SIZE $ucos_handle]]

    set_define "./src/os_cfg_app.h" "OS_CFG_STAT_TASK_PRIO"     [format "%u" [get_property CONFIG.OS_CFG_STAT_TASK_PRIO     $ucos_handle]]
    set_define "./src/os_cfg_app.h" "OS_CFG_STAT_TASK_RATE_HZ"  [format "%u" [get_property CONFIG.OS_CFG_STAT_TASK_RATE_HZ  $ucos_handle]]
    set_define "./src/os_cfg_app.h" "OS_CFG_STAT_TASK_STK_SIZE" [format "%u" [get_property CONFIG.OS_CFG_STAT_TASK_STK_SIZE $ucos_handle]]

    set_define "./src/os_cfg_app.h" "OS_CFG_TICK_RATE_HZ"       [format "%u" [get_property CONFIG.OS_CFG_TICK_RATE_HZ       $ucos_handle]]

    set_define "./src/os_cfg_app.h" "OS_CFG_TMR_TASK_PRIO"     [format "%u" [get_property CONFIG.OS_CFG_TMR_TASK_PRIO $ucos_handle]]
    set_define "./src/os_cfg_app.h" "OS_CFG_TMR_TASK_RATE_HZ"  [format "%u" [get_property CONFIG.OS_CFG_TMR_TASK_RATE_HZ $ucos_handle]]
    set_define "./src/os_cfg_app.h" "OS_CFG_TMR_TASK_STK_SIZE" [format "%u" [get_property CONFIG.OS_CFG_TMR_TASK_STK_SIZE $ucos_handle]]
}
