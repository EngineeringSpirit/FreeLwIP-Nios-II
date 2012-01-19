#ifndef __ALT_SEM_FREERTOS_H__
#define __ALT_SEM_FREERTOS_H__

// Provided by Engineering Spirit (c) 2012

/*
 * This file provides the FreeRTOS specific functions used to implement the 
 * macros in alt_sem.h. These functions are simply wrappers for the 
 * semaphore API.
 *
 * These functions are considered to be part of the internal implementation of
 * the HAL, and should not be called directly by application code or device
 * drivers. They are not guaranteed to be preserved in future versions of the
 * HAL. 
 */

#include "FreeRTOS.h"
#include "projdefs.h"
#include "semphr.h"
#include "alt_types.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*
 * alt_sem_create() is a wrapper for xSemaphoreCreateCounting(). The return value is 0 if 
 * the semaphore has been successfully created, or non-zero otherwise.
 */
static ALT_INLINE int ALT_ALWAYS_INLINE alt_sem_create (xSemaphoreHandle* sem, alt_u16 value)
{
	*sem = xSemaphoreCreateCounting(value ? value : 1, value);
	return *sem ? 0 : -1;
} 

/*
 * alt_sem_pend() is a wrapper for xSemaphoreTake(), with the error code 
 * converted into the functions return value.
 */

static ALT_INLINE int ALT_ALWAYS_INLINE alt_sem_pend (xSemaphoreHandle sem, alt_u16 timeout)
{
	return xSemaphoreTake(sem, timeout) ? 0 : -1;
}

#ifdef __cplusplus
}
#endif

#endif /* __ALT_SEM_FREERTOS_H__ */
