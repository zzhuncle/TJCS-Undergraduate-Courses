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

namespace import ::hsi::utils::*

source "version.tcl"
source "./ucos_tcl_utils.tcl"

proc ucos_drc {os_handle} {
    set lib_common [get_libs -filter "NAME==ucos_common"];
    set lib_osiii [get_libs -filter "NAME==ucos_osiii"];
    set lib_osii [get_libs -filter "NAME==ucos_osii"];
    set lib_standalone [get_libs -filter "NAME==ucos_standalone"];

    if { [llength $lib_osiii] != 0 && [llength $lib_osii] != 0 } {
        error "Only one kernel must be selected."
    }
}


proc generate {os_handle} {

    ucos_log_put "TRACE" "Generating ucos."

    set lib_common [get_libs -filter "NAME==ucos_common"];
    set lib_osiii  [get_libs -filter "NAME==ucos_osiii"];
    set lib_osii   [get_libs -filter "NAME==ucos_osii"];
    set lib_standalone [get_libs -filter "NAME==ucos_standalone"];
    set lib_tcpip   [get_libs -filter "NAME==ucos_tcpip"];
    set lib_dhcpc   [get_libs -filter "NAME==ucos_dhcp-c"];
    set lib_dnsc   [get_libs -filter "NAME==ucos_dns-c"];
    set lib_httpc   [get_libs -filter "NAME==ucos_http-c"];
    set lib_mqttc   [get_libs -filter "NAME==ucos_mqtt-c"];
    set lib_telnets   [get_libs -filter "NAME==ucos_telnet-s"];
    set lib_iperf   [get_libs -filter "NAME==ucos_iperf"];
    set lib_fs   [get_libs -filter "NAME==ucos_fs"];
    set lib_usbd   [get_libs -filter "NAME==ucos_usbd"];
    set lib_usbh   [get_libs -filter "NAME==ucos_usbh"];
    set lib_openamp   [get_libs -filter "NAME==ucos_openamp"];


    if {[llength $lib_common]} {
        set clk_enabled [get_property CONFIG.CLK_ENABLED $lib_common]
    } else {
        set clk_enabled "false"
    }
    if {$clk_enabled == "auto"} {
        ucos_log_put "INFO" "ucos_clk automatic selection."
        if {[llength [get_libs -filter "NAME==ucos_fs"]] != 0} {
            ucos_log_put "INFO" "ucos_clk required."
            set clk_enabled "true"
        } else {
            ucos_log_put "INFO" "ucos_clk not required."
            set clk_enabled "false"
        }
    }

    if {[llength $lib_common]} {
        set shell_enabled [get_property CONFIG.SHELL_ENABLED $lib_common]
    } else {
        set shell_enabled "false"
    }
    if {$shell_enabled == "auto"} {
        ucos_log_put "INFO" "ucos_shell automatic selection."
        if {[llength [get_libs -filter "NAME==ucos_telnet-s"]] != 0} {
            ucos_log_put "INFO" "ucos_shell required."
            set shell_enabled "true"
        } else {
            ucos_log_put "INFO" "ucos_shell not required."
            set shell_enabled "false"
        }
    }

    if {[llength $lib_common]} {
        set crc_enabled [get_property CONFIG.CRC_ENABLED $lib_common]
    } else {
        set crc_enabled "false"
    }
    if {$crc_enabled == "auto"} {
        ucos_log_put "INFO" "ucos_crc automatic selection."
        if {[llength [get_libs -filter "NAME==ucos_fs"]] != 0} {
            ucos_log_put "INFO" "ucos_crc required."
            set crc_enabled "true"
        } else {
            ucos_log_put "INFO" "ucos_crc not required."
            set crc_enabled "false"
        }
    }


    if { [llength $lib_common] } {
        set_define "./src/app_cfg.h" "APP_CPU_ENABLED" "DEF_ENABLED"
        set_define "./src/app_cfg.h" "APP_LIB_ENABLED" "DEF_ENABLED"
        set_define "./src/app_cfg.h" "APP_COMMON_ENABLED" "DEF_ENABLED"
        if { $shell_enabled == "true" } {
            set_define "./src/app_cfg.h" "APP_SHELL_ENABLED" "DEF_ENABLED"
        }
    }

    if { [llength $lib_osiii] } {
        ucos_log_put "INFO" "ucos_osiii set to enabled."
        set_define "./src/app_cfg.h" "APP_OSIII_ENABLED" "DEF_ENABLED"
    }

    if { [llength $lib_osii] } {
        ucos_log_put "INFO" "ucos_osii set to enabled."
        set_define "./src/app_cfg.h" "APP_OSII_ENABLED" "DEF_ENABLED"
    }

    if { [llength $lib_tcpip] } {
        ucos_log_put "INFO" "ucos_tcpip set to enabled."
        set_define "./src/app_cfg.h" "APP_TCPIP_ENABLED" "DEF_ENABLED"

        set src_base [ucos_get_product_source_base tcpip-exp]
        if {$src_base != ""} {
            set_define "./src/app_cfg.h" "APP_TCPIP_EXP_ENABLED" "DEF_ENABLED"
        }
    }

    if { [llength $lib_dhcpc] } {
        ucos_log_put "INFO" "ucos_dhcp set to enabled."
        set_define "./src/app_cfg.h" "APP_DHCPC_ENABLED" "DEF_ENABLED"
    }

    if { [llength $lib_dnsc] } {
        ucos_log_put "INFO" "ucos_dnsc set to enabled."
        set_define "./src/app_cfg.h" "APP_DNSC_ENABLED" "DEF_ENABLED"
    }

    if { [llength $lib_httpc] } {
        ucos_log_put "INFO" "ucos_httpc set to enabled."
        set_define "./src/app_cfg.h" "APP_HTTPC_ENABLED" "DEF_ENABLED"
    }

    if { [llength $lib_mqttc] } {
        ucos_log_put "INFO" "ucos_mqttc set to enabled."
        set_define "./src/app_cfg.h" "APP_MQTTC_ENABLED" "DEF_ENABLED"
    }

    if { [llength $lib_telnets] } {
        ucos_log_put "INFO" "ucos_telnets set to enabled."
        set_define "./src/app_cfg.h" "APP_TELNETS_ENABLED" "DEF_ENABLED"
    }

    if { [llength $lib_iperf] } {
        ucos_log_put "INFO" "ucos_iperf set to enabled."
        set_define "./src/app_cfg.h" "APP_IPERF_ENABLED" "DEF_ENABLED"
    }

    if { [llength $lib_fs] } {
        ucos_log_put "INFO" "ucos_fs set to enabled."
        set_define "./src/app_cfg.h" "APP_FS_ENABLED" "DEF_ENABLED"
    }

    if { [llength $lib_usbd] } {
        ucos_log_put "INFO" "ucos_usbd set to enabled."
        set_define "./src/app_cfg.h" "APP_USBD_ENABLED" "DEF_ENABLED"
    }

    if { [llength $lib_usbh] } {
        ucos_log_put "INFO" "ucos_usbh set to enabled."
        set_define "./src/app_cfg.h" "APP_USBH_ENABLED" "DEF_ENABLED"
    }

    if { [llength $lib_openamp] } {
        ucos_log_put "INFO" "ucos_openamp set to enabled."
        set_define "./src/app_cfg.h" "APP_OPENAMP_ENABLED" "DEF_ENABLED"
    }

    set sw_proc_handle [get_sw_processor]
    set hw_proc_handle [get_cells [get_property HW_INSTANCE $sw_proc_handle] ]

    set proctype [get_property IP_NAME $hw_proc_handle]

    switch $proctype {
        "microblaze" {
            ucos_log_put "INFO" "Setting up build for microblaze."
            file copy -force "./src/mb/Makefile.inc" "./src/Makefile.inc"
        }

        "ps7_cortexa9" {
            ucos_log_put "INFO" "Setting up build for ps7_cortexa9."
            file copy -force "./src/ps7/Makefile.inc" "./src/Makefile.inc"
        }

        "psu_cortexa53" {
            ucos_log_put "INFO" "Setting up build for psu_cortexa53."
            file copy -force "./src/psua53/Makefile.inc" "./src/Makefile.inc"
        }

        "psu_cortexr5" {
            ucos_log_put "INFO" "Setting up build for psu_cortexr5."
            file copy -force "./src/psur5/Makefile.inc" "./src/Makefile.inc"
        }

        "default" {error "Unknown processor type $proctype"}
    }

    ucos_handle_makefile_inc $os_handle
    file copy "./src/Makefile_def.inc" "../../include"

    if {$proctype == "microblaze"} {
        handle_mb $os_handle
        handle_systick $os_handle
        handle_ts $os_handle
    }

    if {$proctype == "ps7_cortexa9"} {
        handle_zynq $os_handle
    }

    if {$proctype == "psu_cortexa53"} {
        handle_zynq_psu_a53 $os_handle
    }

    ucos_handle_cpu_type $os_handle

    ucos_handle_stdin $os_handle

    ucos_handle_stdout $os_handle

    ucos_handle_startup $os_handle

    ucos_handle_ethernet $os_handle

    ucos_handle_usb $os_handle

    ucos_handle_ramdisk $os_handle

    ucos_handle_sdcard $os_handle
}

proc ucos_handle_makefile_inc {os_handle} {
    ucos_log_put "TRACE" "Generating Makefile_def.inc."

    set lib_osiii [get_libs -filter "NAME==ucos_osiii"];
    set lib_osii [get_libs -filter "NAME==ucos_osii"];

    set file_handle [open "./src/Makefile_def.inc" w]

    #ARCH
    set sw_proc_handle [get_sw_processor]
    set hw_proc_handle [get_cells [get_property HW_INSTANCE $sw_proc_handle] ]

    set proctype [get_property IP_NAME $hw_proc_handle]
    if {$proctype == "microblaze"} {
        puts $file_handle "UCOS_ARCH = mb"
        puts $file_handle "override ARCHIVER = mb-ar"
    }

    if {$proctype == "ps7_cortexa9"} {
        puts $file_handle "UCOS_ARCH = v7a"
        puts $file_handle "override ARCHIVER = arm-none-eabi-gcc-ar"
    }

    if {$proctype == "psu_cortexa53"} {
        puts $file_handle "UCOS_ARCH = v8a"
        puts $file_handle "override ARCHIVER = aarch64-none-elf-ar"
    }

    if {$proctype == "psu_cortexr5"} {
        puts $file_handle "UCOS_ARCH = v7r"
        puts $file_handle "override ARCHIVER = armr5-none-eabi-ar"
    }

    puts $file_handle "UCOS_SRC_BASE = ./"

    set source_loc [ucos_get_source_base]

    puts $file_handle "UCOS_SRC_LOC = $source_loc"

    set lib_loc [ucos_get_lib_base]

    puts $file_handle "UCOS_LIB_LOC = $lib_loc"

    if { [llength $lib_osii] != 0 } {
        puts $file_handle "UCOS_DIR_NAME = uCOS-II"
        puts $file_handle "UCOS_OS = osii"
    } else {
        puts $file_handle "UCOS_DIR_NAME = uCOS-III"
        puts $file_handle "UCOS_OS = osiii"
    }

    puts $file_handle ""

    #CPU
    set src_base [ucos_get_product_source_base cpu]
    if {$src_base != ""} {
        set src_base [file join $src_base "uC-CPU"]
    }
    puts $file_handle "UCOS_CPU_SRC_BASE = $src_base"

    #LIB
    set src_base [ucos_get_product_source_base lib]
    if {$src_base != ""} {
        set src_base [file join $src_base "uC-LIB"]
    }
    puts $file_handle "UCOS_LIB_SRC_BASE = $src_base"

    #COMMON
    set src_base [ucos_get_product_source_base common]
    if {$src_base != ""} {
        set src_base [file join $src_base "uC-Common"]
    }
    puts $file_handle "UCOS_COMMON_SRC_BASE = $src_base"

    #CLK
    set src_base [ucos_get_product_source_base clk]
    if {$src_base != ""} {
        set src_base [file join $src_base "uC-Clk"]
    }
    puts $file_handle "UCOS_CLK_SRC_BASE = $src_base"

    #CRC
    set src_base [ucos_get_product_source_base crc]
    if {$src_base != ""} {
        set src_base [file join $src_base "uC-CRC"]
    }
    puts $file_handle "UCOS_CRC_SRC_BASE = $src_base"

    #SHELL
    set src_base [ucos_get_product_source_base shell]
    if {$src_base != ""} {
        set src_base [file join $src_base "uC-Shell"]
    }
    puts $file_handle "UCOS_SHELL_SRC_BASE = $src_base"

    #OSII
    set src_base [ucos_get_product_source_base osii]
    if {$src_base != ""} {
        set src_base [file join $src_base "uCOS-II"]
    }
    puts $file_handle "UCOS_OSII_SRC_BASE = $src_base"

    #OSIII
    set src_base [ucos_get_product_source_base osiii]
    if {$src_base != ""} {
        set src_base [file join $src_base "uCOS-III"]
    }
    puts $file_handle "UCOS_OSIII_SRC_BASE = $src_base"

    #TCP
    set src_base [ucos_get_product_source_base tcpip]
    if {$src_base != ""} {
        set src_base [file join $src_base "uC-TCPIP"]
    }
    puts $file_handle "UCOS_TCPIP_SRC_BASE = $src_base"

    set src_base [ucos_get_product_source_base tcpip-exp]
    if {$src_base != ""} {
        set src_base [file join $src_base "uC-TCPIP"]
    }
    puts $file_handle "UCOS_TCPIP_EXP_SRC_BASE = $src_base"

    #FS
    set src_base [ucos_get_product_source_base fs]
    if {$src_base != ""} {
        set src_base [file join $src_base "uC-FS"]
    }
    puts $file_handle "UCOS_FS_SRC_BASE = $src_base"

    #USBD
    set src_base [ucos_get_product_source_base usbd]
    if {$src_base != ""} {
        set src_base [file join $src_base "uC-USB-Device"]
    }
    puts $file_handle "UCOS_USBD_SRC_BASE = $src_base"

    #USBH
    set src_base [ucos_get_product_source_base usbh]
    if {$src_base != ""} {
        set src_base [file join $src_base "uC-USB-Host"]
    }
    puts $file_handle "UCOS_USBH_SRC_BASE = $src_base"

    #DHCPC
    set src_base [ucos_get_product_source_base dhcpc]
    if {$src_base != ""} {
        set src_base [file join $src_base "uC-DHCPc"]
    }
    puts $file_handle "UCOS_DHCPC_SRC_BASE = $src_base"

    #DNSC
    set src_base [ucos_get_product_source_base dnsc]
    if {$src_base != ""} {
        set src_base [file join $src_base "uC-DNSc"]
    }
    puts $file_handle "UCOS_DNSC_SRC_BASE = $src_base"

    #FTPS
    set src_base [ucos_get_product_source_base ftps]
    if {$src_base != ""} {
        set src_base [file join $src_base "uC-FTPs"]
    }
    puts $file_handle "UCOS_FTPS_SRC_BASE = $src_base"

    #HTTPC
    set src_base [ucos_get_product_source_base httpc]
    if {$src_base != ""} {
        set src_base [file join $src_base "uC-HTTP"]
    }
    puts $file_handle "UCOS_HTTPC_SRC_BASE = $src_base"

    #HTTPS
    set src_base [ucos_get_product_source_base https]
    if {$src_base != ""} {
        set src_base [file join $src_base "uC-HTTP"]
    }
    puts $file_handle "UCOS_HTTPS_SRC_BASE = $src_base"

    #MQTTC
    set src_base [ucos_get_product_source_base mqttc]
    if {$src_base != ""} {
        set src_base [file join $src_base "uC-MQTT"]
    }
    puts $file_handle "UCOS_MQTTC_SRC_BASE = $src_base"

    #IPERF
    set src_base [ucos_get_product_source_base iperf]
    if {$src_base != ""} {
        set src_base [file join $src_base "uC-IPerf"]
    }
    puts $file_handle "UCOS_IPERF_SRC_BASE = $src_base"

    #TELNETs
    set src_base [ucos_get_product_source_base telnets]
    if {$src_base != ""} {
        set src_base [file join $src_base "uC-TELNETs"]
    }
    puts $file_handle "UCOS_TELNETS_SRC_BASE = $src_base"

    #CAN
    set src_base [ucos_get_product_source_base can]
    if {$src_base != ""} {
        set src_base [file join $src_base "uC-CAN"]
    }
    puts $file_handle "UCOS_CAN_SRC_BASE = $src_base"

    #OPENAMP
    set src_base [ucos_get_product_source_base openamp]
    if {$src_base != ""} {
        set src_base [file join $src_base "openamp"]
    }
    puts $file_handle "UCOS_OPENAMP_SRC_BASE = $src_base"

    set lib_iperf   [get_libs -filter "NAME==ucos_telnet-s"];
    if { [llength $lib_iperf] } {
        puts $file_handle "UCOS_TCPIP_NETCMD = TRUE"
    }

    set lib_usbh   [get_libs -filter "NAME==ucos_usbh"];
    if { [llength $lib_usbh] } {
        puts $file_handle "UCOS_USBH = TRUE"
    }

    set host [::hsm::utils::get_hostos_platform]

    if {$host == "win" || $host == "win64"} {
        puts $file_handle ""
        puts $file_handle "#Trick to ensure the GNUWin binary of mkdir is called on Windows"
        puts $file_handle "MKDIR = \$(join \$(dir \$(shell which touch)), \"mkdir\")"
    } else {
        puts $file_handle "MKDIR = mkdir"
    }

    close $file_handle
}

proc ucos_handle_cpu_type {os_handle} {
   set sw_proc_handle [get_sw_processor]
   set hw_proc_handle [get_cells [get_property hw_instance $sw_proc_handle]]
   set processor [get_property NAME $hw_proc_handle]

   set config_file [open_include_file "xparameters.h"]

   set proctype [get_property IP_NAME $hw_proc_handle]

    switch $proctype {
        "microblaze" {
            puts $config_file "#define UCOS_CPU_TYPE UCOS_CPU_TYPE_MB"
        }

        "ps7_cortexa9" {
            puts $config_file "#define UCOS_CPU_TYPE UCOS_CPU_TYPE_PS7"
        }

        "psu_cortexa53" {
            puts $config_file "#define UCOS_CPU_TYPE UCOS_CPU_TYPE_PSUA53"
        }

        "psu_cortexr5" {
            puts $config_file "#define UCOS_CPU_TYPE UCOS_CPU_TYPE_PSUR5"
        }

        "default" {error "Unknown processor type $proctype"}
    }
}

proc ucos_handle_startup {os_handle} {
    set config_file [open_include_file "xparameters.h"]

    set start_prio [get_property CONFIG.UCOS_START_TASK_PRIO $os_handle]
    set start_stack [get_property CONFIG.UCOS_START_TASK_STACK_SIZE $os_handle]
    set start_trace [get_property CONFIG.UCOS_DEBUG_TRACE $os_handle]

    puts $config_file "#define UCOS_START_TASK_PRIO $start_prio"
    puts $config_file "#define UCOS_START_TASK_STACK_SIZE $start_stack"

    if {$start_trace == true} {
        puts $config_file "#define UCOS_START_DEBUG_TRACE DEF_ENABLED"
    } else {
        puts $config_file "#define UCOS_START_DEBUG_TRACE DEF_DISABLED"
    }

    set init_can [get_property CONFIG.UCOS_CFG_INIT_CAN $os_handle]
    set init_net [get_property CONFIG.UCOS_CFG_INIT_NET $os_handle]
    set init_fs [get_property CONFIG.UCOS_CFG_INIT_FS $os_handle]
    set init_openamp [get_property CONFIG.UCOS_CFG_INIT_OPENAMP $os_handle]
    set init_usbd [get_property CONFIG.UCOS_CFG_INIT_USBD $os_handle]
    set init_usbh [get_property CONFIG.UCOS_CFG_INIT_USBH $os_handle]

    if {$init_can == true} {
        puts $config_file "#define UCOS_CFG_INIT_CAN DEF_ENABLED"
    } else {
        puts $config_file "#define UCOS_CFG_INIT_CAN DEF_DISABLED"
    }

    if {$init_net == true} {
        puts $config_file "#define UCOS_CFG_INIT_NET DEF_ENABLED"
    } else {
        puts $config_file "#define UCOS_CFG_INIT_NET DEF_DISABLED"
    }

    if {$init_fs == true} {
        puts $config_file "#define UCOS_CFG_INIT_FS DEF_ENABLED"
    } else {
        puts $config_file "#define UCOS_CFG_INIT_FS DEF_DISABLED"
    }

    if {$init_openamp == true} {
        puts $config_file "#define UCOS_CFG_INIT_OPENAMP DEF_ENABLED"
    } else {
        puts $config_file "#define UCOS_CFG_INIT_OPENAMP DEF_DISABLED"
    }

    if {$init_usbd == true} {
        puts $config_file "#define UCOS_CFG_INIT_USBD DEF_ENABLED"
    } else {
        puts $config_file "#define UCOS_CFG_INIT_USBD DEF_DISABLED"
    }

    if {$init_usbh == true} {
        puts $config_file "#define UCOS_CFG_INIT_USBH DEF_ENABLED"
    } else {
        puts $config_file "#define UCOS_CFG_INIT_USBH DEF_DISABLED"
    }
}

proc handle_zynq_psu_a53 {os_handle} {
    set config_file [open_include_file "xparameters.h"]

    set config_mmu [get_property CONFIG.ZYNQ_MPSOC_A53_CONFIG_MMU $os_handle]
    set enable_mmu [get_property CONFIG.ZYNQ_MPSOC_A53_ENABLE_MMU $os_handle]
    set config_caches [get_property CONFIG.ZYNQ_MPSOC_A53_CONFIG_CACHES $os_handle]
    set enable_caches [get_property CONFIG.ZYNQ_MPSOC_A53_ENABLE_CACHES $os_handle]
    set el3_timer [get_property CONFIG.ZYNQ_MPSOC_A53_TICK_SECURE_EL3 $os_handle]

    if {$config_mmu == true} {
        puts $config_file "#define UCOS_ZYNQ_MPSOC_A53_CONFIG_MMU DEF_ENABLED"
    } else {
        puts $config_file "#define UCOS_ZYNQ_MPSOC_A53_CONFIG_MMU DEF_DISABLED"
    }

    if {$enable_mmu == true} {
        puts $config_file "#define UCOS_ZYNQ_MPSOC_A53_ENABLE_MMU DEF_ENABLED"
    } else {
        puts $config_file "#define UCOS_ZYNQ_MPSOC_A53_ENABLE_MMU DEF_DISABLED"
    }

    if {$config_caches == true} {
        puts $config_file "#define UCOS_ZYNQ_MPSOC_A53_CONFIG_CACHES DEF_ENABLED"
    } else {
        puts $config_file "#define UCOS_ZYNQ_MPSOC_A53_CONFIG_CACHES DEF_DISABLED"
    }

    if {$enable_caches == true} {
        puts $config_file "#define UCOS_ZYNQ_MPSOC_A53_ENABLE_CACHES DEF_ENABLED"
    } else {
        puts $config_file "#define UCOS_ZYNQ_MPSOC_A53_ENABLE_CACHES DEF_DISABLED"
    }

    if {$el3_timer == true} {
        puts $config_file "#define UCOS_ZYNQ_MPSOC_A53_TICK_SECURE_EL3 DEF_ENABLED"
    } else {
        puts $config_file "#define UCOS_ZYNQ_MPSOC_A53_TICK_SECURE_EL3 DEF_DISABLED"
    }
}

proc handle_zynq {os_handle} {
    set config_file [open_include_file "xparameters.h"]

    set config_mmu [get_property CONFIG.ZYNQ_CONFIG_MMU $os_handle]
    set enable_mmu [get_property CONFIG.ZYNQ_ENABLE_MMU $os_handle]
    set config_caches [get_property CONFIG.ZYNQ_CONFIG_CACHES $os_handle]
    set enable_caches [get_property CONFIG.ZYNQ_ENABLE_CACHES $os_handle]
    set enable_optims [get_property CONFIG.ZYNQ_ENABLE_PERF_OPTIMS $os_handle]
    set early_icache [get_property CONFIG.ZYNQ_ENABLE_EARLY_L1_I_EN $os_handle]
    set early_dcache [get_property CONFIG.ZYNQ_ENABLE_EARLY_L1_D_EN $os_handle]

    if {$config_mmu == true} {
        puts $config_file "#define UCOS_ZYNQ_CONFIG_MMU DEF_ENABLED"
    } else {
        puts $config_file "#define UCOS_ZYNQ_CONFIG_MMU DEF_DISABLED"
    }

    if {$enable_mmu == true && $config_mmu == true} {
        puts $config_file "#define UCOS_ZYNQ_ENABLE_MMU DEF_ENABLED"
    } else {
        puts $config_file "#define UCOS_ZYNQ_ENABLE_MMU DEF_DISABLED"
    }

    if {$config_caches == true } {
        puts $config_file "#define UCOS_ZYNQ_CONFIG_CACHES DEF_ENABLED"
    } else {
        puts $config_file "#define UCOS_ZYNQ_CONFIG_CACHES DEF_DISABLED"
    }

    if {$enable_caches == true && $config_caches == true && $enable_mmu == true && $config_mmu == true} {
        puts $config_file "#define UCOS_ZYNQ_ENABLE_CACHES DEF_ENABLED"
    } else {
        puts $config_file "#define UCOS_ZYNQ_ENABLE_CACHES DEF_DISABLED"
    }

    if {$enable_optims == true} {
        puts $config_file "#define UCOS_ZYNQ_ENABLE_OPTIMS DEF_ENABLED"
    } else {
        puts $config_file "#define UCOS_ZYNQ_ENABLE_OPTIMS DEF_DISABLED"
    }

    if {$early_icache == true} {
        puts $config_file "#define ZYNQ_ENABLE_EARLY_L1_I_EN DEF_ENABLED"
    } else {
        puts $config_file "#define ZYNQ_ENABLE_EARLY_L1_I_EN DEF_DISABLED"
    }

    if {$early_dcache == true} {
        puts $config_file "#define ZYNQ_ENABLE_EARLY_L1_D_EN DEF_ENABLED"
    } else {
        puts $config_file "#define ZYNQ_ENABLE_EARLY_L1_D_EN DEF_DISABLED"
    }
}


proc handle_mb {os_handle} {
    set sw_proc_handle [get_sw_processor]
    set periph [get_cells [get_property HW_INSTANCE $sw_proc_handle] ]
    set source_ports [get_interrupt_sources $periph]

    set config_file [open_include_file "xparameters.h"]

    if {[llength $source_ports] != 0} {
        set source_periph [get_cells -of_objects $source_ports]

        set source_periph_name [get_property IP_NAME $source_periph]
        ucos_log_put "INFO" "handle_mb: Interrupt source peripheral $source_periph_name."
        if {$source_periph_name == "axi_intc"} {
            ucos_log_put "INFO" "handle_mb: Interrupt handling source set to UCOS_MB_INTC_TYPE_AXIINTC."
            puts $config_file "#define UCOS_MB_INTC_TYPE UCOS_MB_INTC_TYPE_AXIINTC"
        } else {
            ucos_log_put "INFO" "handle_mb: Interrupt handling source set to UCOS_MB_INTC_TYPE_RAW."
            puts $config_file "#define UCOS_MB_INTC_TYPE UCOS_MB_INTC_TYPE_RAW"
        }

    } else {
        puts $config_file "#define UCOS_MB_INTC_TYPE UCOS_MB_INTC_TYPE_NONE"
    }

    puts $config_file "#define XPAR_CPU_ID 0"

}


proc handle_systick {os_handle} {

    set sys_tick [get_property CONFIG.MB_KERNEL_TICK_SRC $os_handle]
    set int_src [get_property CONFIG.MB_KERNEL_TICK_INT_SRC $os_handle]

    set config_file [open_include_file "xparameters.h"]

    ucos_log_put "INFO" "handle_systick: Configuring system tick source as peripheral $sys_tick."
    if {[string compare -nocase "none" $sys_tick] != 0 && [llength $sys_tick] != 0} {
        set drv_handle [get_drivers -filter "HW_INSTANCE==$sys_tick"]

        if {[llength $drv_handle] == 0} {
            error "No driver for systick peripheral $drv_handle. $drv_handle may not be accessible from the current processor."
            return
        }

        set systick_driver [get_property NAME $drv_handle]

        ucos_log_put "INFO" "handle_systick: Found driver $systick_driver for peripheral $sys_tick."

        #Find device ID
        set device_id 0
        set device_found false
        set periphs [get_common_driver_ips $drv_handle]
        foreach periph $periphs {
            if {[string compare -nocase $periph $sys_tick] == 0} {
                set device_found true
                break
            }
            incr device_id
        }

        if {$device_found != true} {
            error "Failed to find peripheral instance for device $sys_tick"
            return
        }

        ucos_log_put "INFO" "handle_systick: Found instance number $device_id for peripheral $sys_tick."

        switch $systick_driver {
            "tmrctr" {
                ucos_log_put "INFO" "handle_systick: System tick source set to UCOS_SYSTICK_AXITIMER."
                puts $config_file "#define UCOS_SYSTICK_DRIVER UCOS_SYSTICK_AXITIMER"
                puts $config_file "#define UCOS_SYSTICK_DEVICE_ID $device_id"
                puts $config_file "#define UCOS_SYSTICK_DEVICE_INT_SRC 0"
                set systick_config true
            }

            "intc" {
                ucos_log_put "INFO" "handle_systick: System tick source set to UCOS_SYSTICK_INT."
                puts $config_file "#define UCOS_SYSTICK_DRIVER UCOS_SYSTICK_INT"
                puts $config_file "#define UCOS_SYSTICK_DEVICE_ID $device_id"
                puts $config_file "#define UCOS_SYSTICK_DEVICE_INT_SRC $int_src"
                set systick_config true
            }

            "default" {
                ucos_log_put "INFO" "handle_systick: System tick source set to UCOS_SYSTICK_NONE."
                puts $config_file "#define UCOS_SYSTICK_DRIVER UCOS_SYSTICK_NONE"
                puts $config_file "#define UCOS_SYSTICK_DEVICE_ID 0"
                puts $config_file "#define UCOS_SYSTICK_DEVICE_INT_SRC 0"
                set systick_config false
            }
        }

    } else {
        ucos_log_put "INFO" "handle_systick: No systick source specified."

        puts $config_file "#define UCOS_SYSTICK_DRIVER UCOS_SYSTICK_NONE"
        puts $config_file "#define UCOS_SYSTICK_DEVICE_ID 0"
        puts $config_file "#define UCOS_SYSTICK_DEVICE_INT_SRC 0"
    }
}

proc handle_ts {os_handle} {
    set ts_timer [get_property CONFIG.MB_KERNEL_TS_SRC $os_handle]

    set config_file [open_include_file "xparameters.h"]

    ucos_log_put "INFO" "handle_ts: Configuring system timestamp source as peripheral $ts_timer."
    if {[string compare -nocase "none" $ts_timer] != 0 && [llength $ts_timer] != 0} {
        set drv_handle [get_drivers -filter "HW_INSTANCE==$ts_timer"]

        if {[llength $drv_handle] == 0} {
            error "No driver for ts peripheral $drv_handle. $drv_handle may not be accessible from the current processor."
            return
        }

        set ts_driver [get_property NAME $drv_handle]

        ucos_log_put "INFO" "handle_ts: Found driver $ts_driver for peripheral $ts_timer."

        #Find device ID
        set device_id 0
        set device_found false
        set periphs [get_common_driver_ips $drv_handle]
        foreach periph $periphs {
            if {[string compare -nocase $periph $ts_timer] == 0} {
                set device_found true
                set ts_periph $periph
                break
            }
            incr device_id
        }

        if {$device_found != true} {
            error "Failed to find peripheral instance for device $ts_timer"
            return
        }

        ucos_log_put "INFO" "handle_ts: Found instance number $device_id for peripheral $ts_timer."

        switch $ts_driver {
            "tmrctr" {
                ucos_log_put "INFO" "handle_ts: Timestamp source set to UCOS_TS_AXITIMER."
                set base_addr [get_property CONFIG.C_BASEADDR $ts_periph]
                puts $config_file "#define UCOS_TS_DRIVER UCOS_TS_AXITIMER"
                puts $config_file "#define UCOS_TS_DEVICE_ID $device_id"
                puts $config_file "#define UCOS_TS_BASE_ADDR $base_addr"
            }

            "default" {
                ucos_log_put "INFO" "handle_ts: Timestamp source set to UCOS_TS_NONE."
                puts $config_file "#define UCOS_TS_DRIVER UCOS_TS_NONE"
                puts $config_file "#define UCOS_TS_DEVICE_ID 0"
                puts $config_file "#define UCOS_TS_BASE_ADDR 0"
            }
        }

    } else {
        ucos_log_put "INFO" "handle_ts: No timestamp source specified."

        puts $config_file "#define UCOS_TS_DRIVER UCOS_TS_NONE"
        puts $config_file "#define UCOS_TS_DEVICE_ID 0"
        puts $config_file "#define UCOS_TS_BASE_ADDR 0"
    }
}

proc ucos_handle_stdout {os_handle} {
   set lib_standalone [get_libs -filter "NAME==ucos_standalone"];
   set stdout [get_property CONFIG.stdout $lib_standalone]
   set sw_proc_handle [get_sw_processor]
   set hw_proc_handle [get_cells [get_property hw_instance $sw_proc_handle]]
   set processor [get_property NAME $hw_proc_handle]

   if {[llength $stdout] == 1 && [string compare -nocase "none" $stdout] != 0} {
        ucos_log_put "INFO" "handle_stdout: Configuring UCOS STDOUT peripheral $stdout."
        set stdout_drv_handle [get_drivers -filter "HW_INSTANCE==$stdout"]
        if {[llength $stdout_drv_handle] == 0} {
            error "No driver for stdout peripheral $stdout. Check the following reasons: \n 1. $stdout is not accessible from processor $processor.\n 2. No Driver block is defined for $stdout in MSS file." "" "libgen_error"
            return
        }

        set interface_handle [get_sw_interfaces -of_objects $stdout_drv_handle -filter "NAME==stdout"]
        if {[llength $interface_handle] == 0} {
            error "No stdout interface available for driver for peripheral $stdout" "" "libgen_error"
            return
        }


        set config_file [open_include_file "xparameters.h"]

        set uart_driver [get_property NAME $stdout_drv_handle]

        #Find device ID
        set device_id 0
        set device_found false
        set periphs [get_common_driver_ips $stdout_drv_handle]
        foreach periph $periphs {
            if {[string compare -nocase $periph $stdout] == 0} {
                set device_found true
                break
            }
            incr device_id
        }

        if {$device_found != true} {
            error "Failed to find peripheral instance for device $stdout"
            return
        }

        ucos_log_put "INFO" "ucos_handle_stdout: Found device id $device_id for peripheral $stdout."

        switch $uart_driver {
            "ucos_uartps" {
                ucos_log_put "INFO" "ucos_handle_stdout: UCOS STDOUT source set to UCOS_UART_PS7_UART."
                puts $config_file "#define UCOS_STDOUT_DRIVER UCOS_UART_PS7_UART"
                puts $config_file "#define UCOS_STDOUT_DEVICE_ID $device_id"
            }

            "ucos_axiuartlite" {
                ucos_log_put "INFO" "ucos_handle_stdout: UCOS STDOUT source set to UCOS_UART_AXI_UART_LITE."
                puts $config_file "#define UCOS_STDOUT_DRIVER UCOS_UART_AXI_UART_LITE"
                puts $config_file "#define UCOS_STDOUT_DEVICE_ID $device_id"
            }

            "default" {
                ucos_log_put "INFO" "ucos_handle_stdout: UCOS STDOUT source set to UCOS_UART_NONE."
                puts $config_file "#define UCOS_STDOUT_DRIVER UCOS_UART_NONE"
                puts $config_file "#define UCOS_STDOUT_DEVICE_ID 0"
            }
        }

   } else {
        ucos_log_put "INFO" "ucos_handle_stdout: No UCOS STDOUT specified."
   }
}

proc ucos_handle_stdin {os_handle} {
   set lib_standalone [get_libs -filter "NAME==ucos_standalone"];
   set stdin [get_property CONFIG.stdin $lib_standalone]
   set sw_proc_handle [get_sw_processor]
   set hw_proc_handle [get_cells [get_property hw_instance $sw_proc_handle]]
   set processor [get_property NAME $hw_proc_handle]

   if {[llength $stdin] == 1 && [string compare -nocase "none" $stdin] != 0} {
        ucos_log_put "INFO" "ucos_handle_stdin: Configuring UCOS STDIN peripheral $stdin."
        set stdin_drv_handle [get_drivers -filter "HW_INSTANCE==$stdin"]
        if {[llength $stdin_drv_handle] == 0} {
            error "No driver for stdin peripheral $stdin. Check the following reasons: \n 1. $stdin is not accessible from processor $processor.\n 2. No Driver block is defined for $stdin in MSS file." "" "libgen_error"
            return
        }

        set interface_handle [get_sw_interfaces -of_objects $stdin_drv_handle -filter "NAME==stdin"]
        if {[llength $interface_handle] == 0} {
            error "No stdin interface available for driver for peripheral $stdin" "" "libgen_error"
            return
        }


        set config_file [open_include_file "xparameters.h"]

        set uart_driver [get_property NAME $stdin_drv_handle]

        #Find device ID
        set device_id 0
        set device_found false
        set periphs [get_common_driver_ips $stdin_drv_handle]
        foreach periph $periphs {
            if {[string compare -nocase $periph $stdin] == 0} {
                set device_found true
                break
            }
            incr device_id
        }

        if {$device_found != true} {
            error "Failed to find peripheral instance for device $stdin"
            return
        }

        ucos_log_put "INFO" "ucos_handle_stdin: Found device id $device_id for peripheral $stdin."

        switch $uart_driver {
            "ucos_uartps" {
                ucos_log_put "INFO" "ucos_handle_stdin: UCOS STDIN source set to UCOS_UART_PS7_UART."
                puts $config_file "#define UCOS_STDIN_DRIVER UCOS_UART_PS7_UART"
                puts $config_file "#define UCOS_STDIN_DEVICE_ID $device_id"
            }

            "ucos_axiuartlite" {
                ucos_log_put "INFO" "ucos_handle_stdin: UCOS STDIN source set to UCOS_UART_AXI_UART_LITE."
                puts $config_file "#define UCOS_STDIN_DRIVER UCOS_UART_AXI_UART_LITE"
                puts $config_file "#define UCOS_STDIN_DEVICE_ID $device_id"
            }

            "default" {
                ucos_log_put "INFO" "ucos_handle_stdin: UCOS STDIN source set to UCOS_UART_NONE."
                puts $config_file "#define UCOS_STDIN_DRIVER UCOS_UART_NONE"
                puts $config_file "#define UCOS_STDIN_DEVICE_ID 0"
            }
        }

   } else {
        ucos_log_put "INFO" "ucos_handle_stdin: No UCOS STDIN specified."
   }
}

proc ucos_handle_ethernet {ucos_handle} {
    set ethernet_if [get_property CONFIG.UCOS_ETHERNET_INTERFACE $ucos_handle]

    set config_file [open_include_file "xparameters.h"]
    set ethernet_driver ""

    if {[llength $ethernet_if] == 1 && [string compare -nocase "none" $ethernet_if] != 0} {
        ucos_log_put "INFO" "ucos_handle_ethernet: Configuring ETHERNET peripheral $ethernet_if."
        set ethernet_if_drv_handle [get_drivers -filter "HW_INSTANCE==$ethernet_if"]
        if {[llength $ethernet_if_drv_handle] == 0} {
            error "No driver for ethernet peripheral $ethernet_if." "libgen_error"
            return
        }

        set ethernet_if_handle [get_sw_interfaces -of_objects $ethernet_if_drv_handle -filter "NAME==ethernet"]
        if {[llength $ethernet_if_handle] == 0} {
            error "No ethernet interface available for driver for peripheral $ethernet_if" "" "libgen_error"
            return
        }

        #Find device ID
        set device_id 0
        set device_found false
        set periphs [get_common_driver_ips $ethernet_if_drv_handle]
        foreach periph $periphs {
            if {[string compare -nocase $periph $ethernet_if] == 0} {
                set device_found true
                break
            }
            incr device_id
        }

        if {$device_found != true} {
            error "Failed to find peripheral instance for device $ethernet_if"
            return
        }

        ucos_log_put "INFO" "ucos_handle_ethernet: Found device id $device_id for peripheral $ethernet_if."

        set ethernet_driver [get_property NAME $ethernet_if_drv_handle]
        ucos_log_put "TRACE" "ucos_handle_ethernet: Ethernet IF driver: $ethernet_driver."

        switch $ethernet_driver {
            "ucos_emacps" {
                puts $config_file "#define UCOS_ETHERNET_DRIVER UCOS_ETHERNET_EMACPS"
                puts $config_file "#define UCOS_ETHERNET_DEVICE_ID $device_id"
            }

            "ucos_axiethernetlite" {
                puts $config_file "#define UCOS_ETHERNET_DRIVER UCOS_ETHERNET_AXIETHLITE"
                puts $config_file "#define UCOS_ETHERNET_DEVICE_ID $device_id"
            }

            "ucos_axiethernet" {
                puts $config_file "#define UCOS_ETHERNET_DRIVER UCOS_ETHERNET_AXIETH"
                puts $config_file "#define UCOS_ETHERNET_DEVICE_ID $device_id"
            }

            "default" {
                puts $config_file "#define UCOS_ETHERNET_DRIVER UCOS_ETHERNET_NONE"
                puts $config_file "#define UCOS_ETHERNET_DEVICE_ID $device_id"
            }
        }
    } else {
        ucos_log_put "INFO" "ucos_handle_ethernet: No ETHERNET interface specified."
    }

    set config_addr [get_property CONFIG.UCOS_ETHERNET_ADDRESS $ucos_handle]
    set config_gate [get_property CONFIG.UCOS_ETHERNET_GATEWAY $ucos_handle]
    set config_mask [get_property CONFIG.UCOS_ETHERNET_SUBMASK $ucos_handle]
    set config_dhcp [get_property CONFIG.UCOS_ETHERNET_DHCP $ucos_handle]

    puts $config_file "#define UCOS_ETHERNET_ADDRESS \"$config_addr\""
    puts $config_file "#define UCOS_ETHERNET_GATEWAY \"$config_gate\""
    puts $config_file "#define UCOS_ETHERNET_SUBMASK \"$config_mask\""

    if {$config_dhcp == true} {
        puts $config_file "#define UCOS_ETHERNET_DHCP DEF_ENABLED"
    } else {
        puts $config_file "#define UCOS_ETHERNET_DHCP DEF_DISABLED"
    }

    set config_rx_buf_nbr [get_property CONFIG.UCOS_IF_RX_BUF_NBR $ucos_handle]
    set config_tx_lbuf_nbr [get_property CONFIG.UCOS_IF_TX_LARGE_BUF_NBR $ucos_handle]
    set config_tx_sbuf_nbr [get_property CONFIG.UCOS_IF_TX_SMALL_BUF_NBR $ucos_handle]
    set config_rx_desc_nbr [get_property CONFIG.UCOS_IF_RX_DESC_NBR $ucos_handle]
    set config_tx_desc_nbr [get_property CONFIG.UCOS_IF_TX_DESC_NBR $ucos_handle]

    puts $config_file "#define UCOS_IF_RX_BUF_NBR $config_rx_buf_nbr"
    puts $config_file "#define UCOS_IF_TX_LARGE_BUF_NBR $config_tx_lbuf_nbr"
    puts $config_file "#define UCOS_IF_TX_SMALL_BUF_NBR $config_tx_sbuf_nbr"
    puts $config_file "#define UCOS_IF_RX_DESC_NBR $config_rx_desc_nbr"
    puts $config_file "#define UCOS_IF_TX_DESC_NBR $config_tx_desc_nbr"

    set config_dedic_addr [get_property CONFIG.UCOS_IF_DEDIC_MEM_ADDR $ucos_handle]
    set config_dedic_size [get_property CONFIG.UCOS_IF_DEDIC_MEM_SIZE $ucos_handle]

    puts $config_file "#define UCOS_IF_DEDIC_MEM_ADDR $config_dedic_addr"
    puts $config_file "#define UCOS_IF_DEDIC_MEM_SIZE $config_dedic_size"

    set mac_addr [get_property CONFIG.UCOS_IF_HW_ADDR $ucos_handle]
    puts $config_file "#define UCOS_IF_HW_ADDR \"$mac_addr\""

    set config_phy_bus_addr [get_property CONFIG.UCOS_PHY_BUS_ADDR $ucos_handle]
    set config_phy_bus_mode [get_property CONFIG.UCOS_PHY_BUS_MODE $ucos_handle]
    set config_phy_type [get_property CONFIG.UCOS_PHY_TYPE $ucos_handle]
    set config_phy_speed [get_property CONFIG.UCOS_PHY_SPEED $ucos_handle]
    set config_phy_duplex [get_property CONFIG.UCOS_PHY_DUPLEX $ucos_handle]

    if {$config_phy_bus_mode == "UCOS_NET_PHY_BUS_MODE_AUTO"} {
        switch $ethernet_driver {
            "ucos_emacps" {
                set config_phy_bus_mode UCOS_NET_PHY_BUS_MODE_GMII
            }

            "ucos_axiethernetlite" {
                set config_phy_bus_mode UCOS_NET_PHY_BUS_MODE_MII
            }

            "ucos_axiethernet" {
                set config_phy_bus_mode UCOS_NET_PHY_BUS_MODE_GMII
            }

            "default" {
                set config_phy_bus_mode UCOS_NET_PHY_BUS_MODE_MII
            }
        }
    }

    ucos_log_put "TRACE" "ucos_handle_ethernet: Ethernet PHY bus mode set to : $config_phy_bus_mode."

    if {$config_phy_type == "UCOS_NET_PHY_TYPE_AUTO"} {
        set config_phy_type UCOS_NET_PHY_TYPE_INT
    }

    ucos_log_put "TRACE" "ucos_handle_ethernet: Ethernet PHY type set to : $config_phy_type."

    puts $config_file "#define UCOS_PHY_BUS_ADDR $config_phy_bus_addr"
    puts $config_file "#define UCOS_PHY_BUS_MODE $config_phy_bus_mode"
    puts $config_file "#define UCOS_PHY_TYPE $config_phy_type"
    puts $config_file "#define UCOS_PHY_SPEED $config_phy_speed"
    puts $config_file "#define UCOS_PHY_DUPLEX $config_phy_duplex"

}

proc ucos_handle_usb {ucos_handle} {
    set usb_if [get_property CONFIG.UCOS_USB_INTERFACE $ucos_handle]
    set usb_if_type [get_property CONFIG.UCOS_USB_INTERFACE_TYPE $ucos_handle]
    set config_file [open_include_file "xparameters.h"]


    if {[llength $usb_if] == 1 && [string compare -nocase "none" $usb_if] != 0} {
        set usb_if_drv_handle [get_drivers -filter "HW_INSTANCE==$usb_if"]
        if {[llength $usb_if_drv_handle] == 0} {
            error "No driver for usb peripheral $usb_if_drv_handle." "libgen_error"
            return
        }

        set usb_if_handle [get_sw_interfaces -of_objects $usb_if_drv_handle -filter "NAME==ucos_usb"]
        if {[llength $usb_if_handle] == 0} {
            error "No usb interface available for driver for peripheral $usb_if" "" "libgen_error"
            return
        }

        #Find device ID
        set device_id 0
        set device_found false
        set periphs [get_common_driver_ips $usb_if_drv_handle]
        foreach periph $periphs {
            if {[string compare -nocase $periph $usb_if] == 0} {
                set device_found true
                break
            }
            incr device_id
        }

        if {$device_found != true} {
            error "Failed to find peripheral instance for device $usb_if"
            return
        }

        set usb_driver [get_property NAME $usb_if_drv_handle]

        switch $usb_driver {
            "ucos_usbps" {
                puts $config_file "#define UCOS_USB_DRIVER UCOS_USB_USBPS"
                puts $config_file "#define UCOS_USB_DEVICE_ID $device_id"
            }

            "default" {
                puts $config_file "#define UCOS_USB_DRIVER UCOS_USB_NONE"
                puts $config_file "#define UCOS_USB_DEVICE_ID 0"
            }
        }
    } else {
        puts $config_file "#define UCOS_USB_DRIVER UCOS_USB_NONE"
        puts $config_file "#define UCOS_USB_DEVICE_ID 0"
    }

    switch $usb_if_type {
        "device" {
            puts $config_file "#define UCOS_USB_TYPE UCOS_USB_TYPE_DEVICE"
        }

        "host" {
            puts $config_file "#define UCOS_USB_TYPE UCOS_USB_TYPE_HOST"
        }

        "default" {
            puts $config_file "#define UCOS_USB_TYPE UCOS_USB_TYPE_NONE"
        }
    }
}

proc ucos_handle_ramdisk {ucos_handle} {
    set config_file [open_include_file "xparameters.h"]

    set config_enabled [get_property CONFIG.UCOS_RAMDISK_EN $ucos_handle]
    set config_size [get_property CONFIG.UCOS_RAMDISK_SIZE $ucos_handle]
    set config_sec_size [get_property CONFIG.UCOS_RAMDISK_SECTOR_SIZE $ucos_handle]
    set config_base [get_property CONFIG.UCOS_RAMDISK_BASE_ADDRESS $ucos_handle]

    if {$config_enabled == true} {
        puts $config_file "#define UCOS_RAMDISK_EN DEF_ENABLED"
    } else {
        puts $config_file "#define UCOS_RAMDISK_EN DEF_DISABLED"
    }

    puts $config_file "#define UCOS_RAMDISK_SIZE $config_size"
    puts $config_file "#define UCOS_RAMDISK_SECTOR_SIZE $config_sec_size"
    puts $config_file "#define UCOS_RAMDISK_BASE_ADDRESS $config_base"
}

# TODO - Incomplete
proc ucos_handle_sdcard {ucos_handle} {
    set sdcard_if [get_property CONFIG.UCOS_SDCARD_INTERFACE $ucos_handle]

    set config_file [open_include_file "xparameters.h"]

    if {[llength $sdcard_if] == 1 && [string compare -nocase "none" $sdcard_if] != 0} {
        puts $config_file "#define UCOS_SDCARD_EN DEF_ENABLED"
    } else {
        puts $config_file "#define UCOS_SDCARD_EN DEF_DISABLED"
    }
}

