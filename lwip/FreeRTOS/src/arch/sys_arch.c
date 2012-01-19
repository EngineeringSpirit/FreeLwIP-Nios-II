/**
 * \file sys_arch.c
 *
 * The FreeRTOS port used by LwIP
 *
 * Created by Engineering Spirit (c) 2012 http://engineering-spirit.nl
 */

 // include system headers
#include <sys/time.h>
#include <sys/alt_timestamp.h>

// include lwip headers
#include <lwip/opt.h>
#include <lwip/sys.h>
#include <lwip/arch.h>
#include <lwip/err.h>
#include <lwip/stats.h>

// LwIP FreeRTOS port
void sys_init(void)
{
	return;
}

u32_t sys_now(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);

	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

#if ! NO_SYS
// ---------------------------------- Semaphore stuff --------------------------------------------
/**
 * Creates and returns a new semaphore. The "count" argument specifies
 * the initial state of the semaphore.
 */
err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
	// count 0 means a binary semaphore so max value should be 1
	*sem = xSemaphoreCreateCounting((count ? count : 1), count);

	if (*sem == NULL) {
		SYS_STATS_INC(sem.err);
		return ERR_MEM;  // TBD need assert
	}

	SYS_STATS_INC_USED(sem);

	return ERR_OK;
}

/**
 * Deallocates a semaphore.
 */
void sys_sem_free(sys_sem_t *sem)
{
	vQueueDelete(*sem);

	SYS_STATS_DEC(sem.used);
}

/**
 * Signals a semaphore.
 */
void sys_sem_signal(sys_sem_t *sem)
{
	xSemaphoreGive(*sem);
}

/**
 * Blocks the thread while waiting for the semaphore to be
 * signaled. If the "timeout" argument is non-zero, the thread should
 * only be blocked for the specified time (measured in
 * milliseconds). If the "timeout" argument is zero, the thread should be
 * blocked until the semaphore is signaled.
 *
 * If the timeout argument is non-zero, the return value is the number of
 * milliseconds spent waiting for the semaphore to be signaled. If the
 * semaphore wasn't signaled within the specified time, the return value is
 * SYS_ARCH_TIMEOUT. If the thread didn't have to wait for the semaphore
 * (i.e., it was already signaled), the function may return zero.
 *
 * Notice that lwIP implements a function with a similar name,
 * sys_sem_wait(), that uses the sys_arch_sem_wait() function.
 */
u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
	portTickType nStartTime, nEndTime, nElapsed;

	nStartTime = xTaskGetTickCount();

	if (timeout != 0)
	{
		if (xSemaphoreTake(*sem, timeout) == pdTRUE)
		{
			nEndTime = xTaskGetTickCount();
			nElapsed = nEndTime - nStartTime;

			if (nElapsed == 0)
				nElapsed = 1;

			return nElapsed; // return time blocked TBD test
		}
		else
			return SYS_ARCH_TIMEOUT;
	}
	else // must block without a timeout
	{
		while (xSemaphoreTake(*sem, 10000) != pdTRUE);

		nEndTime = xTaskGetTickCount();
		nElapsed = nEndTime - nStartTime;
		if( nElapsed == 0 )
			nElapsed = 1;

		return nElapsed; // return time blocked
	}
}

// ------------------------------------ Mutex Stuff -----------------------------------------------
#if ! LWIP_COMPAT_MUTEX
/** Create a new mutex
 * @param mutex pointer to the mutex to create
 * @return a new mutex */
err_t sys_mutex_new(sys_mutex_t *mutex)
{
	*mutex = xSemaphoreCreateMutex();

	if (!*mutex) {
		SYS_STATS_INC(mutex.err);
		return ERR_MEM;
	}

	SYS_STATS_INC_USED(mutex);

	return ERR_OK;
}

/** Lock a mutex
 * @param mutex the mutex to lock */
void sys_mutex_lock(sys_mutex_t *mutex)
{
	// wait on mutex forever
	while (xSemaphoreTake(*mutex, 10000) != pdTRUE);
}

/** Unlock a mutex
 * @param mutex the mutex to unlock */
void sys_mutex_unlock(sys_mutex_t *mutex)
{
	xSemaphoreGive(*mutex);
}

/** Delete a semaphore
 * @param mutex the mutex to delete */
void sys_mutex_free(sys_mutex_t *mutex)
{
	vQueueDelete(*mutex);
	SYS_STATS_DEC(mutex.used);
}
#endif /* LWIP_COMPAT_MUTEX */

// ---------------------------------- MessageBox stuff --------------------------------------------
/**
 * Creates an empty mailbox for maximum "size" elements. Elements stored
 * in mailboxes are pointers. You have to define macros "_MBOX_SIZE"
 * in your lwipopts.h, or ignore this parameter in your implementation
 * and use a default size.
 */
err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
	*mbox = xQueueCreate(size, sizeof(void *)); // XXX correct? We are posting pointers so I guess it is...

	if (!*mbox) {
		SYS_STATS_INC(mbox.err);
		return ERR_MEM;
	}

	SYS_STATS_INC_USED(mbox);

	return ERR_OK;
}

/**
 * Deallocates a mailbox. If there are messages still present in the
 * mailbox when the mailbox is deallocated, it is an indication of a
 * programming error in lwIP and the developer should be notified.
 */
void sys_mbox_free(sys_mbox_t *mbox)
{
	if (uxQueueMessagesWaiting(*mbox))
	{
		// Line for breakpoint.  Should never break here!
		__asm__ __volatile__ ( "nop" );
	}

	vQueueDelete(*mbox);

	SYS_STATS_DEC(mbox.used);
}

/**
 * Posts the "msg" to the mailbox. This function have to block until
 * the "msg" is really posted.
 */
void sys_mbox_post(sys_mbox_t *mbox, void *data)
{
	xQueueSend(*mbox, &data, (portTickType)(archPOST_BLOCK_TIME_MS / portTICK_RATE_MS));
}

/**
 * Try to post the "msg" to the mailbox. Returns ERR_MEM if this one
 * is full, else, ERR_OK if the "msg" is posted.
 */
err_t sys_mbox_trypost(sys_mbox_t *mbox, void *data)
{
	return (xQueueSend(*mbox, &data, 0) == pdTRUE) ? ERR_OK : ERR_MEM;
}

/**
 * Blocks the thread until a message arrives in the mailbox, but does
 * not block the thread longer than "timeout" milliseconds (similar to
 * the sys_arch_sem_wait() function). If "timeout" is 0, the thread should
 * be blocked until a message arrives. The "msg" argument is a result
 * parameter that is set by the function (i.e., by doing "*msg =
 * ptr"). The "msg" parameter maybe NULL to indicate that the message
 * should be dropped.
 *
 * The return values are the same as for the sys_arch_sem_wait() function:
 * Number of milliseconds spent waiting or SYS_ARCH_TIMEOUT if there was a
 * timeout.
 *
 * Note that a function with a similar name, sys_mbox_fetch(), is
 * implemented by lwIP.
 */
u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
	void *dummyptr;
	portTickType nStartTime, nEndTime, nElapsed;

	nStartTime = xTaskGetTickCount();

	if (msg == NULL)
		msg = &dummyptr;

	if (timeout != 0)
	{
		if (pdTRUE == xQueueReceive(*mbox, msg, timeout))
		{
			nEndTime = xTaskGetTickCount();
			nElapsed = nEndTime - nStartTime;

			if (nElapsed == 0)
				nElapsed = 1;

			return nElapsed;
		}
		else // timed out blocking for message
		{
			*msg = NULL;
			return SYS_ARCH_TIMEOUT;
		}
	}
	else // block forever for a message.
	{
		// wait on message forever
		while (pdTRUE != xQueueReceive(*mbox, msg, 10000));

		nEndTime = xTaskGetTickCount();
		nElapsed = nEndTime - nStartTime;

		if (nElapsed == 0)
			nElapsed = 1;

		return nElapsed; // return time blocked TBD test
	}
}

/**
 * This is similar to sys_arch_mbox_fetch, however if a message is not
 * present in the mailbox, it immediately returns with the code
 * SYS_MBOX_EMPTY. On success 0 is returned.
 *
 * To allow for efficient implementations, this can be defined as a
 * function-like macro in sys_arch.h instead of a normal function. For
 * example, a naive implementation could be:
 * #define sys_arch_mbox_tryfetch(mbox,msg) \
 * 		sys_arch_mbox_fetch(mbox,msg,1)
 * although this would introduce unnecessary delays.
 */
u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
	void *dummyptr;

	if (msg == NULL)
		msg = &dummyptr;

	return (pdTRUE == xQueueReceive(*mbox, msg, 0)) ? 0 : SYS_MBOX_EMPTY;
}

// ------------------------------------ Task stuff ----------------------------------------------
/**
 * Starts a new thread named "name" with priority "prio" that will begin its
 * execution in the function "thread()". The "arg" argument will be passed as an
 * argument to the thread() function. The stack size to used for this thread is
 * the "stacksize" parameter. The id of the new thread is returned. Both the id
 * and the priority are system dependent.
 */
sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio)
{
	xTaskHandle objTask;
	int result = pdFAIL;

	result = xTaskCreate(thread, (signed portCHAR*)name, stacksize, arg, prio, &objTask);

	return (result == pdPASS) ? objTask : NULL;
}
#endif
