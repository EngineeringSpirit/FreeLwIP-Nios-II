/* OPTIONAL: Pools to replace heap allocation
 * Optional: Pools can be used instead of the heap for mem_malloc. If
 * so, these should be defined here, in increasing order according to
 * the pool element size.
 *
 * LWIP_MALLOC_MEMPOOL(number_elements, element_size)
 */
#if MEM_USE_POOLS
LWIP_MALLOC_MEMPOOL_START
LWIP_MALLOC_MEMPOOL(20, 256)
LWIP_MALLOC_MEMPOOL(10, 512)
LWIP_MALLOC_MEMPOOL(10, 1536)
LWIP_MALLOC_MEMPOOL_END
#endif /* MEM_USE_POOLS */

/* Optional: Your custom pools can go here if you would like to use
 * lwIP's memory pools for anything else.
 */
LWIP_MEMPOOL(SYS_MBOX, 25, 4, "SYS_MBOX")
LWIP_MEMPOOL(SYS_SEM, 25, 4, "SYS_SEM")
LWIP_MEMPOOL(SYS_MUTEX, 25, 4, "SYS_MUTEX")
LWIP_MEMPOOL(SYS_THREAD, 25, 4, "SYS_THREAD")
