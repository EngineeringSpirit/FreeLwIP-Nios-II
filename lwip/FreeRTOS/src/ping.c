/**
 * \file ping.h
 *
 * This file provides easy access to a ping interface.
 *
 * Created on: 6 jan. 2012
 * 
 * Created by: Engineering Spirit (c) 2012 http://engineering-spirit.nl/
 */

#include "ping.h"

#include <lwip/opt.h>
#include <lwip/raw.h>
#include <lwip/icmp.h>
#include <lwip/netif.h>
#include <lwip/sys.h>
#include <lwip/timers.h>
#include <lwip/inet_chksum.h>
#include <lwip/icmp.h>
#include <lwip/sockets.h>
#include <lwip/mem.h>
#include <lwip/ip.h>

#include <sys/time.h>

#include <unistd.h>

/**
 * PING_DEBUG: Enable debugging for PING.
 */
#ifndef PING_DEBUG
# define PING_DEBUG			LWIP_DBG_OFF
#endif

/** ping receive timeout - in milliseconds */
#ifndef PING_RCV_TIMEO
# define PING_RCV_TIMEO		1000
#endif

/** ping delay - in milliseconds */
#ifndef PING_DELAY
# define PING_DELAY			1000
#endif

/** ping identifier - must fit on a u16_t */
#ifndef PING_ID
# define PING_ID				0xAFAF
#endif

#define LWIP_PING_DATA_LENGTH			10

static u16_t ping_seq_num;
static u32_t ping_time;

err_t ping_send(int s, ip_addr_t *addr, u8_t *data, u16_t size) {
	int err;
	struct icmp_echo_hdr *iecho;
	struct sockaddr_in to;
	size_t total_size = sizeof(struct icmp_echo_hdr) + size;

	LWIP_ASSERT("ping_size is too big", total_size <= 0xffff);

	iecho = (struct icmp_echo_hdr *)mem_malloc(total_size);
	if (!iecho)
		return ERR_MEM;

	// increase ping counter
	++ping_seq_num;

	// set appropriate data
	ICMPH_TYPE_SET(iecho, ICMP_ECHO);
	ICMPH_CODE_SET(iecho, 0);
	iecho->chksum = 0;
	iecho->id     = PING_ID;
	iecho->seqno  = htons(ping_seq_num); // you can't assume that htons is a function if it's a macro with 2 shifts the counter will be incremented 2 times...

	// fill additional data in the buffer
	memcpy((((char*)iecho) + sizeof(struct icmp_echo_hdr)), data, size);

	// calculate the checksum
	iecho->chksum = inet_chksum(iecho, total_size);

	// set the appropriate ethernet header
	to.sin_len = sizeof(to);
	to.sin_family = AF_INET;
	inet_addr_from_ipaddr(&to.sin_addr, addr);
	//to.sin_addr.s_addr = addr->addr;

	// send the packet
	err = sendto(s, iecho, total_size, 0, (struct sockaddr*)&to, sizeof(to));
	if (err < 0)
		LWIP_DEBUGF(PING_DEBUG, ("Couldn't send ICMP Ping packet!\n"));

	// clear the ping object
	mem_free(iecho);

	// how did it turn out?
	return ((err == (int)total_size) ? ERR_OK : ERR_VAL);
}

int ping_recv(int s)
{
	char buf[64];
	int fromlen, len;
	struct sockaddr_in from;
	struct ip_hdr *iphdr;
	struct icmp_echo_hdr *iecho;

	// remember that there is a one minute timeout on on the receive function ;)
	while ((len = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr*)&from, (socklen_t*)&fromlen)) > 0) {
		if (len >= (int)(sizeof(struct ip_hdr)+sizeof(struct icmp_echo_hdr))) {
			ip_addr_t fromaddr;

			inet_addr_to_ipaddr(&fromaddr, &from.sin_addr);

			LWIP_DEBUGF( PING_DEBUG, ("ping: recv "));
			ip_addr_debug_print(PING_DEBUG, &fromaddr);
			LWIP_DEBUGF( PING_DEBUG, (" %"U32_F" ms\n", (sys_now() - ping_time)));

			iphdr = (struct ip_hdr *)buf;
			iecho = (struct icmp_echo_hdr *)(buf + (IPH_HL(iphdr) * 4));

			if ((iecho->id == PING_ID) && (iecho->seqno == htons(ping_seq_num))) {
				return 1;
			} else {
				LWIP_DEBUGF( PING_DEBUG, ("ping: drop\n"));
			}
		}
	}

	if (len == 0) {
		LWIP_DEBUGF( PING_DEBUG, ("ping: recv - %"U32_F" ms - timeout\n", (sys_now()-ping_time)));
	}

	/* do some ping result processing */
	return -EXIT_FAILURE;
}

int lwip_ping_target(u32_t addr, u8_t packets, int get_response, u8_t size)
{
	u8_t buf[size];
	int idx;

	// fill dummy data
	for (idx = 0; idx < size; idx++)
		buf[idx] = idx;

	// send this
	return lwip_ping_target_data(addr, packets, get_response, buf, size);
}

int lwip_ping_target_data(u32_t addr, u8_t packets, int get_response, u8_t *data, u16_t size)
{
	int s;
	ip_addr_t ping_target;
	int pongcnt = 0;
	int timeout = PING_RCV_TIMEO;

	// open our raw socket
	if ((s = socket(AF_INET, SOCK_RAW, IP_PROTO_ICMP)) < 0)
		return -EXIT_SUCCESS;

	if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
		LWIP_DEBUGF(PING_DEBUG, ("Couldn't set ping receive socket timeout\n"));

	while (packets--) {
		ping_target.addr = addr; // addr should already be converted with htonl just like normal socket operations

		if (ping_send(s, &ping_target, data, size) == ERR_OK) {
			LWIP_DEBUGF( PING_DEBUG, ("ping: send "));

			ip_addr_debug_print(PING_DEBUG, &ping_target);

			LWIP_DEBUGF( PING_DEBUG, ("\n"));

			ping_time = sys_now();

			// get the pong if we we ever receive one
			if (get_response)
				pongcnt += ping_recv(s);
			else
				pongcnt++;
		} else {
			LWIP_DEBUGF( PING_DEBUG, ("ping: send "));

			ip_addr_debug_print(PING_DEBUG, &ping_target);

			LWIP_DEBUGF( PING_DEBUG, (" - error\n"));

			pongcnt--;
		}

		// if we need to send more packets, w'll wait a bit
		if (packets)
			sys_msleep(PING_DELAY);
	}

	close(s);

	return pongcnt;
}
