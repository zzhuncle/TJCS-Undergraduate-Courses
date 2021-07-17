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

proc ucos_log_put {log_level log_message} {
    set file_handle [open "../../ucos.log" a+]

    switch $log_level {
        "TRACE" {
            set log_type "TRACE  "
        }

        "INFO" {
            set log_type "INFO   "
        }

        "WARNING" {
            set log_type "WARNING"
        }

        "ERROR" {
            set log_type "ERROR  "
        }
    }

    puts $file_handle "$log_type - $log_message"

    close $file_handle
}

proc ucos_get_source_base {} {

    set source_dir [get_property CONFIG.MICRIUM_SOURCE_DIR [get_os]]

    if {$source_dir != ""} {
        return $source_dir
    }

    set cur_dir [file dirname [dict get [info frame [info frame]] file]]

    if {[file isdirectory [file join $cur_dir "../../../uC-CPU"]]} {
        return [file normalize [file join $cur_dir "../../..//"]]
    } else {
        return [file normalize [file join $cur_dir "../../../micrium_source"]]
    }

    return ""
}

proc ucos_get_product_source_base {product} {

    set type_file_table(cpu) "uC-CPU/cpu_core.c"
    set type_file_table(lib) "uC-LIB/lib_mem.c"
    set type_file_table(common) "uC-Common/KAL/kal.h"
    set type_file_table(clk) "uC-Clk/Source/clk.c"
    set type_file_table(crc) "uC-CRC/Source/crc_util.c"
    set type_file_table(shell) "uC-Shell/Source/shell.c"
    set type_file_table(osii) "uCOS-II/Source/os_core.c"
    set type_file_table(osiii) "uCOS-III/Source/os_core.c"
    set type_file_table(tcpip) "uC-TCPIP/Source/net.c"
    set type_file_table(tcpip-exp) "uC-TCPIP/Source/net_task.c"
    set type_file_table(fs) "uC-FS/Source/fs.c"
    set type_file_table(usbd) "uC-USB-Device/Source/usbd_core.c"
    set type_file_table(usbh) "uC-USB-Host/Source/usbh_core.c"
    set type_file_table(dhcpc) "uC-DHCPc/Source/dhcp-c.c"
    set type_file_table(dnsc) "uC-DNSc/Source/dns-c.c"
    set type_file_table(ftps) "uC-FTPs/Source/ftp-s.c"
    set type_file_table(httpc) "uC-HTTP/Client/Source/http-c.c"
    set type_file_table(https) "uC-HTTP/Server/Source/http-s.c"
    set type_file_table(iperf) "uC-IPerf/Source/iperf.c"
    set type_file_table(telnets) "uC-TELNETs/Source/telnet-s.c"
    set type_file_table(can) "uC-CAN/Source/can_bus.c"
    set type_file_table(mqttc) "uC-MQTT/Client/Source/mqtt-c.c"
    set type_file_table(openamp) "openamp/source/rpmsg.c"

    set micrium_base [ucos_get_source_base]
    set type_file $type_file_table($product)

    if {[file exists [file join $micrium_base $type_file]]} {
        set src_base [file join $micrium_base $type_file]
        ucos_log_put "INFO" "ucos_get_product_source_base: Source base for $product found at $src_base."
        return $micrium_base
    }
    ucos_log_put "INFO" "ucos_get_product_source_base: Source base for $product not found."
    return ""
}

proc ucos_get_lib_base {} {
    set cur_dir [file dirname [dict get [info frame [info frame]] file]]

    return [file normalize [file join $cur_dir "../../../micrium_libraries"]]
}

proc ucos_get_arch_name {} {
    set sw_proc_handle [get_sw_processor]
    set hw_proc_handle [get_cells [get_property HW_INSTANCE $sw_proc_handle] ]
    set proctype [get_property IP_NAME $hw_proc_handle]

    switch $proctype {
        "microblaze" {
            return "mb"
        }

        "ps7_cortexa9" {
            return "v7a"
        }

        "psu_cortexa53" {
            return "v8a"
        }

        "psu_cortexr5" {
            return "v7r"
        }

        "default" {error "Unknown processor type"}
    }
}

proc ucos_define_intr_sources {drv_handle define_name} {

    set config_file [open_include_file "xparameters.h"]


    set periphs [get_common_driver_ips $drv_handle]

    foreach periph $periphs {
        set intr_source [ucos_get_intr_src $periph]
        set arg_name [get_ip_param_name $periph "INT_SOURCE"]

        if {$intr_source == -1} {
            set intr_source "UCOS_INT_SOURCE_NONE"
        }
        puts $config_file [format "#define %s %s\n" $arg_name $intr_source]
    }
}

proc ucos_get_intr_src {periph} {

    ucos_log_put "TRACE" "ucos_get_intr_src: Finding interrupt source for peripheral $periph."

    #Hardcode the interrupt of the hard periphs for now.
    set ps7_int_table(ps7_usb_0) 53
    set ps7_int_table(ps7_ethernet_0) 54
    set ps7_int_table(ps7_sd_0) 56
    set ps7_int_table(ps7_uart_0) 59
    set ps7_int_table(ps7_usb_1) 76
    set ps7_int_table(ps7_ethernet_1) 77
    set ps7_int_table(ps7_sd_1) 79
    set ps7_int_table(ps7_uart_1) 82

    set ps8_int_table(psu_uart_0) 53
    set ps8_int_table(psu_uart_1) 54
    set ps8_int_table(psu_ethernet_0) 54

    set ps5_int_table(psu_uart_0) 53
    set ps5_int_table(psu_uart_1) 54
    set ps5_int_table(psu_ethernet_0) 54

    set sw_proc_handle [get_sw_processor]
    set hw_proc_handle [get_cells [get_property HW_INSTANCE $sw_proc_handle] ]
    set int_source_ports [get_interrupt_sources $hw_proc_handle]

    set proctype [get_property IP_NAME $hw_proc_handle]

    switch $proctype {
    "microblaze" {
        if {($periph == "ps7_ethernet_0") || ($periph == "ps7_ethernet_1")} {
            set broken_irq_get true
        } else {
            set broken_irq_get false
        }

        set intr_source -1
        set def_intr_source -1
        set intr_ctrl -1
        set axi_intc_driver_handle [get_drivers -filter "NAME==\"intc\""]
        set handle [lindex $axi_intc_driver_handle 0]

        ucos_log_put "TRACE" "ucos_get_intr_src: Found int driver $handle."

        if {[llength $int_source_ports] != 0} {
            set int_source_periph [get_cells -of_objects $int_source_ports]
            set int_source_periph_name [get_property IP_NAME $int_source_periph]

            if {$int_source_periph_name == "axi_intc"} {
                set axiintc_periphs [get_common_driver_ips $handle]

                foreach axiintc_periph $axiintc_periphs {
                    set source_pins [get_interrupt_sources $axiintc_periph]
                    foreach source_pin $source_pins {
                        set source_periph [get_cells -of_objects $source_pin]
                        set t_source_name   [common::get_property NAME $source_periph]

                        if {$source_periph == $periph} {
                            set intr_source [::hsi::utils::get_interrupt_id $t_source_name $source_pin]
                            set intr_ctrl $axiintc_periph
                            break
                        }

                        if {([llength $source_periph] > 1) && ($def_intr_source == -1)} {
                            set def_intr_source $intr_source
                        }

                    }
                }
            } else {
                set int_source_periph_base_name [get_property NAME $int_source_periph]
                if {$int_source_periph_base_name == $periph} {
                    set intr_source 0
                } else {
                    set intr_source -1
                }
            }

        } else {
            set intr_source -1
        }

        if {$intr_ctrl != -1} {
            ucos_log_put "INFO" "ucos_get_intr_src: Interrupt controller for peripheral $periph : $intr_ctrl."
        }

        if {$broken_irq_get == true} {
            ucos_log_put "WARNING" "ucos_get_intr_src: Interrupt signal routing for peripheral $periph is broken guessing interrupt id $def_intr_source."
            set intr_source $def_intr_source
        }

        ucos_log_put "INFO" "ucos_get_intr_src: Interrupt source for peripheral $periph : $intr_source."

        if {$intr_source == -1} {
            ucos_log_put "INFO" "ucos_get_intr_src: Interrupt source for peripheral $periph not found"
        }
        return $intr_source
    }

    "ps7_cortexa9" {
        set intr_source -1
        catch {set intr_source $ps7_int_table($periph)}

        # if not in the global table try the F2P interrupts
        if {$intr_source == -1} {
            ucos_log_put "INFO" "ucos_get_intr_src: Interrupt source for peripheral $periph not found in the hardened IPs trying F2P interrupts."
            set scugic_drv_handle [get_drivers -filter "HW_INSTANCE==ps7_scugic_0"]
            set scugics [get_common_driver_ips $scugic_drv_handle]
            set scugic $scugics

            set pin [get_pins -of_objects $scugic IRQ_F2P]
            set source_ports [get_source_pins $pin]

            # Patch to allow BSP generation when peripheral is not connected to the zynq interrupt controller
            if {[llength $source_ports] == 0} {
                return -1;
            }

            set source_periph [get_cells -of_object $source_ports]
            set source_periph_ip_name [get_property IP_NAME $source_periph]
            if { [string match -nocase $source_periph_ip_name "xlconcat" ] } {
                set source_ports [::hsi::__internal::get_concat_interrupt_sources $source_periph]
            }

            set i 0
            lappend source_list
            foreach source_port $source_ports {
                set source_periph [get_cells -of_object $source_port]

                if {[string compare -nocase $source_periph $periph] == 0 } {
                    set intr_source [expr $i + 61]
                }

                incr i
            }

            if {$intr_source != -1} {
                ucos_log_put "INFO" "ucos_get_intr_src: Interrupt source for peripheral $periph : $intr_source."
            }
        } else {
            ucos_log_put "INFO" "ucos_get_intr_src: Interrupt source for peripheral $periph : $intr_source."
        }

        if {$intr_source == -1} {
            ucos_log_put "INFO" "ucos_get_intr_src: Interrupt source for peripheral $periph not found"
        }
        return $intr_source
    }

    "psu_cortexa53" {
        set intr_source -1
        catch {set intr_source $ps8_int_table($periph)}

        ucos_log_put "INFO" "ucos_get_intr_src: Interrupt source for peripheral $periph : $intr_source."

        if {$intr_source == -1} {
            ucos_log_put "INFO" "ucos_get_intr_src: Interrupt source for peripheral $periph not found"
        }

        return $intr_source
    }

    "psu_cortexr5" {
        set intr_source -1
        catch {set intr_source $ps5_int_table($periph)}

        ucos_log_put "INFO" "ucos_get_intr_src: Interrupt source for peripheral $periph : $intr_source."

        if {$intr_source == -1} {
            ucos_log_put "INFO" "ucos_get_intr_src: Interrupt source for peripheral $periph not found"
        }

        return $intr_source
    }

    "default" {error "Unknown processor type $proctype"}
    }
}


proc get_ethernet_if_type {} {
    set ucos_handle [get_os]
    set ethernet_if [get_property CONFIG.UCOS_ETHERNET_INTERFACE $ucos_handle]
    if {[llength $ethernet_if] == 1 && [string compare -nocase "none" $ethernet_if] != 0} {
        set ethernet_if_drv_handle [get_drivers -filter "HW_INSTANCE==$ethernet_if"]
        if {[llength $ethernet_if_drv_handle] == 0} {
            error "No driver for ethernet peripheral $ethernet_if." "libgen_error"
            return ""
        }

        set ethernet_if_handle [get_sw_interfaces -of_objects $ethernet_if_drv_handle -filter "NAME==ethernet"]
        if {[llength $ethernet_if_handle] == 0} {
            error "No ethernet interface available for driver for peripheral $ethernet_if" "" "libgen_error"
            return ""
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
            return ""
        }

        set ethernet_driver [get_property NAME $ethernet_if_drv_handle]

        switch $ethernet_driver {
            "ucos_emacps" {
                return ps7_ethernet
            }

            "default" {
                return ""
            }
        }
    }
}

proc ucos_heap_estimate {} {
    ucos_log_put "TRACE" "ucos_heap_estimate: Computing heap estimate."

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

    set ucos_base_heap 512
    set ucos_task_overhead 208

    if {[llength $lib_osiii]} {
        set ucos_sem_overhead 32
    } else {
        set ucos_sem_overhead 0
    }

    set heap_size 0

    #Base BSP + kernel
    set heap_size [expr $heap_size + $ucos_base_heap]
    ucos_log_put "TRACE" "ucos_heap_estimate: Base heap estimate: $ucos_base_heap."

    #uC-TCPIP
    set tcpip_base_heap 784
    set tcpip_tcp_sock_heap 408
    set tcpip_udp_sock_heap 168
    set tcpip_ipv6_base_heap 96
    set tcpip_if_base_heap 24
    set tcpip_ipv4_buf_heap 264
    set tcpip_ipv6_buf_heap 352

    #uC-HTTPc
    set httpc_base_heap 128

    #uC-MQTTc
    set mqttc_base_heap 128

    if {[llength $lib_tcpip]} {
        set initial_heap $heap_size
        set rx_stack_size [get_property CONFIG.NET_TASK_CFG_RX_STACK_SIZE $lib_tcpip]
        set txd_stack_size [get_property CONFIG.NET_TASK_CFG_TXDEALLOC_STACK_SIZE $lib_tcpip]
        set tmr_stack_size [get_property CONFIG.NET_TASK_CFG_TMR_STACK_SIZE $lib_tcpip]
        set tcp_sock_nbr [get_property CONFIG.NET_SOCK_CFG_SOCK_NBR_TCP $lib_tcpip]
        set udp_sock_nbr [get_property CONFIG.NET_SOCK_CFG_SOCK_NBR_UDP $lib_tcpip]
        set ipv6_en [get_property CONFIG.NET_IPv6_CFG_EN $lib_tcpip]
        set if_nbr [get_property CONFIG.NET_IF_CFG_MAX_NBR_IF $lib_tcpip]
        set eth_if [get_property CONFIG.UCOS_ETHERNET_INTERFACE [get_os]]
        set rx_buf_size 1536
        set rx_buf_nbr [get_property CONFIG.UCOS_IF_RX_BUF_NBR [get_os]]
        set txl_buf_size 1632
        set txl_buf_nbr [get_property CONFIG.UCOS_IF_TX_LARGE_BUF_NBR [get_os]]
        set txs_buf_size 160
        set txs_buf_nbr [get_property CONFIG.UCOS_IF_TX_SMALL_BUF_NBR [get_os]]

        ucos_log_put "TRACE" "ucos_heap_estimate: TCP-IP base heap estimate: $tcpip_base_heap."
        set heap_size [expr $heap_size + $tcpip_base_heap]

        set stack_heap [expr $rx_stack_size + $txd_stack_size + $tmr_stack_size +($ucos_task_overhead * 3)]
        set heap_size [expr $heap_size + $stack_heap]
        ucos_log_put "TRACE" "ucos_heap_estimate: TCP tasks heap estimate: $stack_heap."

        set tcp_sock_heap [expr $tcp_sock_nbr * $tcpip_tcp_sock_heap]
        set heap_size [expr $heap_size + $tcp_sock_heap]
        ucos_log_put "TRACE" "ucos_heap_estimate: TCP Socket heap estimate: $tcp_sock_heap."


        set udp_sock_heap [expr $udp_sock_nbr * $tcpip_udp_sock_heap]
        set heap_size [expr $heap_size + $udp_sock_heap]
        ucos_log_put "TRACE" "ucos_heap_estimate: UDP Socket heap estimate: $udp_sock_heap."

        if {$ipv6_en == true} {
            set heap_size [expr $heap_size + $tcpip_ipv6_base_heap]
            ucos_log_put "TRACE" "ucos_heap_estimate: IPv6 base heap estimate: $tcpip_ipv6_base_heap."
        }

        set if_heap [expr $udp_sock_nbr * $tcpip_if_base_heap]
        set heap_size [expr $heap_size + $if_heap]
        ucos_log_put "TRACE" "ucos_heap_estimate: Interface heap estimate: $if_heap."

        if {$eth_if != "none"} {
            if {$ipv6_en == true} {
                set tcpip_buf_heap $tcpip_ipv6_buf_heap
            } else {
                set tcpip_buf_heap $tcpip_ipv4_buf_heap
            }

            set rx_buf_heap [expr $rx_buf_nbr * ($tcpip_buf_heap + $rx_buf_size)]
            set heap_size [expr $heap_size + $rx_buf_heap]
            ucos_log_put "TRACE" "ucos_heap_estimate: RX buffers heap estimate: $rx_buf_heap."

            set txl_buf_heap [expr $txl_buf_nbr * ($tcpip_buf_heap + $txl_buf_size)]
            set heap_size [expr $heap_size + $txl_buf_heap]
            ucos_log_put "TRACE" "ucos_heap_estimate: Large TX buffers heap estimate: $txl_buf_heap."

            set txs_buf_heap [expr $txs_buf_nbr * ($tcpip_buf_heap + $txs_buf_size)]
            set heap_size [expr $heap_size + $txs_buf_heap]
            ucos_log_put "TRACE" "ucos_heap_estimate: Small TX buffers heap estimate: $txs_buf_heap."
        }

        set tcp_heap_tot [expr $heap_size - $initial_heap]
        ucos_log_put "INFO" "ucos_heap_estimate: uC/TCP-IP estimated heap usage: $tcp_heap_tot."
    }

    if {[llength $lib_httpc]} {
        set initial_heap $heap_size

        ucos_log_put "TRACE" "ucos_heap_estimate: HTTPc base heap estimate: $httpc_base_heap."
        set heap_size [expr $heap_size + $httpc_base_heap]

        set async_en [get_property CONFIG.HTTPc_CFG_MODE_ASYNC_TASK_EN $lib_httpc]

        if {$async_en == true} {
            set task_heap_size [get_property CONFIG.HTTPc_OS_CFG_TASK_STK_SIZE $lib_httpc]
            set task_heap_size [expr $task_heap_size + $ucos_task_overhead]
            set heap_size [expr $heap_size + $task_heap_size]
            ucos_log_put "TRACE" "ucos_heap_estimate: HTTPc tasks heap estimate: $task_heap_size."
        } else {
            ucos_log_put "TRACE" "ucos_heap_estimate: HTTPc tasks heap estimate: 0."
        }

        set httpc_heap_tot [expr $heap_size - $initial_heap]
        ucos_log_put "INFO" "ucos_heap_estimate: uC/HTTPc estimated heap usage: $httpc_heap_tot."

    }

    if {[llength $lib_mqttc]} {
        set initial_heap $heap_size

        set stack_size [get_property CONFIG.MQTTc_OS_CFG_TASK_STK_SIZE $lib_mqttc]

        ucos_log_put "TRACE" "ucos_heap_estimate: MQTTc base heap estimate: $mqttc_base_heap."
        set heap_size [expr $heap_size + $mqttc_base_heap]

        set stack_heap [expr $stack_size + $ucos_task_overhead]
        set heap_size [expr $heap_size + $stack_heap]
        ucos_log_put "TRACE" "ucos_heap_estimate: MQTTc task heap estimate: $stack_heap."

        set mqttc_heap_tot [expr $heap_size - $initial_heap]
        ucos_log_put "INFO" "ucos_heap_estimate: uC/MQTTc estimated heap usage: $mqttc_heap_tot."
    }

    #uC-FS
    set fs_base_heap 512
    set fs_buf_heap 24
    set fs_dev_heap 44
    set fs_dir_heap 24
    set fs_file_heap 120
    set fs_file_buf_opt_heap 28
    set fs_vol_heap 156
    set fs_vol_cache_heap 8
    set fs_drv_heap 32

    if {[llength $lib_fs]} {
        set initial_heap $heap_size
        set stat_en [get_property CONFIG.FS_CFG_CTR_STAT_EN $lib_fs]
        if {$stat_en == true} {
            set fs_stat_heap 8
        } else {
            set fs_stat_heap 0
        }

        set dev_cnt [get_property CONFIG.FS_CFG_MAX_DEV_CNT $lib_fs]
        set vol_cnt [get_property CONFIG.FS_CFG_MAX_VOL_CNT $lib_fs]
        set file_cnt [get_property CONFIG.FS_CFG_MAX_FILE_CNT $lib_fs]
        set dir_cnt [get_property CONFIG.FS_CFG_MAX_DIR_CNT $lib_fs]
        set buf_cnt [get_property CONFIG.FS_CFG_MAX_BUF_CNT $lib_fs]
        set drv_cnt [get_property CONFIG.FS_CFG_MAX_DEV_DRV_CNT $lib_fs]
        set sec_size [get_property CONFIG.FS_CFG_MAX_SEC_SIZE $lib_fs]

        set max_dev_name [get_property CONFIG.FS_CFG_MAX_DEV_NAME_LEN $lib_fs]
        set max_dev_drv_name [get_property CONFIG.FS_CFG_MAX_DEV_DRV_NAME_LEN $lib_fs]
        set max_file_name [get_property CONFIG.FS_CFG_MAX_FILE_NAME_LEN $lib_fs]
        set max_path_name [get_property CONFIG.FS_CFG_MAX_PATH_NAME_LEN $lib_fs]
        set max_vol_name [get_property CONFIG.FS_CFG_MAX_VOL_NAME_LEN $lib_fs]

        set file_lock [get_property CONFIG.FS_CFG_FILE_LOCK_EN $lib_fs]
        set file_buf [get_property CONFIG.FS_CFG_FILE_BUF_EN $lib_fs]

        if {$buf_cnt == 0} {
            set buf_cnt [expr $vol_cnt * 3]
        }

        if {$drv_cnt == 0} {
            set drv_cnt 2
        }

        ucos_log_put "TRACE" "ucos_heap_estimate: FS base heap estimate: $fs_base_heap."
        set heap_size [expr $heap_size + $fs_base_heap]

        set dev_heap [expr $dev_cnt * ($fs_dev_heap + $fs_stat_heap + $max_dev_name + ($ucos_sem_overhead * 2))]
        set heap_size [expr $heap_size + $dev_heap]
        ucos_log_put "TRACE" "ucos_heap_estimate: Device heap estimate: $dev_heap."

        set vol_heap [expr $vol_cnt * ($fs_vol_heap + ($fs_stat_heap * 2) + $max_vol_name)]
        set heap_size [expr $heap_size + $vol_heap]
        ucos_log_put "TRACE" "ucos_heap_estimate: Volumes heap estimate: $vol_heap."


        if {$file_lock == true} {
            set file_lock_heap $ucos_sem_overhead
        } else {
            set file_lock_heap 0
        }

        if {$file_buf == true} {
            set fs_file_buf_opt_heap $fs_file_buf_opt_heap
        } else {
            set fs_file_buf_opt_heap 0
        }

        set file_heap [expr $file_cnt * ($fs_file_heap + $fs_stat_heap + $file_lock_heap + $fs_file_buf_opt_heap)]
        set heap_size [expr $heap_size + $file_heap]
        ucos_log_put "TRACE" "ucos_heap_estimate: Files heap estimate: $file_heap."

        set dir_heap [expr $dir_cnt * ($fs_dir_heap + $fs_stat_heap)]
        set heap_size [expr $heap_size + $dir_heap]
        ucos_log_put "TRACE" "ucos_heap_estimate: Directories heap estimate: $dir_heap."

        set buf_heap [expr $buf_cnt * ($fs_buf_heap + $sec_size)]
        set heap_size [expr $heap_size + $buf_heap]
        ucos_log_put "TRACE" "ucos_heap_estimate: Buffers heap estimate: $buf_heap."

        set drv_heap [expr $drv_cnt * ($fs_drv_heap)]
        set heap_size [expr $heap_size + $drv_heap]
        ucos_log_put "TRACE" "ucos_heap_estimate: Drivers heap estimate: $drv_heap."

        set fs_heap_tot [expr $heap_size - $initial_heap]
        ucos_log_put "INFO" "ucos_heap_estimate: uC/FS estimated heap usage: $fs_heap_tot."
    }

    #uc-usbd
    set usbd_base_heap 8192

    if {[llength $lib_usbd]} {
        set initial_heap $heap_size

        ucos_log_put "TRACE" "ucos_heap_estimate: USBD base heap estimate: $usbd_base_heap."
        set heap_size [expr $heap_size + $usbd_base_heap]

        set usbd_heap_tot [expr $heap_size - $initial_heap]
        ucos_log_put "INFO" "ucos_heap_estimate: uC/USB-Device estimated heap usage: $usbd_heap_tot."
    }

    #uc-usbh
    # Add 70,000 for the MSC driver.

    set usbh_base_heap [expr 8192 + 70000]

    if {[llength $lib_usbh]} {
        set initial_heap $heap_size

        ucos_log_put "TRACE" "ucos_heap_estimate: USBH base heap estimate: $usbh_base_heap."
        set heap_size [expr $heap_size + $usbh_base_heap]

        set usbh_heap_tot [expr $heap_size - $initial_heap]
        ucos_log_put "INFO" "ucos_heap_estimate: uC/USB-Host estimated heap usage: $usbh_heap_tot."
    }

    #uc-openamp
    set openamp_base_heap 4096

    if {[llength $lib_openamp]} {
        set initial_heap $heap_size

        ucos_log_put "TRACE" "ucos_heap_estimate: OpenAMP base heap estimate: $openamp_base_heap."
        set heap_size [expr $heap_size + $openamp_base_heap]

        set openamp_heap_tot [expr $heap_size - $initial_heap]
        ucos_log_put "INFO" "ucos_heap_estimate: OpenAMP estimated heap usage: $openamp_heap_tot."
    }

    set sw_proc_handle [get_sw_processor]
    set hw_proc_handle [get_cells [get_property HW_INSTANCE $sw_proc_handle] ]
    set proctype [get_property IP_NAME $hw_proc_handle]

    if {$proctype == "psu_cortexa53"} {
        set heap_size [expr $heap_size * 2]
    }

    ucos_log_put "INFO" "ucos_heap_estimate: Computed heap size $heap_size."

    return $heap_size
}

proc set_define { CFG_FILE DEFINE_NAME NEW_DEFINE_VAL } {
    set FILE     [open $CFG_FILE r]
    set tmpname "temp"
    set NEW_FILE [open $tmpname w]
    set REPLACE_SUCCESSFUL 0
    set REPLACE_SKIPPED    0
    set CNT                0
    set NEW_LINE  "-1"
    set lnum 0

    # Get a line from the config file (header file)
    while {[gets $FILE line] >= 0} {
        # Replace all tabs with spaces before submitting line for testing
        regsub -all "\t" $line " " line

        if {([regexp "#define  $DEFINE_NAME " $line NEW_LINE]) || \
              ([regexp "#define $DEFINE_NAME "  $line NEW_LINE])  } {

            # Compare the old and new define values to see if replacement is necessary
            set OLD_DEFINE_VAL [get_string_from_line $line 1]

            if {$OLD_DEFINE_VAL == $NEW_DEFINE_VAL} {
               set REPLACE_SKIPPED 1
               set NEW_LINE $line
            }

            # If we couldn't find the old define value - flag an error and quit
            if {$OLD_DEFINE_VAL == "-1"} {
                exit
            }

            # Otherwise get ready to replace the old define value with the new one
            if {$REPLACE_SKIPPED != 1} {
                # Get the index of the old define value on the line
                set START_INDEX [get_index $line $OLD_DEFINE_VAL]

                # Find the length of the new line ==> #define DEFINE_NAME
                set DEFINE_LEN  [string length $NEW_LINE]

                # Pad the new line with spaces between the pound-def name and value
                while {$CNT < [expr {$START_INDEX - $DEFINE_LEN -1}] } {
                append NEW_LINE " "
                incr CNT
                }
                # Append the new define value on to the new line
                append NEW_LINE $NEW_DEFINE_VAL
            }
            # Add the new line to the temporary file and flag the success of the operation
            puts $NEW_FILE $NEW_LINE
            set REPLACE_SUCCESSFUL 1

        } else {
            # Else copy the all non-replacement target lines into the temporary file as is
            puts $NEW_FILE $line
        }
    }
    # clean up
    close $FILE
    close $NEW_FILE
    # Overwrite the existing header file with the new temporary file "temp"
    file copy -force   $tmpname $CFG_FILE

    # Delete the temporary file named "temp"
    file delete -force $tmpname

    if {$REPLACE_SUCCESSFUL == 0} {
        return 0
    } else {
        return 1
    }

}


proc get_string_from_line { LINE WS_NBR } {
   set WS_CNT 0
   set STRING_VALUE "-1"
   #puts "Inside get_string_from_line:\$WS_NBR = $WS_NBR, \$LINE = $LINE\n"
   while {[string length $LINE] && $WS_CNT <= $WS_NBR} {
        # Look for the next group of whitespace characters.
        if {[regexp -indices "\[ \t\n\]+" $LINE all]} {
            # Remove the text leading up to and including the white space
            # from the block.
            set text [string range $LINE 0 [lindex $all 1]]
            #puts "\$text = $text\n"
            set LINE [string range $LINE [expr {[lindex $all 1] + 1}] end]
            incr WS_CNT
            set STRING_VALUE $LINE
        } else {
            # Take everything up to the end of the block.
            #puts "Took everything up to end of line.\n"
            set text $LINE
            set LINE {}
        }
    }
    set TRIMMED_STRING_VALUE [string trim $STRING_VALUE]
    return $TRIMMED_STRING_VALUE
}

proc get_index {LINE COMPARE_STRING} {
   set CNT   0
   set LINE_LEN [string length $LINE]
   while {$CNT < $LINE_LEN } {
       set TRIMMED_LINE [string range $LINE $CNT $LINE_LEN]
       incr CNT
       if {$TRIMMED_LINE == $COMPARE_STRING} {
           return $CNT
       }
   }
   return 0
}
