#
# freertos_sw.tcl -- A description of FreeRTOS for
#                    Altera Nios II BSP tools
#

# Create a new operating system called "freertos"
create_os FreeRTOS

# Set UI display name
set_sw_property display_name "Real Time Engineers Ltd FreeRTOS 7.2.0"

# This OS "extends" HAL BSP type
set_sw_property extends_bsp_type HAL

# The version of this software
# Note: this reflects the version of the Altera release that this file
#       shipped in. The FreeRTOS version is 7.2.0
set_sw_property version 12.0

# Location in generated BSP that above sources will be copied into
# Suggestion: Keep this the same (case and all) as that where the OS
# source files came from. The Nios II FreeRTOS distribution had this
# as a root-directory name for the sources.
set_sw_property bsp_subdirectory FreeRTOS

# Interrupt properties: This OS supports ISR preemption
# (in its interrupt entry/exit code registered with
# macros in alt_hooks.h).
set_sw_property isr_preemption_supported true
  
#
# Source file listings...
#

# C source files
add_sw_property c_source FreeRTOS/src/alt_env_lock.c
add_sw_property c_source FreeRTOS/src/alt_malloc_lock.c
add_sw_property c_source FreeRTOS/src/alt_main.c
add_sw_property c_source FreeRTOS/src/croutine.c
add_sw_property c_source FreeRTOS/src/heap_3.c
add_sw_property c_source FreeRTOS/src/list.c
add_sw_property c_source FreeRTOS/src/port.c
add_sw_property c_source FreeRTOS/src/queue.c
add_sw_property c_source FreeRTOS/src/tasks.c
add_sw_property c_source FreeRTOS/src/timers.c
add_sw_property c_source FreeRTOS/src/tse_ethernet_phys.c

# ASM source files
add_sw_property asm_source FreeRTOS/src/port_asm.S

# Include files
add_sw_property include_source FreeRTOS/inc/croutine.h
add_sw_property include_source FreeRTOS/inc/FreeRTOS.h
add_sw_property include_source FreeRTOS/inc/FreeRTOSConfig.h
add_sw_property include_source FreeRTOS/inc/list.h
add_sw_property include_source FreeRTOS/inc/mpu_wrappers.h
add_sw_property include_source FreeRTOS/inc/os/alt_flag.h
add_sw_property include_source FreeRTOS/inc/os/alt_hooks.h
add_sw_property include_source FreeRTOS/inc/os/alt_sem.h
add_sw_property include_source FreeRTOS/inc/portable.h
add_sw_property include_source FreeRTOS/inc/portmacro.h
add_sw_property include_source FreeRTOS/inc/priv/alt_sem_freertos.h
add_sw_property include_source FreeRTOS/inc/projdefs.h
add_sw_property include_source FreeRTOS/inc/queue.h
add_sw_property include_source FreeRTOS/inc/semphr.h
add_sw_property include_source FreeRTOS/inc/StackMacros.h
add_sw_property include_source FreeRTOS/inc/task.h
add_sw_property include_source FreeRTOS/inc/timers.h

# Overridden HAL files
add_sw_property excluded_hal_source HAL/src/alt_env_lock.c
add_sw_property excluded_hal_source HAL/src/alt_malloc_lock.c
add_sw_property excluded_hal_source HAL/src/alt_exception_entry.S
add_sw_property excluded_hal_source HAL/src/alt_exception_trap.S
add_sw_property excluded_hal_source HAL/src/alt_irq_entry.S
add_sw_property excluded_hal_source HAL/src/alt_main.c
add_sw_property excluded_hal_source HAL/src/alt_software_exception.S
add_sw_property excluded_hal_source HAL/inc/os/alt_flag.h
add_sw_property excluded_hal_source HAL/inc/os/alt_hooks.h
add_sw_property excluded_hal_source HAL/inc/os/alt_sem.h

# Include paths
add_sw_property include_directory inc

# Makefile additions
add_sw_property alt_cppflags_addition "-D__freertos__"

# Register generate-time tcl script for OS_TICKS_PER_SEC definition
add_sw_property systemh_generation_script freertos_systemh_generation.tcl

#
# FreeRTOS Settings
#

#
# System settings
#
add_sw_setting boolean system_h_define system.preemption OS_USE_PREEMPTION 1 "Set to 1 to use the preemptive kernel, or 0 to use the cooperative kernel."

add_sw_setting boolean system_h_define system.idle_hook OS_USE_IDLE_HOOK 0 "Set to 1 if you wish to use an idle hook, or 0 to omit an idle hook."

add_sw_setting boolean system_h_define system.use_tick_hook OS_USE_TICK_HOOK 0 "Set to 1 if you wish to use an tick hook, or 0 to omit an tick hook."

add_sw_setting boolean system_h_define system.use_16_bit_ticks OS_USE_16_BIT_TICKS 0 "Time is measured in 'ticks' - which is the number of times the tick interrupt has executed since the kernel was started. The tick count is held in a variable of type portTickType. Defining configUSE_16_BIT_TICKS as 1 causes portTickType to be defined (typedef'ed) as an unsigned 16bit type. Defining configUSE_16_BIT_TICKS as 0 causes portTickType to be defined (typedef'ed) as an unsigned 32bit type.

Using a 16 bit type will greatly improve performance on 8 and 16 bit architectures, but limits the maximum specifiable time period to 65535 'ticks'. Therefore, assuming a tick frequency of 250Hz, the maximum time a task can delay or block when a 16bit counter is used is 262 seconds, compared to 17179869 seconds when using a 32bit counter."

add_sw_setting decimal_number system_h_define system.tick_rate_hz OS_TICK_RATE_HZ 1000 " -- ignored ... calculated on the go -- The frequency of the RTOS tick interrupt.
The tick interrupt is used to measure time. Therefore a higher tick frequency means time can be measured to a higher resolution. However, a high tick frequency also means that the kernel will use more CPU time so be less efficient. The RTOS demo applications all use a tick rate of 1000Hz. This is used to test the kernel and is higher than would normally be required.

More than one task can share the same priority. The kernel will share processor time between tasks of the same priority by switching between the tasks during each RTOS tick. A high tick rate frequency will therefore also have the effect of reducing the 'time slice' given to each task."

add_sw_setting boolean system_h_define system.idle_should_yield OS_IDLE_SHOULD_YIELD 0 "This parameter controls the behaviour of tasks at the idle priority. It only has an effect if:
The preemptive scheduler is being used.
The users application creates tasks that run at the idle priority.
Tasks that share the same priority will time slice. Assuming none of the tasks get preempted, it might be assumed that each task of at a given priority will be allocated an equal amount of processing time - and if the shared priority is above the idle priority then this is indeed the case.
 When tasks share the idle priority the behaviour can be slightly different. When configIDLE_SHOULD_YIELD is set to 1 the idle task will yield immediately should any other task at the idle priority be ready to run. This ensures the minimum amount of time is spent in the idle task when application tasks are available for scheduling. This behaviour can however have undesirable effects (depending on the needs of your application) as depicted below:


This diagram shows the execution pattern of four tasks at the idle priority. Tasks A, B and C are application tasks. Task I is the idle task. A context switch occurs with regular period at times T0, T1, ..., T6. When the idle task yields task A starts to execute - but the idle task has already taken up some of the current time slice. This results in task I and task A effectively sharing a time slice. The application tasks B and C therefore get more processing time than the application task A.

This situation can be avoided by:

If appropriate, using an idle hook in place of separate tasks at the idle priority.
Creating all application tasks at a priority greater than the idle priority.
Setting configIDLE_SHOULD_YIELD to 0.
Setting configIDLE_SHOULD_YIELD prevents the idle task from yielding processing time until the end of its time slice. This ensure all tasks at the idle priority are allocated an equal amount of processing time - but at the cost of a greater proportion of the total processing time being allocated to the idle task."

#
# Task Settings
#

add_sw_setting decimal_number system_h_define task.max_priorities OS_MAX_PRIORITIES 5 "The number of priorities available to the application tasks. Any number of tasks can share the same priority. Co-routines are prioritised separately - see configMAX_CO_ROUTINE_PRIORITIES.
Each available priority consumes RAM within the kernel so this value should not be set any higher than actually required by your application."

add_sw_setting decimal_number system_h_define task.minimal_stack_size OS_MINIMAL_STACK_SIZE 4096 "The size of the stack used by the idle task. Generally this should not be reduced from the value set in the FreeRTOSConfig.h file provided with the demo application for the port you are using."

add_sw_setting decimal_number system_h_define task.max_task_name_len OS_MAX_TASK_NAME_LEN 40 "The maximum permissible length of the descriptive name given to a task when the task is created. The length is specified in the number of characters including the NULL termination byte."

#
# Memmory Settings
# 
#add_sw_setting decimal_number system_h_define memory.total_heap_size OS_TOTAL_HEAP_SIZE 8388608 "The total amount of RAM available to the kernel.
#This value will only be used if your application makes use of one of the sample memory allocation schemes provided in the FreeRTOS source code download. See the memory configuration section for further details."

add_sw_setting boolean system_h_define memory.malloc_failed_hook OS_USE_MALLOC_FAILED_HOOK 0 "Defines whether vApplicationMallocFailedHook should be called on memory allocation failure"

add_sw_setting boolean system_h_define memory.thread_safe_malloc OS_THREAD_SAFE_NEWLIB 1 "Defines whether if we lock malloc with a recursive mutex so we it would be safe to malloc from different applications simultaniously"

add_sw_setting decimal_number system_h_define memory.check_for_stack_overflow OS_CHECK_FOR_STACK_OVERFLOW 2 "Stack Overflow Detection - Method 1
 It is likely that the stack will reach its greatest (deepest) value after the kernel has swapped the task out of the Running state because this is when the stack will contain the task context. At this point the kernel can check that the processor stack pointer remains within the valid stack space. The stack overflow hook function is called if the stack pointer contain a value that is outside of the valid stack range.
This method is quick but not guaranteed to catch all stack overflows. Set configCHECK_FOR_STACK_OVERFLOW to 1 to use this method only.


Stack Overflow Detection - Method 2

 When a task is first created its stack is filled with a known value. When swapping a task out of the Running state the kernel can check the last 16 bytes within the valid stack range to ensure that these known values have not been overwritten by the task or interrupt activity. The stack overflow hook function is called should any of these 16 bytes not remain at their initial value.
This method is less efficient than method one, but still fairly fast. It is very likely to catch stack overflows but is still not guaranteed to catch all overflows.

To use this method in combination with method 1 set configCHECK_FOR_STACK_OVERFLOW to 2. It is not possible to use only this method. 
"

#
# Debug Settings
#
add_sw_setting boolean system_h_define debug.use_trace_facility OS_USE_TRACE_FACILITY 1 "Set to 1 if you wish to include additional structure members and functions to assist with execution visualisation and tracing."


#
# Mutex Settings
#
add_sw_setting boolean system_h_define mutexes.use_mutexes OS_USE_MUTEXES 1 "Set to 1 to include mutex functionality in the build, or 0 to omit mutex functionality from the build. Readers should familiarise themselves with the differences between mutexes and binary semaphores in relation to the FreeRTOS functionality."

add_sw_setting boolean system_h_define mutexes.use_recursive_mutexes OS_USE_RECURSIVE_MUTEXES 1 "Set to 1 to include recursive mutex functionality in the build, or 0 to omit recursive mutex functionality from the build."

#
# Semaphore Settings
#
add_sw_setting boolean system_h_define semaphores.use_counting_semaphores OS_USE_COUNTING_SEMAPHORES 1 "Set to 1 to include counting semaphore functionality in the build, or 0 to omit counting semaphore functionality from the build."

#
# Queue Settings
#
add_sw_setting boolean system_h_define queue.queue_registery_size OS_QUEUE_REGISTRY_SIZE 0 "The queue registry has two purposes, both of which are associated with kernel aware debugging:
It allows a textual name to be associated with a queue for easy queue identification within a debugging GUI.
It contains the information required by a debugger to locate each registered queue and semaphore.
The queue registry has no purpose unless you are using a kernel aware debugger.
configQUEUE_REGISTRY_SIZE defines the maximum number of queues and semaphores that can be registered. Only those queues and semaphores that you want to view using a kernel aware debugger need be registered. See the API reference documentation for vQueueAddToRegistry() and vQueueUnregisterQueue() for more information."

add_sw_setting boolean system_h_define queue.use_alternative_api OS_USE_ALTERNATIVE_API 0 "The source code that implements the alternative (Alt) API is much simpler because it executes everything from within a critical section. This is the approach taken by many other RTOSes, but FreeRTOS.org has the preferred fully featured API too. The fully featured API has more complex code that takes longer to execute, but makes much less use of critical sections. Therefore the alternative API sacrifices interrupt responsiveness to gain execution speed, whereas the fully featured API sacrifices execution speed to ensure better interrupt responsiveness."

#
# Co-Routine Settings
#
add_sw_setting boolean system_h_define co_routines.use_co_routines OS_USE_CO_ROUTINES 0 "Set to 1 to include co-routine functionality in the build, or 0 to omit co-routine functionality from the build. To include co-routines croutine.c must be included in the project."

add_sw_setting decimal_number system_h_define co_routines.max_co_routine_priority OS_MAX_CO_ROUTINE_PRIORITIES 2 "The number of priorities available to the application co-routines. Any number of co-routines can share the same priority. Tasks are prioritised separately - see configMAX_PRIORITIES."

#
# Timer Settings
#
#add_sw_setting boolean system_h_define timers.use_timers OS_USE_TIMERS 1 "Set to 1 to include software timer functionality, or 0 to omit software timer functionality. See the FreeRTOS software timers page for a full description."

add_sw_setting decimal_number system_h_define timers.timer_task_priority OS_TIMER_TASK_PRIORITY 5 "Sets the priority of the software timer service/daemon task. See the FreeRTOS software timers page for a full description."

add_sw_setting decimal_number system_h_define timers.timer_queue_length OS_TIMER_QUEUE_LENGTH 10 "Sets the length of the software timer command queue. See the FreeRTOS software timers page for a full description."

#
# Include Settings
#
add_sw_setting boolean system_h_define include.inc_task_prio_set OS_vTaskPrioritySet 1 "The macros starting 'INCLUDE' allow those components of the real time kernel not utilized by your application to be excluded from your build. This ensures the RTOS does not use any more ROM or RAM than necessary for your particular embedded application."

add_sw_setting boolean system_h_define include.inc_task_prio_get OS_uxTaskPriorityGet 1 "The macros starting 'INCLUDE' allow those components of the real time kernel not utilized by your application to be excluded from your build. This ensures the RTOS does not use any more ROM or RAM than necessary for your particular embedded application."

add_sw_setting boolean system_h_define include.inc_task_delete OS_vTaskDelete 1 "The macros starting 'INCLUDE' allow those components of the real time kernel not utilized by your application to be excluded from your build. This ensures the RTOS does not use any more ROM or RAM than necessary for your particular embedded application."

add_sw_setting boolean system_h_define include.inc_task_cleanup_resources OS_vTaskCleanUpResources 0 "The macros starting 'INCLUDE' allow those components of the real time kernel not utilized by your application to be excluded from your build. This ensures the RTOS does not use any more ROM or RAM than necessary for your particular embedded application."

add_sw_setting boolean system_h_define include.inc_task_suspend OS_vTaskSuspend 1 "The macros starting 'INCLUDE' allow those components of the real time kernel not utilized by your application to be excluded from your build. This ensures the RTOS does not use any more ROM or RAM than necessary for your particular embedded application."

add_sw_setting boolean system_h_define include.inc_task_delay_util OS_vTaskDelayUntil 1 "The macros starting 'INCLUDE' allow those components of the real time kernel not utilized by your application to be excluded from your build. This ensures the RTOS does not use any more ROM or RAM than necessary for your particular embedded application."

add_sw_setting boolean system_h_define include.inc_task_delay OS_vTaskDelay 1 "The macros starting 'INCLUDE' allow those components of the real time kernel not utilized by your application to be excluded from your build. This ensures the RTOS does not use any more ROM or RAM than necessary for your particular embedded application."

add_sw_setting boolean system_h_define include.inc_task_get_stack OS_uxTaskGetStackHighWaterMark 1 "The macros starting 'INCLUDE' allow those components of the real time kernel not utilized by your application to be excluded from your build. This ensures the RTOS does not use any more ROM or RAM than necessary for your particular embedded application."

#
# Low level kernel settings
#
add_sw_setting decimal_number system_h_define kernel.kernel_irq_priority OS_KERNEL_INTERRUPT_PRIORITY 1 "Should not be changed and removed?"

add_sw_setting decimal_number system_h_define kernel.max_syscall_irq_priority OS_MAX_SYSCALL_INTERRUPT_PRIORITY 3 "Should not be changed and removed?"
