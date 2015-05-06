/*
 * Copyright (c) 2011-2012, Engineering Spirit NL
 * All rights reserved. 
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met: 
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 * 3. Neither the name of the Institute nor the names of its contributors 
 *    may be used to endorse or promote products derived from this software 
 *    without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE 
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
 * SUCH DAMAGE. 
 *
 * This file is part of the LwIP TCP/IP stack port for NIOS II with FreeRTOS
 * 
 * FreeRTOS support included by Engineering Spirit (c) 2012 http://engineering-spirit.nl/
 */
#ifndef __CC_H__
#define __CC_H__

// CLib includes
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// Altera HAL includes
#include <system.h>
#include <alt_types.h>

//#define LWIP_NOASSERT					0
#define LWIP_DEBUG						1

// Set altera typedefs for LwIP (in-case someone chooses a word size different from 32bits)
typedef alt_u8							u8_t;
typedef alt_8							s8_t;
typedef alt_u16							u16_t;
typedef alt_16							s16_t;
typedef alt_u32							u32_t;
typedef alt_32							s32_t;

typedef u32_t							mem_ptr_t;
typedef u32_t							ipaddr_t;

// Set printf formats
#define U16_F							"hu"
#define X16_F							"hX"
#define U32_F							"lu"
#define X32_F							"lX"
#define S16_F							"hd"
#define S32_F							"ld"

// NIOS II is configured as Little Endian most of the time however we also want to support a Big Endian CPU
#if NIOS2_BIG_ENDIAN == 1		// we don't need to swap
# define BYTE_ORDER BIG_ENDIAN
# define htonl(l)						(l)
# define htons(s)						(s)
# define ntohl(l)						(l)
# define ntohs(s)						(s)
#else							// we need to swap
# define BYTE_ORDER LITTLE_ENDIAN
# define htons(s)						((((s) & 0xFF00) >> 8) | \
											(((s) & 0x00FF) << 8))

# define htonl(l)						(((((l) >> 24) & 0x000000FF)) | \
											((((l) >> 8) & 0x0000FF00)) | \
											(((l) & 0x0000FF00) << 8) | \
											(((l) & 0x000000FF) << 24))

# define ntohs(s)						(htons(s))
# define ntohl(l)						(htonl(l))
#endif

// Avoid naming collision for our network byte swap functions
#define LWIP_PREFIX_BYTEORDER_FUNCS		1
#define LWIP_PLATFORM_BYTESWAP			1

// Now define the macro's so our byte swap functions will be used
#define LWIP_PLATFORM_HTONL(l)			(htonl(l))
#define LWIP_PLATFORM_HTONS(s)			(htons(s))
#define LWIP_PLATFORM_NTOHL(l)			(ntohl(l))
#define LWIP_PLATFORM_NTOHS(s)			(ntohs(s))

// GCC packing is straight forward
#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_STRUCT				__attribute__((packed))
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(x)			x

// Some GCC attributes for better readability
#if __GNUC__ >= 3
# ifndef __unused
#  define __unused					__attribute__((unused))
# endif

# ifndef __noinline
#  define __noinline				__attribute__((noinline))
# endif
#else
# ifndef __unused
#  define __unused					/* not supported */
# endif

# ifndef __noinline
#  define __noinline				/* not supported */
# endif
#endif

// We use the default printf functionality from the BSP package
# define LWIP_PLATFORM_DIAG(lvl, x)		do { \
	if ((lvl) >= LWIP_DBG_FORCE_LEVEL) { \
		printf(((lvl) == LWIP_DBG_LEVEL_WARNING) ? "!! WARNING !! " : ((lvl) == LWIP_DBG_LEVEL_SERIOUS) ? "!! SERIOUS !! " : "!! SEVERE !! "); \
	} \
	printf x; \
} while(0)

// TODO And we call exit (which calls ALT_EXIT on it's turn for nice process termination)
# define LWIP_PLATFORM_ASSERT(x)		do { printf("[LwIP] Assertion \"%s\" failed at line %d in %s\n", x, __LINE__, __FILE__); while (1); /* exit(1); */ } while(0)

// Redefine memory align functionality
#define LWIP_MEM_ALIGN(addr)			((void *)((((mem_ptr_t)(addr) + MEM_ALIGNMENT - 1) & ~(mem_ptr_t)(MEM_ALIGNMENT-1))))

// Implement add a random number generator for IGMP support
#define LWIP_RAND()						rand()

// Some default math stuff
#ifndef KB
# define KB(x)							((x) * 1024)
#endif
#ifndef MB
# define MB(x)							(KB(x) * 1024)
#endif
#ifndef ABS
# define ABS(x)							((x) >= 0 ? (x) : -(x))
#endif

#endif /* __CC_H__ */
