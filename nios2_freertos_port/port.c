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

/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the NIOS2 port.
 *----------------------------------------------------------*/

/* Standard Includes. */
#include <string.h>
#include <errno.h>

/* Altera includes. */
#include "sys/alt_irq.h"
#include "altera_avalon_timer_regs.h"
#include "priv/alt_irq_table.h"
#include "priv/alt_legacy_irq.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "projdefs.h"
#include "task.h"

/* Interrupts are enabled. */
#define portINITIAL_ESTATUS     ( portSTACK_TYPE ) 0x01 

/*-----------------------------------------------------------*/

static inline void prvReadGp( unsigned long *ulValue )
{
	asm volatile ( "stw gp, (%0)" : : "r"(ulValue) );
}
/*-----------------------------------------------------------*/

static volatile alt_irq_context lastContext;

void enh_alt_irq_disable_all()
{
	alt_irq_context ctxt = alt_irq_disable_all();
	lastContext |= ctxt;
}

void enh_alt_irq_enable_all()
{
	alt_irq_context restore = lastContext;
	lastContext = 0;
	alt_irq_enable_all(restore);
}

/* 
 * See header file for description. 
 */
portSTACK_TYPE *pxPortInitialiseStack( portSTACK_TYPE *pxTopOfStack, pdTASK_CODE pxCode, void *pvParameters )
{    
portSTACK_TYPE *pxFramePointer = pxTopOfStack - 1;
portSTACK_TYPE xGlobalPointer;

    prvReadGp( &xGlobalPointer ); 

    /* End of stack marker. */
    *pxTopOfStack = 0xdeadbeef;
    pxTopOfStack--;

    *pxTopOfStack = ( portSTACK_TYPE ) pxFramePointer;
    pxTopOfStack--;
    
    *pxTopOfStack = xGlobalPointer; 
    
    /* Space for R23 to R16. */
    pxTopOfStack -= 9;

    *pxTopOfStack = ( portSTACK_TYPE ) pxCode;
    pxTopOfStack--;

    *pxTopOfStack = portINITIAL_ESTATUS;

    /* Space for R15 to R5. */
    pxTopOfStack -= 12;

    *pxTopOfStack = ( portSTACK_TYPE ) pvParameters;

    /* Space for R3 to R1, muldiv and RA. */
    pxTopOfStack -= 5;

    /* Initialize stack pointer to prevent debugger crash. */
    *pxTopOfStack = ( portSTACK_TYPE ) xPortStartScheduler;

    return pxTopOfStack;
}
/*-----------------------------------------------------------*/

/* 
 * See header file for description. 
 */
portBASE_TYPE xPortStartScheduler( void )
{
	// Just load the task which is currently revered to by TCB
    asm volatile (  " movia r2, restore_sp_from_pxCurrentTCB        \n"
                    " jmp r2                                          " );

    // This should never be reached
    return 0;
}
/*-----------------------------------------------------------*/

void vPortEndScheduler( void )
{
	// It is unlikely that the NIOS2 port will require this function as there is nothing to return to.
}
/*-----------------------------------------------------------*/

void vPortSysTickHandler( void )
{
	// only make ticks if the scheduler has been started.
	if ( xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED )
	{
		// Increment the Kernel Tick.
		vTaskIncrementTick();

		// If using preemption, also force a context switch.
#if configUSE_PREEMPTION == 1
		vTaskSwitchContext();
#endif
	}
}
/*-----------------------------------------------------------*/
