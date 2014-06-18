#ifndef __ALT_HOOKS_H__
#define __ALT_HOOKS_H__

// Provided by Engineering Spirit (c) 2012
 
/*
 * This file is included by the Altera Vectored Interrpt Controller's 
 * interrpt funnel assembly code. Only those macros relevant to the funnel 
 * should be seen by the assembler. The funnel code defines the ALT_ASM_SRC 
 * macro. 
 */
#ifndef ALT_ASM_SRC

#include "FreeRTOS.h"
#include "semphr.h"

extern void freertosIntEnter(void);
extern void freertosIntExit(void);

/*
 * Tick handler for FreeRTOS
 */
extern void vPortSysTickHandler(void);

/*
 * Thisheader provides definitions for the operating system hooks used by the HAL.
 */

#define ALT_OS_TIME_TICK	vPortSysTickHandler
#define ALT_OS_INIT()
#define ALT_OS_STOP()		vPortEndScheduler

#define ALT_OS_INT_ENTER	freertosIntEnter
#define ALT_OS_INT_EXIT		freertosIntExit

#endif /* ALT_ASM_SRC */

/* These macros are used by the VIC funnel assembly code */
#define ALT_OS_INT_ENTER_ASM    call freertosIntEnter
#define ALT_OS_INT_EXIT_ASM     call freertosIntExit

#endif /* __ALT_HOOKS_H__ */
