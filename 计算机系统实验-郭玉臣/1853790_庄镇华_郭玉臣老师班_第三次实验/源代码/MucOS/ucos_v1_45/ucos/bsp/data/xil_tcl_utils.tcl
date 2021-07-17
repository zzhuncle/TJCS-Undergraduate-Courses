##############################################################################
# Copyright 2013 Xilinx Inc. All rights reserved
##############################################################################

#
# Create configuration C file as required by Xilinx Zynq drivers
# Similar to proc xdefine_config_file, except that uses regsub
# to replace "S_AXI_" with ""
#
namespace import ::hsi::utils::*

proc xdefine_zynq_config_file_intr {drv_handle file_name drv_string args} {
   set args [get_exact_arg_list $args]
   set filename [file join "src" $file_name] 
   #Fix for CR 784758
   #file delete $filename
   set config_file [open $filename w]
   xprint_generated_header $config_file "Driver configuration"    
   puts $config_file "#include \"xparameters.h\""
   puts $config_file "#include \<ucos_bsp.h\>"
   puts $config_file "#include \"[string tolower $drv_string].h\""
   puts $config_file "\n/*"
   puts $config_file "* The configuration table for devices"
   puts $config_file "*/\n"
   puts $config_file [format "%s_Config %s_ConfigTable\[\] =" $drv_string $drv_string]
   puts $config_file "\{"
   set periphs [get_common_driver_ips $drv_handle]     
   set start_comma ""
   foreach periph $periphs {
       puts $config_file [format "%s\t\{" $start_comma]
       set comma ""
       foreach arg $args {
           # Check if this is a driver parameter or a peripheral parameter
           set value [get_property CONFIG.$arg $drv_handle]
           if {[llength $value] == 0} {
            set local_value [get_property CONFIG.$arg $periph ]
            # If a parameter isn't found locally (in the current
            # peripheral), we will (for some obscure and ancient reason)
            # look in peripherals connected via point to point links
            if { [string compare -nocase $local_value ""] == 0} { 
               set p2p_name [get_p2p_name $periph $arg]
               if { [string compare -nocase $p2p_name ""] == 0} {
                   set arg_name [get_ip_param_name $periph $arg]
                   regsub "S_AXI_" $arg_name "" arg_name
                   puts -nonewline $config_file [format "%s\t\t%s" $comma $arg_name]
               } else {
                   regsub "S_AXI_" $p2p_name "" p2p_name
                   puts -nonewline $config_file [format "%s\t\t%s" $comma $p2p_name]
               }
           } else {
               set arg_name [get_ip_param_name $periph $arg]
               regsub "S_AXI_" $arg_name "" arg_name
               puts -nonewline $config_file [format "%s\t\t%s" $comma $arg_name]
                   }
           } else {
               set arg_name [get_driver_param_name $drv_string $arg]
               regsub "S_AXI_" $arg_name "" arg_name
               puts -nonewline $config_file [format "%s\t\t%s" $comma $arg_name]
           }
           set comma ",\n"
       }
       
       set arg_name [get_ip_param_name $periph "INT_SOURCE"]
       puts -nonewline $config_file [format "%s\t\t%s" $comma $arg_name]
       
       puts -nonewline $config_file "\n\t\}"
       set start_comma ",\n"
   }
   puts $config_file "\n\};"

   puts $config_file "\n";

   close $config_file
}


#
# Create configuration C file as required by Xilinx  drivers
#
proc xdefine_config_file_intr {drv_handle file_name drv_string args} {
    set args [get_exact_arg_list $args]
    set filename [file join "src" $file_name] 
    #Fix for CR 784758
    #file delete $filename
    set config_file [open $filename w]
    write_c_header $config_file "Driver configuration"    
    puts $config_file "#include \"xparameters.h\""
    puts $config_file "#include \<ucos_bsp.h\>"
    puts $config_file "#include \"[string tolower $drv_string].h\""
    puts $config_file "\n/*"
    puts $config_file "* The configuration table for devices"
    puts $config_file "*/\n"
    puts $config_file [format "%s_Config %s_ConfigTable\[\] =" $drv_string $drv_string]
    puts $config_file "\{"
    set periphs [get_common_driver_ips $drv_handle]     
    set start_comma ""
    foreach periph $periphs {
        puts $config_file [format "%s\t\{" $start_comma]
        set comma ""
        foreach arg $args {
            if {[string compare -nocase "DEVICE_ID" $arg] == 0} {
                puts -nonewline $config_file [format "%s\t\t%s,\n" $comma [get_ip_param_name $periph $arg]]
                continue
            }
            # Check if this is a driver parameter or a peripheral parameter
            set value [get_property CONFIG.$arg $drv_handle]
            if {[llength $value] == 0} {
                set local_value [get_property CONFIG.$arg $periph ]
                # If a parameter isn't found locally (in the current
                # peripheral), we will (for some obscure and ancient reason)
                # look in peripherals connected via point to point links
                if { [string compare -nocase $local_value ""] == 0} { 
                    set p2p_name [get_p2p_name $periph $arg]
                    if { [string compare -nocase $p2p_name ""] == 0} {
                        puts -nonewline $config_file [format "%s\t\t%s" $comma [get_ip_param_name $periph $arg]]
                    } else {
                        puts -nonewline $config_file [format "%s\t\t%s" $comma $p2p_name]
                    }
                } else {
                    puts -nonewline $config_file [format "%s\t\t%s" $comma [get_ip_param_name $periph $arg]]
                }
            } else {
                puts -nonewline $config_file [format "%s\t\t%s" $comma [get_driver_param_name $drv_string $arg]]
            }
            set comma ",\n"
        }
        
       set arg_name [get_ip_param_name $periph "INT_SOURCE"]
       puts -nonewline $config_file [format "%s\t\t%s" $comma $arg_name]
       
        puts -nonewline $config_file "\n\t\}"
        set start_comma ",\n"
    }
    puts $config_file "\n\};"

    puts $config_file "\n";

    close $config_file
}
