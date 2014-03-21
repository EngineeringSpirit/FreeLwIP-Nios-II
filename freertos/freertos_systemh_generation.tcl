#
# freertos_systemh_generation.tcl
#
proc create_os_ticks_per_sec_macro {} {
    # Get BSP system timer name
    set timer_name [get_setting hal.sys_clk_timer]
    
    # Convert to upper-case
    set timer_name [string toupper $timer_name]
    
    #
    # Bail if not assigned
    #
    # If timer is assigned, add system.h macro. Should look like:
    #   "#define OS_TICKS_PER_SEC <timer name upper case>_TICKS_PER_SEC
    if {[string compare $timer_name "NONE"] == 0} {
        puts "ERROR: FreeRTOS system.h generation tcl script:"
        puts " --> HAL system timer not defined. FreeRTOS may not build!"
        puts " --> Set hal.sys_clk_timer to a timer device and then re-generate the BSP."
    } else {
        set tps_str _TICKS_PER_SEC
        set tps_setting $timer_name$tps_str
	
        # Add to system.h content for FreeRTOS OS
        add_systemh_line FreeRTOS OS_TICKS_PER_SEC $tps_setting
    }
}

proc resolve_ethernet_macs {} {
    set phy_count 0

	foreach slave [get_slave_descs] {
		set module [get_module_name $slave]
		
		#if [is_ethernet_mac_device $slave] {
			if {[scan $slave "tse_mac_%d" nr] != 1} {
				continue
			}

			# see if we can find an SGDMA_TSE_TX_# and SGDMA_TSE_RX_# and TSE_DESCRIPTOR_MEMORY_#
			set found_tx 0
			set found_rx 0
			set found_descriptor 0

			set rx_challenge [format "sgdma_tse_rx_%d" $nr]
			set tx_challenge [format "sgdma_tse_tx_%d" $nr]
			set descriptor_challenge [format "tse_descriptor_memory_%d" $nr]

			foreach tmp [get_slave_descs] {
				if {[string compare $tmp $rx_challenge] == 0} {
					set found_rx 1
				} elseif {[string compare $tmp $tx_challenge] == 0} {
					set found_tx 1
				} elseif {[string compare $tmp $descriptor_challenge] == 0} {
					set found_descriptor 1
				}
			}

			if {$found_tx == 0} {
				puts "ERROR: Couldn't find a matching transmit SGDMA controller for $slave"
			}

			if {$found_rx == 0} {
				puts "ERROR: Couldn't find a matching receive SGDMA controller for $slave"
			}

			if {$found_descriptor == 0} {
				puts "ERROR: Couldn't find matching descriptor memory for $slave"
			}

			if {$found_tx == 0 || $found_rx == 0 || $found_descriptor == 0} {
				continue;
			}

			puts "INFO: Adding ethernet MAC $slave to the TSE driver"

			add_systemh_line FreeRTOS [format "GOT_TSE_MAC_%d" $nr] 1

			set phy_count [expr $phy_count + 1]
		#} else {
		#	set bla [is_ethernet_mac_device $slave]
		#	puts "ERROR: Not an TSE MAC: $slave - $bla"
		#}
	}
	
    # we have tell the system
    if { $phy_count != 0 } {
		add_systemh_line FreeRTOS TSE_MY_SYSTEM 1
    }
	
	add_systemh_line FreeRTOS PHY_COUNT $phy_count
}

# Call procedure to generate OS_TICKS_PER_SEC macro
create_os_ticks_per_sec_macro
resolve_ethernet_macs
