/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2006 Altera Corporation, San Jose, California, USA.           *
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
* Altera does not recommend, suggest or require that this reference design    *
* file be used in conjunction or combination with any other product.          *
******************************************************************************/

/******************************************************************************
*                                                                             *
* THIS IS A LIBRARY READ-ONLY SOURCE FILE. DO NOT EDIT IT DIRECTLY.           *
*                                                                             *
* Overriding HAL Functions                                                    *
*                                                                             *
******************************************************************************/

#include <fcntl.h>
#include <stdarg.h>
#include "system.h"
#include "sys/alt_sys_wrappers.h"
#include <lwip/sockets.h>

/*
 * fcntl() is called by an application to release a file descriptor. This 
 * implementation duplicates the code of the HAL alt_fcntl() function 
 * (for files and device drivers) or calls the InterNiche bsd_ioctl for 
 * sockets.
 */
 
int fcntl (int file, int cmd, ...)
{
	long     flags;
	va_list  argp;

	if (file < ALT_MAX_FD)
	{
		va_start(argp, cmd);
		flags = va_arg(argp, long);
		va_end(argp);
		return ALT_FCNTL(file, cmd, flags);
	}
#if LWIP_SOCKET
	else
	{
		va_start(argp, cmd);
		flags = va_arg(argp, long);
		va_end(argp);
		return lwip_fcntl(file, cmd, flags);
	}
#else
	else
		return -1;
#endif
}
