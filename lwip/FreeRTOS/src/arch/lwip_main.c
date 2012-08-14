/**
 * \file lwip_main.c
 *
 * \brief This is the main interface API to initialize the LwIP stack.
 *
 * This is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License (version 2) as published by the
 * Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
 * >>>NOTE<<< The modification to the GPL is included to allow you to
 * distribute a combined work that includes FreeRTOS without being obliged to
 * provide the source code for proprietary components outside of the FreeRTOS
 * kernel.  FreeRTOS is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details. You should have received a copy of the GNU General Public
 * License and the FreeRTOS license exception along with FreeRTOS; if not it
 * can be viewed here: http://www.freertos.org/a00114.html and also obtained
 * by writing to Richard Barry, contact details for whom are available on the
 * FreeRTOS WEB site.
 *
 * Created by Engineering Spirit (c) 2012 http://engineering-spirit.nl/
 */

// include libC headers
#include <unistd.h>

// include Altera headers
#include <system.h>
#include <alt_types.h>

#include <sys/alt_alarm.h>

// include LwIP headers
#include <lwip/opt.h>
#include <lwip/init.h>
#include <lwip/netif.h>
#include <lwip/dhcp.h>
#include <lwip/tcp.h>
#include <lwip/stats.h>
#include <lwip/ip_frag.h>
#include <lwip/ip_addr.h>
#include <lwip/tcp_impl.h>
#include <lwip/tcpip.h>
#include <lwip/sockets.h>

#include <netif/etharp.h>

// include TSE driver headers
#include "altera_tse_ethernetif.h"

// include FreeRTOS headers
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <timers.h>

// don't forget our header
#include "lwip_main.h"

// some defines we need
#define TCP_TIMER_MS				250
#define USE_DHCP					1

#define mssleep(x)					usleep((x) * 1000)

#define MY_TIMER					1

// timer variables
#if MY_TIMER
static alt_alarm lwip_timer;
static sys_sem_t lwip_timer_semaphore;
#endif

static int dhcp_running = 0;

static netif_status_callback_fn status_callback = NULL;
static netif_status_callback_fn link_callback = NULL;

// our interface array, in case we want to support more then one PHY
static struct netif eth_tse[PHY_COUNT];

// some private functions
#if MY_TIMER
static void lwip_process_timers(void *args);
#endif
static void lwip_handle_interfaces(void *args);
static void lwip_handle_ethernet_input(void *ethif);
static void lwip_status_callback(struct netif *netif);

#if MY_TIMER
// Callback function which releases a semaphore for the timer task.
// Note: This is FreeRTOS specific since the LwIP port interface doesn't support from ISR functions
//       change to suit your needs
static __noinline alt_u32 lwip_timer_callback(__unused void* context)
{
	portBASE_TYPE switch_context = 0;

	// release the semaphore and check if a task with a higher priority then the current one is waiting for it
    xSemaphoreGiveFromISR(lwip_timer_semaphore, &switch_context);

    // force context switch if needed
    portEND_SWITCHING_ISR(switch_context);

    // return the amount of ticks we need to wait
    return TCP_TIMER_MS;
}
#endif

// Function which initializes the LwIP TCP/IP stack
void lwip_initialize(void)
{
	int idx;

#if MY_TIMER
	// Setup the timer task needed for LwIP book keeping
	// For that we need a semaphore
	if (sys_sem_new(&lwip_timer_semaphore, 0) != ERR_OK) {
		printf("[LwIP] Failed to create the timer semaphore, aborting initialize.\n");
		return;
	}

	// A task in a high priority which waits for the timer semaphore to be released
	// this way it will be scheduled ASAP when the timer callback returns
	if (sys_thread_new("LwIP timer", lwip_process_timers, NULL, KB(2), TCPIP_THREAD_PRIO) == NULL) {
		printf("[LwIP] Couldn't create timer timer task, aborting initialize.\n");
		sys_sem_free(&lwip_timer_semaphore);
		return;
	}
#endif

	// Initialize the TCP/IP stack and give our configuration function as callback
	tcpip_init(lwip_handle_interfaces, NULL);

	// Wait for the network to get up
	for (idx = 0; idx < PHY_COUNT; ++idx)
		while (!netif_is_up(&eth_tse[idx]))
		mssleep(10);
}

#if MY_TIMER
// This task will process the LwIP timers and will be scheduled every 250ms
static void lwip_process_timers(__unused void *params)
{
#define LWIP_TIMER_TICK_COUNTER(tcp_timer)			lwip##tcp_timer##MSTimer
#define LWIP_TIMER_VAR								LWIP_TIMER_TICK_COUNTER(TCP_TIMER_MS)

	alt_u32 LWIP_TIMER_VAR = 0;

	// initialize the timer, we'll use the altera timer since this one is checked on interrupt instead
	// of the timer task from FreeRTOS, this makes the timer more accurate
	if (alt_alarm_start(&lwip_timer, TCP_TIMER_MS, lwip_timer_callback, NULL) < 0)
	{
		// we can't create our TCP timer, TCP stack can't start
		printf("Couldn't create LwIP TCP timer, LwIP can't start!\n");

		// free the semaphore
		sys_sem_free(&lwip_timer_semaphore);

		// exit the timer task
		return;
	}

	// now we can wait for the semaphore to be released by the timer
	while (1)
	{
		// wait for the semaphore to be cleared in the alarm callback
		if (sys_arch_sem_wait(&lwip_timer_semaphore, (TCP_TIMER_MS + 50)))
		{
			LWIP_TIMER_VAR += TCP_TIMER_MS;
			if( (LWIP_TIMER_VAR % TCP_TMR_INTERVAL) == 0 ) { tcp_tmr(); }
			if( (LWIP_TIMER_VAR % ARP_TMR_INTERVAL) == 0 ) { etharp_tmr(); }
#if IP_REASSEMBLY
			if( (LWIP_TIMER_VAR % IP_TMR_INTERVAL) == 0 ) { ip_reass_tmr(); }
#endif
#if LWIP_AUTOIP
			if( (LWIP_TIMER_VAR % AUTOIP_TMR_INTERVAL ) == 0 ) { autoip_tmr(); }
#endif
#if LWIP_IGMP
			if( (LWIP_TIMER_VAR % IGMP_TMR_INTERVAL ) == 0 ) { igmp_tmr(); }
#endif
#if LWIP_DHCP
			if( (LWIP_TIMER_VAR % DHCP_FINE_TIMER_MSECS ) == 0 ) { dhcp_fine_tmr(); }
			if( (LWIP_TIMER_VAR % (DHCP_COARSE_TIMER_SECS*1000) ) == 0 ) { dhcp_coarse_tmr(); }
#endif
#if LWIP_DNS
			if( (LWIP_TIMER_VAR % DNS_TMR_INTERVAL) == 0 ) { dns_tmr(); }
#endif

		}
		else
		{
			printf("[LwIP] Timer task semaphore not cleared!!!!\n\t\tTimer is not running?!\n");

			// play nice and sleep a bit
			mssleep(10);
		}
	}
}
#endif

void lwip_handle_interfaces(__unused void *params)
{
	int idx;
	int active_macs = 0;
	char tmpbuf[OS_MAX_TASK_NAME_LEN];

	ip_addr_t ip = {0}, subnet = {0}, gateway = {0};
	int dhcp;

	for (idx = 0; idx < PHY_COUNT; ++idx)
	{
		struct netif *eth = &eth_tse[idx];

	// Load platform specific MAC address into netif
		if (get_mac_addr(idx, eth, eth->hwaddr) != EXIT_SUCCESS)
		printf("[LwIP] Failed to get MAC address\n");

	// Get the requested IP configuration for the given interface
		if (get_ip_addr(idx, &ip.addr, &subnet.addr, &gateway.addr, &dhcp) != EXIT_SUCCESS)
		printf("[LwIP] Failed to get IP config\n");

	//  Initialize lwIP, Altera TSE and the ethernetif
	#if NO_SYS
		if (netif_add(eth, &ip, &subnet, &gateway, eth->state, ethernetif_init, ethernet_input) == NULL)
	#else
		if (netif_add(eth, &ip, &subnet, &gateway, eth->state, ethernetif_init, tcpip_input) == NULL)
	#endif
	{
			printf("[eth%d] Fatal error initializing...\n", idx);
			for(;;) ;
	}

		// check whether this interface should be used
		if (!is_interface_active(idx))
			continue;

		// update the active phy count
		++active_macs;

		if (active_macs == 1)
			netif_set_default(eth);

	// Set status and link callback (link is not working?)
		netif_set_status_callback(eth, lwip_status_callback);

	if (link_callback)
			netif_set_link_callback(eth, link_callback);

	// Initialize Altera TSE in a loop if waiting for a link
	printf("Waiting for link...");
		while (((struct ethernetif *) eth->state)->link_alive != 1) {
		mssleep(100);
		putchar('.');
			tse_mac_init(idx, eth->state);
	}
	printf("OK\n");

	// create input output task and start DHCP or static w/e
		snprintf(tmpbuf, OS_MAX_TASK_NAME_LEN, "LwIP %*sih", 2, eth->name);
	tmpbuf[(OS_MAX_TASK_NAME_LEN - 1)] = 0;

	// create input task, this must be started before we can do any DHCP request
		if (sys_thread_new(tmpbuf, lwip_handle_ethernet_input, eth, KB(32), TCPIP_THREAD_PRIO) == NULL)
		printf("LwIP Couldn't create input / output task for ethernet\n");

		// wait previous DHCP to finish
	if (dhcp) {
			printf("[LwIP] Start DHCP request...\n");

			dhcp_start(eth);

		// wait till we got our address
		printf("[LwIP] Waiting for DHCP IP address...\n");
	} else {
		// the static IP has already been set, just bring up the interface
			netif_set_up(eth);
	}
	}
}

netif_status_callback_fn lwip_set_status_callback(netif_status_callback_fn callback)
{
	netif_status_callback_fn old = status_callback;

	status_callback = callback;

	return old;
}

netif_status_callback_fn lwip_set_link_callback(netif_status_callback_fn callback)
{
	netif_status_callback_fn old = link_callback;

	link_callback = callback;

	return old;
}

// Input task per ethernet device
static void lwip_handle_ethernet_input(void *pvParameters)
{
	sys_sem_t rcvsem = NULL;

#if LWIP_RECEIVE_SEMAPHORE
	rcvsem = ((struct ethernetif*)((struct netif*)pvParameters)->state)->tse_info->rx_semaphore;
#endif

	// if we have a semaphore we'll wait for the semaphore
	// else we'll poll the function once every 10ms
	if (rcvsem) {
		while (1) {
			// wait for the semaphore to be released by the SGDMA IRQ
			// if we timeout also call ethernetif_input although most likely it would be useless
			sys_arch_sem_wait(&rcvsem, 5);

			// Use semaphore or the timeout to call ethernet_input
			// this to avoid unnecessary load and faster responses ;)
			ethernetif_input(pvParameters);
		}
	} else {
		while (1) {
			ethernetif_input(pvParameters);

			mssleep(5);
		}
	}
}

const char* print_ipad(alt_u32 ip, char* buf)
{
	alt_u8 bip[4];
	int idx = 0;

	for (; idx < sizeof(bip); idx++) {
		bip[idx] = ip & 0xFF;
		ip >>= 8;
	}

	snprintf(buf, 17, "%d.%d.%d.%d", bip[0], bip[1], bip[2], bip[3]);

	return buf;
}

static void lwip_status_callback(struct netif *netif)
{
	if (status_callback)
		status_callback(netif);

	dhcp_running = 0;
}
