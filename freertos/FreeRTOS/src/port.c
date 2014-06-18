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

/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the NIOS2 port.
 *----------------------------------------------------------*/

/* Standard Includes. */
#include <string.h>
#include <errno.h>
#include <reent.h>

/* Altera includes. */
#include "sys/alt_irq.h"
#include "altera_avalon_timer_regs.h"
#include "priv/alt_irq_table.h"
#include "priv/alt_legacy_irq.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "projdefs.h"
#include "task.h"

extern void (*taskStart)(void);                 /* The entry point for all tasks. */

extern void *pxCurrentTCB;

/*-----------------------------------------------------------*/

static inline void prvReadGp( unsigned long *ulValue )
{
	asm volatile ( "stw gp, (%0)" : : "r"(ulValue) );
}
/*-----------------------------------------------------------*/

static volatile int interruptNesting = 0;

int getInterruptNesting(void)
{
	return interruptNesting;
}

void freertosIntEnter(void)
{
	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
	{
		alt_irq_context context = alt_irq_disable_all();

		if (interruptNesting < 255)
			++interruptNesting;

		alt_irq_enable_all(context);
	}
}

void freertosIntExit(void)
{
	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
	{
		alt_irq_context context = alt_irq_disable_all();

		if (interruptNesting > 0)
			--interruptNesting;

		if (interruptNesting == 0 && xTaskGetSchedulerState() == taskSCHEDULER_RUNNING)
			freertosIntContextSwitch();

		alt_irq_enable_all(context);
	}
}

/* 
 * See header file for description. 
 */
StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters )
{    
	StackType_t *pxFramePointer = pxTopOfStack - 1;
	StackType_t *pxStackPointer;
	pxStackPointer = pxFramePointer - 13;

	/* fill the stack frame. */

	pxStackPointer[12] = (StackType_t)pxCode;
	pxStackPointer[11] = (StackType_t)pvParameters;

	pxStackPointer[0] = ((StackType_t)&taskStart) + sizeof(StackType_t); /* exception return address (ea) */

	return pxStackPointer;
}
/*-----------------------------------------------------------*/

/* 
 * See header file for description. 
 */
BaseType_t xPortStartScheduler( void )
{
	// Just load the task which is currently revered to by TCB
    asm volatile (  " movia r2, freertosStartHighestPrioTask		\n"
                    " jmp r2                                          " );

    // This should never be reached
    return pdTRUE;
}
/*-----------------------------------------------------------*/

void vPortEndScheduler( void )
{
	// TODO reset the device?

	// It is unlikely that the NIOS2 port will require this function as there is nothing to return to.
}
/*-----------------------------------------------------------*/

void vPortYieldTask(void)
{
	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
	{
		alt_irq_context context = alt_irq_disable_all();

		portDISABLE_INTERRUPTS();

		freertosContextSwitch();

		alt_irq_enable_all(context);

#if 0
		printf("t: %p, sp: %p, fp: %p\n", pxCurrentTCB, GetStackPointer(), GetFramePointer());
#endif
	}
}

void* GetStackPointer()
{
	void *sp;
	asm volatile ("mov %0, sp" : "=r" (sp) );
	return sp;
}

void* GetFramePointer()
{
	void *fp;
	asm volatile ( "mov %0, fp" : "=r"(fp) );
	return fp;
}

void freertosTaskSwitchHook(void)
{
#if 0
	if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING)
		printf("t: %p, sp: %p, fp: %p\n", pxCurrentTCB, GetStackPointer(), GetFramePointer());
#endif
}

void vPortSysTickHandler()
{
	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
		xTaskIncrementTick();
}
