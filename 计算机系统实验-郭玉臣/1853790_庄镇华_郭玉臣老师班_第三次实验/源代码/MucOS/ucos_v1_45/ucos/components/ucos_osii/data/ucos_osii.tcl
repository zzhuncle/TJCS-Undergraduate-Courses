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
    ucos_log_put "TRACE" "Generating ucos_osii."

    set ucos_src_base [ucos_get_source_base]

    file copy -force "$ucos_src_base/uCOS-II/Cfg/Template/os_cfg.h" "./src"

    file copy -force "$ucos_src_base/uCOS-II" "./src/uCOS-II"

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


    set_define "./src/os_cfg.h" "OS_APP_HOOKS_EN"            [expr ([get_property CONFIG.OS_APP_HOOKS_EN             $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_ARG_CHK_EN"              [expr ([get_property CONFIG.OS_ARG_CHK_EN               $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_CPU_HOOKS_EN"            [expr ([get_property CONFIG.OS_CPU_HOOKS_EN             $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_DEBUG_EN"                [expr ([get_property CONFIG.OS_DEBUG_EN                 $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_EVENT_MULTI_EN"          [expr ([get_property CONFIG.OS_EVENT_MULTI_EN           $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_EVENT_NAME_EN"           [expr ([get_property CONFIG.OS_EVENT_NAME_EN            $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_LOWEST_PRIO"             [format "%u" [get_property CONFIG.OS_LOWEST_PRIO        $ucos_handle]]
    set_define "./src/os_cfg.h" "OS_MAX_EVENTS"              [format "%u" [get_property CONFIG.OS_MAX_EVENTS         $ucos_handle]]
    set_define "./src/os_cfg.h" "OS_MAX_FLAGS"               [format "%u" [get_property CONFIG.OS_MAX_FLAGS          $ucos_handle]]
    set_define "./src/os_cfg.h" "OS_MAX_MEM_PART"            [format "%u" [get_property CONFIG.OS_MAX_MEM_PART       $ucos_handle]]
    set_define "./src/os_cfg.h" "OS_MAX_QS"                  [format "%u" [get_property CONFIG.OS_MAX_QS             $ucos_handle]]
    set_define "./src/os_cfg.h" "OS_MAX_TASKS"               [format "%u" [get_property CONFIG.OS_MAX_TASKS          $ucos_handle]]
    set_define "./src/os_cfg.h" "OS_SCHED_LOCK_EN"           [expr ([get_property CONFIG.OS_SCHED_LOCK_EN            $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_TICK_STEP_EN"            [expr ([get_property CONFIG.OS_TICK_STEP_EN             $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_TICKS_PER_SEC"           [format "%u" [get_property CONFIG.OS_TICKS_PER_SEC      $ucos_handle]]
    set_define "./src/os_cfg.h" "OS_TLS_TBL_SIZE"            [format "%u" [get_property CONFIG.OS_TLS_TBL_SIZE       $ucos_handle]]

    set_define "./src/os_cfg.h" "OS_TASK_TMR_STK_SIZE"       [format "%u" [get_property CONFIG.OS_TASK_TMR_STK_SIZE  $ucos_handle]]
    set_define "./src/os_cfg.h" "OS_TASK_STAT_STK_SIZE"      [format "%u" [get_property CONFIG.OS_TASK_STAT_STK_SIZE $ucos_handle]]
    set_define "./src/os_cfg.h" "OS_TASK_IDLE_STK_SIZE"      [format "%u" [get_property CONFIG.OS_TASK_IDLE_STK_SIZE $ucos_handle]]

    set_define "./src/os_cfg.h" "OS_TASK_CHANGE_PRIO_EN"     [expr ([get_property CONFIG.OS_TASK_CHANGE_PRIO_EN      $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_TASK_CREATE_EN"          [expr ([get_property CONFIG.OS_TASK_CREATE_EN           $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_TASK_CREATE_EXT_EN"      [expr ([get_property CONFIG.OS_TASK_CREATE_EXT_EN       $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_TASK_DEL_EN"             [expr ([get_property CONFIG.OS_TASK_DEL_EN              $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_TASK_NAME_EN"            [expr ([get_property CONFIG.OS_TASK_NAME_EN             $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_TASK_PROFILE_EN"         [expr ([get_property CONFIG.OS_TASK_PROFILE_EN          $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_TASK_QUERY_EN"           [expr ([get_property CONFIG.OS_TASK_QUERY_EN            $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_TASK_REG_TBL_SIZE"       [format "%u" [get_property CONFIG.OS_TASK_REG_TBL_SIZE  $ucos_handle]]
    set_define "./src/os_cfg.h" "OS_TASK_STAT_EN"            [expr ([get_property CONFIG.OS_TASK_STAT_EN             $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_TASK_STAT_STK_CHK_EN"    [expr ([get_property CONFIG.OS_TASK_STAT_STK_CHK_EN     $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_TASK_SUSPEND_EN"         [expr ([get_property CONFIG.OS_TASK_SUSPEND_EN          $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_TASK_SW_HOOK_EN"         [expr ([get_property CONFIG.OS_TASK_SW_HOOK_EN          $ucos_handle] == true)?"1":"0"]

    set_define "./src/os_cfg.h" "OS_FLAG_EN"                 [expr ([get_property CONFIG.OS_FLAG_EN                  $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_FLAG_ACCEPT_EN"          [expr ([get_property CONFIG.OS_FLAG_ACCEPT_EN           $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_FLAG_DEL_EN"             [expr ([get_property CONFIG.OS_FLAG_DEL_EN              $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_FLAG_NAME_EN"            [expr ([get_property CONFIG.OS_FLAG_NAME_EN             $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_FLAG_QUERY_EN"           [expr ([get_property CONFIG.OS_FLAG_QUERY_EN            $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_FLAG_WAIT_CLR_EN"        [expr ([get_property CONFIG.OS_FLAG_WAIT_CLR_EN         $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_FLAGS_NBITS"             [format "%u" [get_property CONFIG.OS_FLAGS_NBITS        $ucos_handle]]

    set_define "./src/os_cfg.h" "OS_MBOX_EN"                 [expr ([get_property CONFIG.OS_MBOX_EN                  $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_MBOX_ACCEPT_EN"          [expr ([get_property CONFIG.OS_MBOX_ACCEPT_EN           $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_MBOX_DEL_EN"             [expr ([get_property CONFIG.OS_MBOX_DEL_EN              $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_MBOX_PEND_ABORT_EN"      [expr ([get_property CONFIG.OS_MBOX_PEND_ABORT_EN       $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_MBOX_POST_EN"            [expr ([get_property CONFIG.OS_MBOX_POST_EN             $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_MBOX_POST_OPT_EN"        [expr ([get_property CONFIG.OS_MBOX_POST_OPT_EN         $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_MBOX_QUERY_EN"           [expr ([get_property CONFIG.OS_MBOX_QUERY_EN            $ucos_handle] == true)?"1":"0"]

    set_define "./src/os_cfg.h" "OS_MEM_EN"                  [expr ([get_property CONFIG.OS_MEM_EN                   $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_MEM_NAME_EN"             [expr ([get_property CONFIG.OS_MEM_NAME_EN              $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_MEM_QUERY_EN"            [expr ([get_property CONFIG.OS_MEM_QUERY_EN             $ucos_handle] == true)?"1":"0"]

    set_define "./src/os_cfg.h" "OS_MUTEX_EN"                [expr ([get_property CONFIG.OS_MUTEX_EN                 $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_MUTEX_ACCEPT_EN"         [expr ([get_property CONFIG.OS_MUTEX_ACCEPT_EN          $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_MUTEX_DEL_EN"            [expr ([get_property CONFIG.OS_MUTEX_DEL_EN             $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_MUTEX_QUERY_EN"          [expr ([get_property CONFIG.OS_MUTEX_QUERY_EN           $ucos_handle] == true)?"1":"0"]

    set_define "./src/os_cfg.h" "OS_Q_EN"                    [expr ([get_property CONFIG.OS_Q_EN                     $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_Q_ACCEPT_EN"             [expr ([get_property CONFIG.OS_Q_ACCEPT_EN              $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_Q_DEL_EN"                [expr ([get_property CONFIG.OS_Q_DEL_EN                 $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_Q_FLUSH_EN"              [expr ([get_property CONFIG.OS_Q_FLUSH_EN               $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_Q_PEND_ABORT_EN"         [expr ([get_property CONFIG.OS_Q_PEND_ABORT_EN          $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_Q_POST_EN"               [expr ([get_property CONFIG.OS_Q_POST_EN                $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_Q_POST_FRONT_EN"         [expr ([get_property CONFIG.OS_Q_POST_FRONT_EN          $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_Q_POST_OPT_EN"           [expr ([get_property CONFIG.OS_Q_POST_OPT_EN            $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_Q_QUERY_EN"              [expr ([get_property CONFIG.OS_Q_QUERY_EN               $ucos_handle] == true)?"1":"0"]

    set_define "./src/os_cfg.h" "OS_SEM_EN"                  [expr ([get_property CONFIG.OS_SEM_EN                   $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_SEM_ACCEPT_EN"           [expr ([get_property CONFIG.OS_SEM_ACCEPT_EN            $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_SEM_DEL_EN"              [expr ([get_property CONFIG.OS_SEM_DEL_EN               $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_SEM_PEND_ABORT_EN"       [expr ([get_property CONFIG.OS_SEM_PEND_ABORT_EN        $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_SEM_QUERY_EN"            [expr ([get_property CONFIG.OS_SEM_QUERY_EN             $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_SEM_SET_EN"              [expr ([get_property CONFIG.OS_SEM_SET_EN               $ucos_handle] == true)?"1":"0"]

    set_define "./src/os_cfg.h" "OS_TIME_DLY_HMSM_EN"        [expr ([get_property CONFIG.OS_TIME_DLY_HMSM_EN         $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_TIME_DLY_RESUME_EN"      [expr ([get_property CONFIG.OS_TIME_DLY_RESUME_EN       $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_TIME_GET_SET_EN"         [expr ([get_property CONFIG.OS_TIME_GET_SET_EN          $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_TIME_TICK_HOOK_EN"       [expr ([get_property CONFIG.OS_TIME_TICK_HOOK_EN        $ucos_handle] == true)?"1":"0"]

    set_define "./src/os_cfg.h" "OS_TMR_EN"                  [expr ([get_property CONFIG.OS_TMR_EN                      $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_TMR_CFG_MAX"             [format "%u" [get_property CONFIG.OS_TMR_CFG_MAX           $ucos_handle]]
    set_define "./src/os_cfg.h" "OS_TMR_CFG_NAME_EN"         [expr ([get_property CONFIG.OS_TMR_CFG_NAME_EN             $ucos_handle] == true)?"1":"0"]
    set_define "./src/os_cfg.h" "OS_TMR_CFG_WHEEL_SIZE"      [format "%u" [get_property CONFIG.OS_TMR_CFG_WHEEL_SIZE    $ucos_handle]]
    set_define "./src/os_cfg.h" "OS_TMR_CFG_TICKS_PER_SEC"   [format "%u" [get_property CONFIG.OS_TMR_CFG_TICKS_PER_SEC $ucos_handle]]
}
