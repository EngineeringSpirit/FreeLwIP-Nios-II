/*
    FreeRTOS V8.0.1 - Copyright (C) 2014 Real Time Engineers Ltd.
	

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

#ifndef PORTMACRO_H
#define PORTMACRO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <alt_types.h>
#include "sys/alt_irq.h"

/*-----------------------------------------------------------
 * Port specific definitions.  
 *
 * The settings in this file configure FreeRTOS correctly for the
 * given hardware and compiler.
 *
 * These settings should not be altered.
 *-----------------------------------------------------------
 */

typedef long						BaseType_t;
typedef unsigned long				UBaseType_t;
typedef uint32_t					StackType_t;

extern void freertosContextSwitch(void);
extern void freertosIntContextSwitch(void);

extern void* GetStackPointer(void);
extern void* GetFramePointer(void);

extern void vPortYieldTask(void);

#if( configUSE_16_BIT_TICKS == 1 )
typedef uint16_t					TickType_t;
#define portMAX_DELAY				(TickType_t) 0xffff
#else
typedef uint32_t					TickType_t;
#define portMAX_DELAY				(TickType_t) 0xffffffff
#endif
/*-----------------------------------------------------------*/	

/* Architecture specifics. */
#define portSTACK_GROWTH				( -1 )
#define portTICK_RATE_MS				( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT				4
#define portCRITICAL_NESTING_IN_TCB		1
/*-----------------------------------------------------------*/	

extern void vTaskSwitchContext( void );

#define portYIELD()									vPortYieldTask()

// No yield from ISR required since when we exit the ISR we will always select the next available higher task
#define portEND_SWITCHING_ISR( xSwitchRequired ) 	((void*)xSwitchRequired) //if( xSwitchRequired ) 	vTaskSwitchContext()
#define portYIELD_FROM_ISR( xSwitchRequired ) 		((void*)xSwitchRequired) //portEND_SWITCHING_ISR( xSwitchRequired )

/*-----------------------------------------------------------*/

extern void vTaskEnterCritical( void );
extern void vTaskExitCritical( void );

/**
 * If you look at alt_irq_disable_all() all it does is disable the PIE
 * There is no need to write back the last context.
 */
static ALT_INLINE void ALT_ALWAYS_INLINE mod_irq_enable_all (void)
{
  alt_irq_context context;
  NIOS2_READ_STATUS (context);
  NIOS2_WRITE_STATUS (context | NIOS2_STATUS_PIE_MSK);
}

#if 0
extern void enh_alt_irq_disable_all( void );
extern void enh_alt_irq_enable_all( void );
#endif

#define portDISABLE_INTERRUPTS()	alt_irq_disable_all()
#define portENABLE_INTERRUPTS()		mod_irq_enable_all()
#define portENTER_CRITICAL()        if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) vTaskEnterCritical()
#define portEXIT_CRITICAL()         if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) vTaskExitCritical()
/*-----------------------------------------------------------*/

/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) void vFunction( void *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void *pvParameters )

#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */

