/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2009 Altera Corporation, San Jose, California, USA.           *
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
#include "system.h"

#ifdef TSE_MY_SYSTEM

#include "altera_avalon_tse.h"
#include "altera_avalon_tse_system_info.h"
 

alt_tse_system_info tse_mac_device[MAXNETS] = {
        /************************************************************************************/
#ifdef GOT_TSE_MAC_0
			TSE_SYSTEM_EXT_MEM_NO_SHARED_FIFO(TSE_MAC_0, 0, SGDMA_TSE_TX_0, SGDMA_TSE_RX_0, TSE_PHY_AUTO_ADDRESS, 0, TSE_DESCRIPTOR_MEMORY_0)
#endif
#ifdef GOT_TSE_MAC_1
			TSE_SYSTEM_EXT_MEM_NO_SHARED_FIFO(TSE_MAC_1, 0, SGDMA_TSE_TX_1, SGDMA_TSE_RX_1, TSE_PHY_AUTO_ADDRESS, 0, TSE_DESCRIPTOR_MEMORY_1)
#endif
#ifdef GOT_TSE_MAC_2
			TSE_SYSTEM_EXT_MEM_NO_SHARED_FIFO(TSE_MAC_2, 0, SGDMA_TSE_TX_2, SGDMA_TSE_RX_2, TSE_PHY_AUTO_ADDRESS, 0, TSE_DESCRIPTOR_MEMORY_2)
#endif
#ifdef GOT_TSE_MAC_3
			TSE_SYSTEM_EXT_MEM_NO_SHARED_FIFO(TSE_MAC_3, 0, SGDMA_TSE_TX_3, SGDMA_TSE_RX_3, TSE_PHY_AUTO_ADDRESS, 0, TSE_DESCRIPTOR_MEMORY_3)
#endif
#ifdef GOT_TSE_MAC_4
			TSE_SYSTEM_EXT_MEM_NO_SHARED_FIFO(TSE_MAC_4, 0, SGDMA_TSE_TX_4, SGDMA_TSE_RX_4, TSE_PHY_AUTO_ADDRESS, 0, TSE_DESCRIPTOR_MEMORY_4)
#endif
#ifdef GOT_TSE_MAC_5
			TSE_SYSTEM_EXT_MEM_NO_SHARED_FIFO(TSE_MAC_5, 0, SGDMA_TSE_TX_5, SGDMA_TSE_RX_5, TSE_PHY_AUTO_ADDRESS, 0, TSE_DESCRIPTOR_MEMORY_5)
#endif
#ifdef GOT_TSE_MAC_6
			TSE_SYSTEM_EXT_MEM_NO_SHARED_FIFO(TSE_MAC_6, 0, SGDMA_TSE_TX_6, SGDMA_TSE_RX_6, TSE_PHY_AUTO_ADDRESS, 0, TSE_DESCRIPTOR_MEMORY_6)
#endif
#ifdef GOT_TSE_MAC_7
			TSE_SYSTEM_EXT_MEM_NO_SHARED_FIFO(TSE_MAC_7, 0, SGDMA_TSE_TX_7, SGDMA_TSE_RX_7, TSE_PHY_AUTO_ADDRESS, 0, TSE_DESCRIPTOR_MEMORY_7)
#endif
        /************************************************************************************/		
};

#endif /* TSE_MY_SYSTEM */

