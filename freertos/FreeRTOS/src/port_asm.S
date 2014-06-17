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

#include <system.h>

        .text

/*********************************************************************************************************
 *                                PERFORM A CONTEXT SWITCH
 *                                           void freertosContextSwitch(void)    - from task level
 *                                           void freertosIntContextSwitch(void) - from interrupt level
 *
 * Note(s): 1) Upon entry, 
 *             OSTCBCur     points to the OS_TCB of the task to suspend
 *             OSTCBHighRdy points to the OS_TCB of the task to resume
 *
 *********************************************************************************************************/
			.global freertosIntContextSwitch
			.global freertosContextSwitch


freertosIntContextSwitch:
freertosContextSwitch:

      /* 
       * Save the remaining registers to the stack. 
       */

      addi sp, sp, -44

#ifdef ALT_STACK_CHECK

      bltu  sp, et, .Lstack_overflow

#endif

      ldw r4, %gprel(pxCurrentTCB)(gp)

      stw ra,  0(sp)
      stw fp,  4(sp)
      stw r23, 8(sp)
      stw r22, 12(sp)
      stw r21, 16(sp)
      stw r20, 20(sp)
      stw r19, 24(sp)
      stw r18, 28(sp)
      stw r17, 32(sp)
      stw r16, 36(sp)

      /*
       * Save the current tasks stack pointer into the current tasks OS_TCB.
       * i.e. pxCurrentTCB->pxTopOfStack = sp;
       */
      stw sp, (r4)                  /* save the stack pointer (OSTCBStkPtr */
                                    /* is the first element in the OS_TCB  */
                                    /* structure.                          */

      /*
       * Call the user definable freertosTaskSwitchHook()
       */
      call freertosTaskSwitchHook

0:

9:

	  call vTaskSwitchContext

      ldw r4, %gprel(pxCurrentTCB)(gp)

      /*
       * Set the stack pointer to point to the new task's stack
       */
      ldw sp, (r4) /* the stack pointer is the first entry in the OS_TCB structure */

#if defined(ALT_STACK_CHECK) && (OS_TASK_CREATE_EXT_EN > 0)

      ldw et, 8(r4)                  /* load the new stack limit */

#endif

      /*
       * Restore the saved registers for the new task.
       */
      ldw ra,  0(sp)
      ldw fp,  4(sp)
      ldw r23, 8(sp)
      ldw r22, 12(sp)
      ldw r21, 16(sp)
      ldw r20, 20(sp)
      ldw r19, 24(sp)
      ldw r18, 28(sp)
      ldw r17, 32(sp)
      ldw r16, 36(sp)

#if defined(ALT_STACK_CHECK) && (OS_TASK_CREATE_EXT_EN > 0)

      stw et, %gprel(alt_stack_limit_value)(gp)

#endif

      addi sp, sp, 44
      /*
       * resume execution of the new task.
       */

      ret

#ifdef ALT_STACK_CHECK

.Lstack_overflow:
        break 3

#endif

.set freertosContextSwitch_SWITCH_PC,0b-freertosContextSwitch

/*********************************************************************************************************
 *                                        START THE HIGHEST PRIORITY TASK
 *                                           void OSStartHighRdy(void)
 *
 * Note(s): 1) Upon entry,
 *             OSTCBCur     points to the OS_TCB of the task to suspend
 *             OSTCBHighRdy points to the OS_TCB of the task to resume
 *
 *********************************************************************************************************/
      .global freertosStartHighestPrioTask

freertosStartHighestPrioTask:
      /*
       * disable interrupts so that the scheduler doesn't run while
       * we're initialising this task.
       */
      rdctl r18, status
      subi  r17, zero, 2   /* r17 = 0xfffffffe */
      and   r18, r18, r17
      wrctl status, r18

      /*
       * Call the user definable freertosTaskSwitchHook()
       */
      call freertosTaskSwitchHook

#if defined(ALT_STACK_CHECK) && (OS_TASK_CREATE_EXT_EN == 0)

      mov et, zero                   /* Don't check stack limits */
      stw et, %gprel(alt_stack_limit_value)(gp)

#endif

      /*
       * start execution of the new task.
       */
      br 9b

/*********************************************************************************************************
 *                                        CALL THE TASK INITILISATION FUNCTION
 *                                           void taskStart(void)
 *********************************************************************************************************/

      .global taskStart

taskStart:
      /* This instruction is never executed.  Its here to make the
       * backtrace work right 
       */
      movi sp, 0

      /* Enable interrupts */
      rdctl r5, status
      ori   r5, r5, 0x1
      wrctl status, r5

	  /* Load the function and it's argument */
      ldw r2, 4(sp)
      ldw r4, 0(sp)
      addi sp, sp, 8

	  /* Call the function passing the argument */
      callr r2

      nop
