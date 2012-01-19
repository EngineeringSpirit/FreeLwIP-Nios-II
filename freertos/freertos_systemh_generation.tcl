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

# Call procedure to generate OS_TICKS_PER_SEC macro
create_os_ticks_per_sec_macro

