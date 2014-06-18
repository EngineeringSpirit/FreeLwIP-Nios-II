/*
    FreeRTOS V7.1.0 - Copyright (C) 2011 Real Time Engineers Ltd.
	

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS tutorial books are available in pdf and paperback.        *
     *    Complete, revised, and edited pdf reference manuals are also       *
     *    available.                                                         *
     *                                                                       *
     *    Purchasing FreeRTOS documentation will not only help you, by       *
     *    ensuring you get running as quickly as possible and with an        *
     *    in-depth knowledge of how to use FreeRTOS, it will also help       *
     *    the FreeRTOS project to continue with its mission of providing     *
     *    professional grade, cross platform, de facto standard solutions    *
     *    for microcontrollers - completely free of charge!                  *
     *                                                                       *
     *    >>> See http://www.FreeRTOS.org/Documentation for details. <<<     *
     *                                                                       *
     *    Thank you for using FreeRTOS, and thank you for your support!      *
     *                                                                       *
    ***************************************************************************


    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    >>>NOTE<<< The modification to the GPL is included to allow you to
    distribute a combined work that includes FreeRTOS without being obliged to
    provide the source code for proprietary components outside of the FreeRTOS
    kernel.  FreeRTOS is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/

/**
 * Modified by Engineering Spirit (c) 2012 http://engineering-spirit.nl to provide 
 * a more general support to configure the package with the BSP editor.
 *
 * Also fixed support for system clock detection in a more automated way.
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "system.h"

#include <assert.h>

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE. 
 *----------------------------------------------------------*/
#define configUSE_PREEMPTION						OS_USE_PREEMPTION
#define configUSE_IDLE_HOOK							OS_USE_IDLE_HOOK
#define configUSE_TICK_HOOK							OS_USE_TICK_HOOK
#define configTICK_RATE_HZ							( ( TickType_t ) OS_TICK_RATE_HZ )
#define configCPU_CLOCK_HZ							( ( unsigned long ) OS_CLK_FREQ )
#define configMAX_PRIORITIES						( ( BaseType_t ) OS_MAX_PRIORITIES )
#define configMINIMAL_STACK_SIZE					( OS_MINIMAL_STACK_SIZE )
#define configISR_STACK_SIZE						configMINIMAL_STACK_SIZE
#define configTOTAL_HEAP_SIZE						( ( size_t ) OS_TOTAL_HEAP_SIZE )
#define configMAX_TASK_NAME_LEN						( OS_MAX_TASK_NAME_LEN )
#define configUSE_TRACE_FACILITY					OS_USE_TRACE_FACILITY
#define configUSE_16_BIT_TICKS						OS_USE_16_BIT_TICKS
#define configIDLE_SHOULD_YIELD						OS_IDLE_SHOULD_YIELD
#define configUSE_MUTEXES							OS_USE_MUTEXES
#define configUSE_RECURSIVE_MUTEXES					OS_USE_RECURSIVE_MUTEXES
#define configUSE_COUNTING_SEMAPHORES				OS_USE_COUNTING_SEMAPHORES
#define configCHECK_FOR_STACK_OVERFLOW				OS_CHECK_FOR_STACK_OVERFLOW
#define configQUEUE_REGISTRY_SIZE					OS_QUEUE_REGISTER_SIZE
#define configUSE_ALTERNATIVE_API					OS_USE_ALTERNATIVE_API
#define configUSE_MALLOC_FAILED_HOOK				OS_USE_MALLOC_FAILED_HOOK
#define configUSE_NEWLIB_REENTRANT					OS_THREAD_SAFE_NEWLIB
#define configENABLE_BACKWARD_COMPATIBILITY			0

// Timer settings
#define configUSE_TIMERS                			1
#define INCLUDE_xTimerPendFunctionCall				1
#define configTIMER_TASK_PRIORITY       			OS_TIMER_TASK_PRIORITY
#define configTIMER_QUEUE_LENGTH        			OS_TIMER_QUEUE_LENGTH
#define configTIMER_TASK_STACK_DEPTH    			configMINIMAL_STACK_SIZE


// Event Settings
#define INCLUDE_xEventGroupSetBitFromISR			1

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 						OS_USE_CO_ROUTINES
#define configMAX_CO_ROUTINE_PRIORITIES 			OS_MAX_CO_ROUTINE_PRIORITIES

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet					OS_vTaskPrioritySet
#define INCLUDE_uxTaskPriorityGet					OS_uxTaskPriorityGet
#define INCLUDE_vTaskDelete							OS_vTaskDelete
#define INCLUDE_vTaskCleanUpResources				OS_vTaskCleanUpResources
#define INCLUDE_vTaskSuspend						OS_vTaskSuspend
#define INCLUDE_vTaskDelayUntil						OS_vTaskDelayUntil
#define INCLUDE_vTaskDelay							OS_vTaskDelay
#define INCLUDE_uxTaskGetStackHighWaterMark			OS_uxTaskGetStackHighWaterMark
#define INCLUDE_eTaskGetState						1
#define INCLUDE_pcTaskGetTaskName					1

/* The priority at which the tick interrupt runs.  This should probably be
kept at 1. */
#define configKERNEL_INTERRUPT_PRIORITY				OS_KERNEL_INTERRUPT_PRIORITY

/* The maximum interrupt priority from which FreeRTOS.org API functions can
be called.  Only API functions that end in ...FromISR() can be used within
interrupts. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY		OS_MAX_SYSCALL_INTERRUPT_PRIORITY

#if OS_USE_ASSERT_DEBUG
# define configASSERT( x )							assert( (x) )
#else
# define configASSERT( x )							((void*)x)
#endif

#define CONCAT(a,b)									a ## b

#define OS_CLOCK_BASE(clk)							CONCAT(clk,_BASE)
#define OS_CLOCK_FREQ(clk)							CONCAT(clk,_FREQ)
#define OS_CLOCK_IRQ(clk)							CONCAT(clk,_IRQ)
#define OS_CLOCK_IRQ_INTERRUPT_CONTROLLER_ID(clk)	CONCAT(clk,_IRQ_INTERRUPT_CONTROLLER_ID)

#define OS_CLK_BASE									OS_CLOCK_BASE(ALT_SYS_CLK)
#define OS_CLK_FREQ									OS_CLOCK_FREQ(ALT_SYS_CLK)
#define OS_CLK_IRQ									OS_CLOCK_IRQ(ALT_SYS_CLK)
#define OS_CLK_IRQ_INTERRUPT_CONTROLLER_ID			OS_CLOCK_IRQ_INTERRUPT_CONTROLLER_ID(ALT_SYS_CLK)

#endif /* FREERTOS_CONFIG_H */
