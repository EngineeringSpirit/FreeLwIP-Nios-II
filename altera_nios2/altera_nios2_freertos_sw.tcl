#
# altera_nios2_driver_freertos.tcl
#

# Create a new driver
create_driver altera_nios2_freertos_driver

# Associate it with some hardware known as "altera_nios2"
set_sw_property hw_class_name altera_nios2

# The version of this driver
set_sw_property version 12.0

# This driver may be incompatible with versions of hardware less
# than specified below. Updates to hardware and device drivers
# rendering the driver incompatible with older versions of
# hardware are noted with this property assignment.
set_sw_property min_compatible_hw_version 8.0

# Initialize the driver in alt_irq_init() if this module
# is recognized as containing an interrupt controller.
set_sw_property irq_auto_initialize true

# Location in generated BSP that above sources will be copied into
set_sw_property bsp_subdirectory HAL


# This driver supports the FreeRTOS BSP (OS) type
add_sw_property supported_bsp_type FreeRTOS

# This uses the $argv0 pre-set variable which contains the
# complete path to this script.
set dir [file dirname $argv0]

#
# Source file listings...
#

# C/C++ source files
add_sw_property c_source HAL/src/altera_nios2_irq.c

# Include files
add_sw_property include_source HAL/inc/altera_nios2_irq.h

# HAL driver common settings 
source $dir/altera_nios2_hal_common.tcl

# uC/OS-II driver common settings 
source $dir/altera_nios2_freertos_common.tcl

# End of file
