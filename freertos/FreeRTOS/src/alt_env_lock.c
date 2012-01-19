/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2003 Altera Corporation, San Jose, California, USA.           *
* All rights reserved.                                                        *
*                                                                             *
* Permission is hereby granted, free of charge, to any person obtaining a     *
* copy of this software and associated documentation files (the "Software"),  *
* to deal in the Software without restriction, including without limitation   *
* the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
* and/or sell copies of the Software, and to permit persons to whom the       *
* Software is furnished to do so, subject to the following conditions:        *
*                                                                             *
* The above copyright notice and this permission notice shall be included in  *
* all copies or substantial portions of the Software.                         *
*                                                                             *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
* DEALINGS IN THE SOFTWARE.                                                   *
*                                                                             *
* This agreement shall be governed in all respects by the laws of the State   *
* of California and by the laws of the United States of America.              *
*                                                                             *
******************************************************************************/

// Provided by Engineering Spirit (c) 2012

#include "system.h"

/*
 * These are the malloc lock/unlock stubs required by newlib. These are 
 * used to make accesses to the heap thread safe. Note that 
 * this implementation requires that the heap is never manipulated 
 * by an interrupt service routine.
 */

#include <reent.h>

#include "FreeRTOS.h"
#include "projdefs.h"
#include "semphr.h"
#include "task.h"

/* semaphore to protect the heap */

xSemaphoreHandle alt_envsem;

/* __env_lock needs to provide recursive mutex locking */

void __env_lock ( struct _reent *_r )
{
#if OS_THREAD_SAFE_NEWLIB
	if (!xTaskGetSchedulerState())
		return;

	// wait for the mutex to be released
	while (xSemaphoreTakeRecursive(alt_envsem, 10) != pdTRUE)
		vTaskDelay(1);

#endif /* OS_THREAD_SAFE_NEWLIB */
	return;
}

/* __env_unlock needs to provide recursive mutex unlocking */

void __env_unlock ( struct _reent *_r )
{
#if OS_THREAD_SAFE_NEWLIB
	if (!xTaskGetSchedulerState())
		return;
	  
	xSemaphoreGiveRecursive(alt_envsem);
#endif /* OS_THREAD_SAFE_NEWLIB */
}

