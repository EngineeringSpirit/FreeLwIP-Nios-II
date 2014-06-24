/**
 * @file
 * Ethernet Interface for Altera TSE and the NIOS II processor
 * 
 * Contributed by:  Bill Auerbach
 *                  lwip@softools.com
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#include <lwip/opt.h>

#include <lwip/def.h>
#include <lwip/mem.h>
#include <lwip/pbuf.h>
#include <lwip/sys.h>
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include <netif/etharp.h>
#include <netif/ppp_oe.h>
#include "altera_tse_ethernetif.h"

#include <sys/alt_irq.h>
#include <sys/alt_cache.h>

#include <lwip_main.h>

/* Define those to better describe your network interface. */
#define IFNAME0 'e'
#define IFNAME1 'n'

extern int lwip_wait_for_an(int idx, struct ethernetif *ethernetif);

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void
low_level_init(struct netif *netif)
{
	struct ethernetif *ethernetif = netif->state;

	/* set MAC hardware address length */
	netif->hwaddr_len = ETHARP_HWADDR_LEN;

	/* maximum transfer unit */
	netif->mtu = IP_FRAG_MAX_MTU;

	/* device capabilities */
	/* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
	netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

	//tse_mac_init(netif->num, ethernetif);
	lwip_wait_for_an(netif->num, ethernetif);

	//usleep(250 * 1000);
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf *
low_level_input(struct netif *netif)
{
	struct ethernetif *ethernetif = netif->state;
	struct pbuf *p, *nextPkt;
	alt_irq_context context;

	if(ethernetif->lwipRxCount <= 0)
		return NULL;

	//  Dump current packet if there is no memory for the next packet.
	//  The lwipRxPbuf must contain pointers to pbufs at all times.
	nextPkt = pbuf_alloc(PBUF_RAW,  PBUF_POOL_BUFSIZE, PBUF_POOL);
	if(nextPkt == NULL)
	{
		LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_LEVEL_WARNING, ("no pbuf available\n"));

		LINK_STATS_INC(link.memerr);
		LINK_STATS_INC(link.drop);
		return NULL;
	}

	nextPkt = (void *) alt_remap_uncached(nextPkt,sizeof *nextPkt);
	nextPkt->payload = (void *) alt_remap_uncached(nextPkt->payload, PBUF_POOL_BUFSIZE);

	context = alt_irq_disable_all();

	p = ethernetif->lwipRxPbuf[ethernetif->lwipRxIndex];
	ethernetif->lwipRxPbuf[ethernetif->lwipRxIndex] = nextPkt;
	if(++ethernetif->lwipRxIndex >= LWIP_RX_ETH_BUFFER)
		ethernetif->lwipRxIndex = 0;

	--ethernetif->lwipRxCount;

	alt_irq_enable_all(context);

	LWIP_ASSERT("low_level_input: pbuf in rx buffer is NULL", p != NULL );
	LWIP_ASSERT("low_level_input: pbuf->len in rx buffer is 0", p->len != 0 );

	if( p->len == 0 )
		p = NULL;

	return p;
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
int
ethernetif_input(struct netif *netif)
{
	struct ethernetif *ethernetif;
	struct eth_hdr *ethhdr;
	struct pbuf *p;

	ethernetif = netif->state;

	/* move received packet into a new pbuf */
	p = low_level_input(netif);

	/* no packet could be read, silently ignore this */
	if (p == NULL) return -1;

	/* points to packet payload, which starts with an Ethernet header */
	ethhdr = p->payload;

	LINK_STATS_INC(link.recv);

#if NO_SYS
	switch (htons(ethhdr->type)) {
	/* IP or ARP packet? */
	case ETHTYPE_IP:
	case ETHTYPE_ARP:
#if PPPOE_SUPPORT
		/* PPPoE packet? */
	case ETHTYPE_PPPOEDISC:
	case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
		/* full packet send to tcpip_thread to process */
		if (netif->input(p, netif) != ERR_OK) {
			LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
			pbuf_free(p);
			p = NULL;
		}
		break;

	default:
		pbuf_free(p);
		p = NULL;
		break;
	}
#else
	switch (htons(ethhdr->type)) {
	/* IP packet? */
	case ETHTYPE_IP:
	case ETHTYPE_ARP:
#if PPPOE_SUPPORT
	/* PPPoE packet? */
	case ETHTYPE_PPPOEDISC:
	case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
		netif->input(p, netif);
		break;

	default:
		pbuf_free( p );
		p = NULL;
		break;
	}
#endif

	// return the waiting packet count
	return ethernetif->lwipRxCount;
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t
ethernetif_init(struct netif *netif)
{
	int idx;
	struct pbuf *p;
	struct ethernetif *ethernetif;
	err_t tse_mac_raw_send(struct netif *netif, struct pbuf *p);
	char *tmpptr;

	LWIP_ASSERT("netif != NULL", (netif != NULL));

	ethernetif = mem_malloc(sizeof(struct ethernetif));
	if (ethernetif == NULL) {
		LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_LEVEL_WARNING, ("ethernetif_init: out of memory\n"));
		return ERR_MEM;
	}

#if LWIP_NETIF_HOSTNAME
	/* Initialize interface hostname */
	if (get_hostname(netif->num, (const char**)&tmpptr) == ERR_OK)
		netif->hostname = tmpptr;
	else
		netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

	/*
	 * Initialize the snmp variables and counters inside the struct netif.
	 * The last argument should be replaced with your link speed, in units
	 * of bits per second.
	 */
	NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);

	// pointer abuse, don't worry ;)
	netif->state = ethernetif;
	if (get_iface_name(netif->num, (char*)&tmpptr) == ERR_OK) {
		netif->name[0] = ((char*)&tmpptr)[0];
		netif->name[1] = ((char*)&tmpptr)[1];
	} else {
		netif->name[0] = 'e';
		netif->name[1] = 't';
	}

	/* We directly use etharp_output() here to save a function call.
	 * You can instead declare your own function an call etharp_output()
	 * from it if you have to do some checks before sending (e.g. if link
	 * is available...) */
	netif->output = etharp_output;
	netif->linkoutput = tse_mac_raw_send;

	ethernetif->ethaddr = (struct eth_addr *)&(netif->hwaddr[0]);

	/* initialize the rx buffer for access by the TSE ISR */
	for(idx = 0; idx < LWIP_RX_ETH_BUFFER; ++idx)
	{
		p = pbuf_alloc(PBUF_RAW, PBUF_POOL_BUFSIZE, PBUF_POOL);
		if (p == NULL)
			return ERR_MEM;
		p = (void *) alt_remap_uncached(p,sizeof *p);
		p->payload = (void *) alt_remap_uncached(p->payload,PBUF_POOL_BUFSIZE);
		ethernetif->lwipRxPbuf[idx] = p;
	}
	ethernetif->lwipRxCount = 0;
	ethernetif->lwipRxIndex = 0;
	ethernetif->lwipRxIndexIsr = 0;

	/* initialize the low level hardware */
	low_level_init(netif);

	return ERR_OK;
}
