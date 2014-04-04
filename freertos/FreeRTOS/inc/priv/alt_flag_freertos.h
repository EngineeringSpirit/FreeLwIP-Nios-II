#ifndef __ALT_FLAG_FREERTOS_H__
#define __ALT_FLAG_FREERTOS_H__

// Provided by Engineering Spirit (c) 2012

/*
 * This file provides the FreeRTOS specific functions used to implement the
 * macros in alt_flag.h. These functions are simply wrappers for the
 * event group API.
 *
 * These functions are considered to be part of the internal implementation of
 * the HAL, and should not be called directly by application code or device
 * drivers. They are not guaranteed to be preserved in future versions of the
 * HAL.
 */

#include <stdlib.h>

#include "FreeRTOS.h"
#include "projdefs.h"
#include "event_groups.h"
#include "alt_types.h"
#include <os/flags.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

//#include "alt_flag.h"

/*
 * alt_event_create() is a wrapper for xEventGroupCreate(). The return value is 0 if
 * the semaphore has been successfully created, or non-zero otherwise.
 */
static ALT_INLINE int ALT_ALWAYS_INLINE alt_event_create (EventGroupHandle_t* groupHandler)
{
	*groupHandler = xEventGroupCreate();
	return (*groupHandler != NULL) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static ALT_INLINE int ALT_ALWAYS_INLINE alt_event_pend(EventGroupHandle_t group, BaseType_t flags, BaseType_t wait_type, TickType_t timeout)
{
	if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED)
		return -1;

	return (xEventGroupWaitBits(group, flags, HAS_FLAG(wait_type, OS_FLAG_CONSUME), HAS_FLAG(wait_type, OS_FLAG_WAIT_SET_ANY), timeout) == pdTRUE) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static ALT_INLINE int ALT_ALWAYS_INLINE alt_event_post(EventGroupHandle_t group, BaseType_t flags, BaseType_t opts)
{
	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
		if (HAS_FLAG(opts, OS_FLAG_SET)) {
			BaseType_t higherPriorityTaskWoken;
			BaseType_t result = xEventGroupSetBitsFromISR(group, flags, &higherPriorityTaskWoken);
			if (result == pdTRUE && higherPriorityTaskWoken)
				portYIELD_FROM_ISR(higherPriorityTaskWoken);
		} else if (HAS_FLAG(opts, OS_FLAG_CLR)) {
			xEventGroupClearBitsFromISR(group, flags);
		} else {
			NIOS2_BREAK();
		}

		return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;
}

#ifdef __cplusplus
}
#endif

#endif /* __ALT_FLAG_FREERTOS_H__ */
