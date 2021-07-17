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

    ucos_log_put "TRACE" "Generating ucos_common."

    set lib_osiii  [get_libs -filter "NAME==ucos_osiii"]
    set lib_osii   [get_libs -filter "NAME==ucos_osii"]

    set clk_enabled [get_property CONFIG.CLK_ENABLED $ucos_handle]
    if {$clk_enabled == "auto"} {
        if {[llength [get_libs -filter "NAME==ucos_fs"]] != 0} {
            set clk_enabled "true"
        } else {
            set clk_enabled "false"
        }
    }

    set shell_enabled [get_property CONFIG.SHELL_ENABLED $ucos_handle]
    if {$shell_enabled == "auto"} {
        if {[llength [get_libs -filter "NAME==ucos_telnet-s"]] != 0} {
            set shell_enabled "true"
        } else {
            set shell_enabled "false"
        }
    }

    set crc_enabled [get_property CONFIG.CRC_ENABLED $ucos_handle]
    if {$crc_enabled == "auto"} {
        if {[llength [get_libs -filter "NAME==ucos_fs"]] != 0} {
            set crc_enabled "true"
        } else {
            set crc_enabled "false"
        }
    }

    set ucos_src_base [ucos_get_source_base]


    file copy -force "$ucos_src_base/uC-CPU/Cfg/Template/cpu_cfg.h" "./src"
    file copy -force "$ucos_src_base/uC-LIB/Cfg/Template/lib_cfg.h" "./src"

    if {$clk_enabled == "true"} {
        file copy -force "$ucos_src_base/uC-Clk/Cfg/Template/clk_cfg.h" "./src"
    }

    if {$crc_enabled == "true"} {
        file copy -force "$ucos_src_base/uC-CRC/Cfg/Template/crc_cfg.h" "./src"
    }

    if {$shell_enabled == "true"} {
        file copy -force "$ucos_src_base/uC-Shell/Cfg/Template/shell_cfg.h" "./src"
    }


    file copy -force "$ucos_src_base/uC-CPU" "./src/uC-CPU"
    file copy -force "$ucos_src_base/uC-LIB" "./src/uC-LIB"

    if {$clk_enabled == "true"} {
        file copy -force "$ucos_src_base/uC-Clk" "./src/uC-Clk"
    }

    if {$crc_enabled == "true"} {
        file copy -force "$ucos_src_base/uC-CRC" "./src/uC-CRC"
    }
    file copy -force "$ucos_src_base/uC-Common" "./src/uC-Common"

    if {$shell_enabled == "true"} {
        file copy -force "$ucos_src_base/uC-Shell" "./src/uC-Shell"
    }

    file mkdir "../../include/KAL"
    file mkdir "../../include/Source"
    file mkdir "../../include/Collections"

    set sw_proc_handle [get_sw_processor]
    set hw_proc_handle [get_cells [get_property HW_INSTANCE $sw_proc_handle] ]

    set proctype [get_property IP_NAME $hw_proc_handle]

    switch $proctype {
    "microblaze" {
        file copy -force "./src/mb/Makefile.inc" "./src/Makefile.inc"
    }

    "ps7_cortexa9" {
        file copy -force "./src/ps7/Makefile.inc" "./src/Makefile.inc"

        set file_handle [open "./src/cpu_cfg.h" a]
        puts $file_handle "#define  CPU_CACHE_CFG_L2C310_BASE_ADDR  0xF8F02000"
        close $file_handle
    }

    "psu_cortexa53" {
        file copy -force "./src/psua53/Makefile.inc" "./src/Makefile.inc"
    }

    "psu_cortexr5" {
        file copy -force "./src/psur5/Makefile.inc" "./src/Makefile.inc"
    }

    "default" {error "Unknown processor type $proctype"}
    }

    # Copy kernel specific port
    file mkdir "./src/uC-Common/KAL/kal"
    if { [llength $lib_osiii] } {
        file copy -force "$ucos_src_base/uC-Common/KAL/uCOS-III/kal.c" "./src/uC-Common/KAL/kal"
    }

    if { [llength $lib_osii] } {
        file copy -force "$ucos_src_base/uC-Common/KAL/uCOS-II/kal.c" "./src/uC-Common/KAL/kal"
    }

    set_define "./src/cpu_cfg.h" "CPU_CFG_NAME_EN"                [expr ([get_property CONFIG.CPU_CFG_NAME_EN                 $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/cpu_cfg.h" "CPU_CFG_NAME_SIZE"              [format "%u" [get_property CONFIG.CPU_CFG_NAME_SIZE         $ucos_handle]]
    set_define "./src/cpu_cfg.h" "CPU_CFG_TS_32_EN"               [expr ([get_property CONFIG.CPU_CFG_TS_32_EN                $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/cpu_cfg.h" "CPU_CFG_TS_64_EN"               [expr ([get_property CONFIG.CPU_CFG_TS_64_EN                $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]

    switch $proctype {
        "microblaze" {
            set_define "./src/cpu_cfg.h" "CPU_CFG_TS_TMR_SIZE" "CPU_WORD_SIZE_64"
        }

        "ps7_cortexa9" {
            set_define "./src/cpu_cfg.h" "CPU_CFG_TS_TMR_SIZE" "CPU_WORD_SIZE_64"
        }

        "psu_cortexa53" {
            set_define "./src/cpu_cfg.h" "CPU_CFG_TS_TMR_SIZE" "CPU_WORD_SIZE_64"
        }

        "psu_cortexr5" {
            set_define "./src/cpu_cfg.h" "CPU_CFG_TS_TMR_SIZE" "CPU_WORD_SIZE_64"
        }

        "default" {error "Unknown processor type $proctype"}
    }


    set_define "./src/lib_cfg.h" "LIB_MEM_CFG_ARG_CHK_EXT_EN"     [expr ([get_property CONFIG.LIB_MEM_CFG_ARG_CHK_EXT_EN       $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
    set_define "./src/lib_cfg.h" "LIB_MEM_CFG_DBG_INFO_EN"        [expr ([get_property CONFIG.LIB_MEM_CFG_DBG_INFO_EN          $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]

    set heap_auto_conf [get_property CONFIG.LIB_MEM_CFG_HEAP_AUTO      $ucos_handle]
    if {$heap_auto_conf == true} {
        set heap_size [ucos_heap_estimate]
    } else {
        set heap_size [format "%u" [get_property CONFIG.LIB_MEM_CFG_HEAP_SIZE      $ucos_handle]]
        if {$heap_size == 0} {
            ucos_log_put "TRACE" "Heap has been disabled."
        } else {
            ucos_log_put "TRACE" "Heap size has been set manually: $heap_size."
        }
    }
    set_define "./src/lib_cfg.h" "LIB_MEM_CFG_HEAP_SIZE" $heap_size

    set_define "./src/lib_cfg.h" "LIB_MEM_CFG_HEAP_BASE_ADDR"     [format "%u" [get_property CONFIG.LIB_MEM_CFG_HEAP_BASE_ADDR $ucos_handle]]

    if {$clk_enabled == "true"} {
        set_define "./src/clk_cfg.h" "CLK_CFG_ARG_CHK_EN"             [expr ([get_property CONFIG.CLK_CFG_ARG_CHK_EN              $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
        set_define "./src/clk_cfg.h" "CLK_CFG_STR_CONV_EN"            [expr ([get_property CONFIG.CLK_CFG_STR_CONV_EN             $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
        set_define "./src/clk_cfg.h" "CLK_CFG_NTP_EN"                 [expr ([get_property CONFIG.CLK_CFG_NTP_EN                  $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
        set_define "./src/clk_cfg.h" "CLK_CFG_UNIX_EN"                [expr ([get_property CONFIG.CLK_CFG_UNIX_EN                 $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
        set_define "./src/clk_cfg.h" "CLK_CFG_EXT_EN"                 [expr ([get_property CONFIG.CLK_CFG_EXT_EN                  $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
        set_define "./src/clk_cfg.h" "CLK_CFG_SIGNAL_EN"              [expr ([get_property CONFIG.CLK_CFG_SIGNAL_EN               $ucos_handle] == true)?"DEF_ENABLED":"DEF_DISABLED"]
        set_define "./src/lib_cfg.h" "CLK_CFG_SIGNAL_FREQ_HZ"         [format "%u" [get_property CONFIG.CLK_CFG_SIGNAL_FREQ_HZ    $ucos_handle]]
        set_define "./src/lib_cfg.h" "CLK_CFG_TZ_DFLT_SEC"            [format "%u" [get_property CONFIG.CLK_CFG_TZ_DFLT_SEC       $ucos_handle]]
    }

    if {$shell_enabled == "true"} {
        set_define "./src/shell_cfg.h" "SHELL_CFG_CMD_TBL_SIZE"            [format "%u" [get_property CONFIG.SHELL_CFG_CMD_TBL_SIZE            $ucos_handle]]
        set_define "./src/shell_cfg.h" "SHELL_CFG_CMD_ARG_NBR_MAX"         [format "%u" [get_property CONFIG.SHELL_CFG_CMD_ARG_NBR_MAX         $ucos_handle]]
        set_define "./src/shell_cfg.h" "SHELL_CFG_MODULE_CMD_NAME_LEN_MAX" [format "%u" [get_property CONFIG.SHELL_CFG_MODULE_CMD_NAME_LEN_MAX $ucos_handle]]
    }

    # Handle cache management configuration
    set cache_mgmt [get_property CONFIG.CPU_CFG_CACHE_MGMT_EN $ucos_handle]
    switch $cache_mgmt {
        "true" {
            set_define "./src/cpu_cfg.h" "CPU_CFG_CACHE_MGMT_EN" "DEF_ENABLED"
        }
        "false" {
            set_define "./src/cpu_cfg.h" "CPU_CFG_CACHE_MGMT_EN" "DEF_DISABLED"
        }
        "auto" { # Cache management for the MicroBlaze is configured a little later in this script, disable for now.
            if {$proctype == "ps7_cortexa9"} {
                ucos_log_put "TRACE" "ps7_cortexa9 found, enabling CPU_CFG_CACHE_MGMT_EN in cpu_cfg.h."
                set_define "./src/cpu_cfg.h" "CPU_CFG_CACHE_MGMT_EN" "DEF_ENABLED"
            } else {
                set_define "./src/cpu_cfg.h" "CPU_CFG_CACHE_MGMT_EN" "DEF_DISABLED"
            }
        }
    }

    set file_handle [open "./src/Makefile_common_def.inc" w]

    if {$clk_enabled == "true"} {
        puts $file_handle "UCOS_CLK_EN = \"TRUE\""
    } else {
        puts $file_handle "UCOS_CLK_EN = \"FALSE\""
    }

    if {$shell_enabled == "true"} {
        puts $file_handle "UCOS_SHELL_EN = \"TRUE\""
    } else {
        puts $file_handle "UCOS_SHELL_EN = \"FALSE\""
    }

    if {$crc_enabled == "true"} {
        puts $file_handle "UCOS_CRC_EN = \"TRUE\""
    } else {
        puts $file_handle "UCOS_CRC_EN = \"FALSE\""
    }

    set mb_has_cache [get_property CONFIG.C_USE_DCACHE $hw_proc_handle]
    if {$mb_has_cache == 1 && $cache_mgmt == "auto"} {
        ucos_log_put "TRACE" "MicroBlaze core has data cache, enabling CPU_CFG_CACHE_MGMT_EN in cpu_cfg.h."
        set_define "./src/cpu_cfg.h" "CPU_CFG_CACHE_MGMT_EN" "DEF_ENABLED"
    }

    set mb_has_msr [get_property CONFIG.C_USE_MSR_INSTR $hw_proc_handle]
    if {$mb_has_msr == 1} {
        ucos_log_put "TRACE" "MicroBlaze core has MSR instruction, defining CPU_CFG_MB_HAS_MSR_INST in cpu_cfg.h."
        set cpu_file_handle [open "./src/cpu_cfg.h" a]
        puts $cpu_file_handle "#define  CPU_CFG_MB_HAS_MSR_INST"
        close $cpu_file_handle
    }

    set mb_has_clz [get_property CONFIG.C_USE_PCMP_INSTR $hw_proc_handle]
    if {$mb_has_clz == 1} {
        ucos_log_put "TRACE" "MicroBlaze core has pattern compare instructions, defining CPU_CFG_LEAD_ZEROS_ASM_PRESENT in cpu_cfg.h."
        set cpu_file_handle [open "./src/cpu_cfg.h" a]
        puts $cpu_file_handle "#define  CPU_CFG_LEAD_ZEROS_ASM_PRESENT"
        close $cpu_file_handle
    }

    #handle interrupt disable time measurement
    set int_dis_meas_en [get_property CONFIG.CPU_CFG_INT_DIS_MEAS_EN $ucos_handle]
    if {$int_dis_meas_en == true} {
        ucos_log_put "TRACE" "Interrupt disable time measurement enabled, defining CPU_CFG_INT_DIS_MEAS_EN in cpu_cfg.h."
        set cpu_file_handle [open "./src/cpu_cfg.h" a]
        puts $cpu_file_handle "#define  CPU_CFG_INT_DIS_MEAS_EN"
        close $cpu_file_handle
    }

    close $file_handle

}
