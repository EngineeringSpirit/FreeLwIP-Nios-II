#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

// include LwIP
#include <lwip_main.h>
#include <lwip/dhcp.h>
#include <lwip/sockets.h>
#include <lwip/stats.h>
#include <netif/etharp.h>
#include <ping.h>
#include <math.h>

#include "BasicWEB.h"

// include low-level network support
#include <triple_speed_ethernet_regs.h>
#include <altera_tse_ethernetif.h>
#include <altera_avalon_tse.h>

#include "BasicWEB.h"

#define mssleep(x)						usleep((x) * 1000)

#define MDIO_IFACE						mdio1

extern void prvMySocketTest(void *params);

#ifdef __cplusplus
};
#endif

#define ETH_STATUS_DISCONNECTED			0x20

// callback function for when the DHCP subsystem acquires an IP address.
static void StatusCallback(struct netif* netif)
{
	// get IP and stuff
	printf("[ethernet] Acquired IP address via DHCP client for interface: %s\n", netif->name);

	printf("[ethernet] IP address : %s\n", print_ipad(netif->ip_addr.addr));
	printf("[ethernet] Subnet     : %s\n", print_ipad(netif->netmask.addr));
	printf("[ethernet] Gateway    : %s\n", print_ipad(netif->gw.addr));
}

static void LinkCallback(__unused struct netif* netif)
{
	// link change callback
	// TODO release semaphore
	// TODO switch context
}

int InitNetwork(void)
{
	// register new DHCP "IP attained" callback function.
	// if DHCP is acquired, ws_ipset will be called instead of dhc_main_ipset().
	lwip_set_status_callback(StatusCallback);
	lwip_set_link_callback(LinkCallback);

	// Initialize LwIP TCP/IP stack.
	// This function is blocking till the the interface is up.
	lwip_initialize();

	return EXIT_SUCCESS;
}

static int WaitOnPHY(void)
{
	int phyadd;
	int phyid;
	int phyid2 = 0;

	np_tse_mac* pmac;
	bool bInitialized = false;

	while (!bInitialized) {
		printf("[ethernet] PHY INFO: Interface: %d Waiting for PHY\n", 0);

		// initialize the structure necessary for "pmac" to function.
		pmac = (np_tse_mac*) TSE_MAC_BASE;

		// we are using a Lantiq PHY
		for (phyadd = 0x00; phyadd < 0xff; phyadd++) {
			IOWR(&pmac->MDIO_ADDR0, 0, phyadd);

			phyid = IORD(&pmac->MDIO_IFACE.PHY_ID1, 0);
			phyid2 = IORD(&pmac->MDIO_IFACE.PHY_ID2, 0);

			if (phyid != phyid2) {
				printf("[ethernet] PHY INFO: [PHY ID] 0x%x %x %x\n", phyadd, phyid, phyid2);
				phyadd = 0xff;
			}
		}

		if ((phyadd == 0xff) && (phyid == phyid2)) {
			printf("[ethernet] PHY INFO: No PHY found... restart detect\n");
			bInitialized = true;
			mssleep(1000);
		}
		else
			bInitialized = true;
	}

	// issue a PHY reset.
	IOWR(&pmac->MDIO_IFACE.CONTROL, 0, PCS_CTL_an_enable | PCS_CTL_sw_reset);
	if (((IORD(&pmac->MDIO_IFACE.CONTROL, 0) & PCS_CTL_rx_slpbk) != 0) || ((IORD(&pmac->MDIO_IFACE.STATUS, 0) & PCS_ST_an_done) == 0)) {
		IOWR(&pmac->MDIO_IFACE.CONTROL, 0, PCS_CTL_an_enable | PCS_CTL_sw_reset);
		printf("[ethernet] PHY INFO: Issuing PHY Reset\n");
	}

	// holding pattern until autonegotiation completes.
	if ((IORD(&pmac->MDIO_IFACE.STATUS, 0) & PCS_ST_an_done) == 0) {
		printf("[ethernet] PHY INFO: Waiting on PHY link...\n");

		while ((IORD(&pmac->MDIO_IFACE.STATUS, 0) & PCS_ST_an_done) == 0)
			mssleep(10);

		printf("[ethernet] PHY INFO: PHY link detected, allowing network to start.\n");
		
		mssleep(1000);
	}

	mssleep(10);

	return 0;
}

void xEthernetRun(__unused void* param)
{
	// initialize PHY
	WaitOnPHY();

	if (InitNetwork() != EXIT_SUCCESS) {
		// the network initialization has failed.
		printf("[ethernet] Network initialize failed!\n");
		return;
	}

	// populate the local pmac structure upon successful network initialization.
	np_tse_mac* pmac = (np_tse_mac*)TSE_MAC_BASE;

	// set a counter to allow for a brief network disconnect.
	int nDisconnectCnt = 0;

	// start the demo network tasks
	sys_thread_new("TCP Echo Server", prvMySocketTest, NULL, KB(4), 3);
	sys_thread_new("HTTP Server", vBasicWEBServer, NULL, KB(8), 3);
	
	// keep checking our network status, are we connected or disconnected?
	while (1) {
		// TODO wait for semaphore is network status changes

		// sleep for 1 second
		mssleep(1000);

#if 0
		// TODO not implemented yet... 

		status = IORD(&pmac->MDIO_IFACE.STATUS, 0);
		if (!(status & ETH_STATUS_DISCONNECTED)) {
			nDisconnectCnt++;
			if (nDisconnectCnt == NET_MAX_CABLE_DOWN) {
				//printf("\n[ethernet] PHY ERROR: Ethernet Cable Disconnected!\n");

				//if (PauseChilds() != EXIT_SUCCESS)
				//printf("[ethernet] INFO:  Error pausing network tasks!\n");
			}
		} else {
			// if the cable was disconnected, reset the PHY and initiate the DHCP discovery process
			if (nDisconnectCnt > 0) {
				printf("[ethernet] PHY INFO:  Cable Reconnected!\n");

				// reset the PHY
				//WaitOnPHY();

				// if DHCP force re-acquisition of existing IP Address.
				// XXX reinitialize the LwIP TCP/IP stack... don't know whether this is a good idea ;)
				//lwip_initialize();
				//if (configs[0].dhcp_client)
				//dhc_set_state(0, DHCS_INITREBOOT);
			}

			// reset the disconnect counter.
			nDisconnectCnt = 0;
		}

#endif
	}

	return 0;
}

// callback wrapper for lwip to get the interface configurations
int get_mac_addr(int iface, struct netif* ethif, unsigned char mac_addr[6])
{	
	mac_addr[0] = 0x00;
	mac_addr[1] = 0x07;
	mac_addr[2] = 0xED;
	mac_addr[3] = 0xFF;
	mac_addr[4] = 0x68;
	mac_addr[5] = 0xB0 + iface;
	
	// only show info if net is not NULL
	if (ethif)
		printf("[ethernet] Using Ethernet MAC address %02x:%02x:%02x:%02x:%02x:%02x for interface: %d\n",
				mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5], iface);

	return EXIT_SUCCESS;
}

// callback wrapper for lwip to get the IP configurations
int get_ip_addr(int iface, ip_addr_t* ipaddr, ip_addr_t* netmask, ip_addr_t* gw, int* use_dhcp)
{
	// set configuration
	IP4_ADDR(ipaddr, 192, 168, 0, 218);
	IP4_ADDR(netmask, 255, 255, 255, 0);
	IP4_ADDR(gw, 192, 168, 0, 1);
	*use_dhcp = 1;

	if (*use_dhcp == 0)
		printf("[ethernet] Static IP Address for interface %d %s\n", iface, print_ipad(ipaddr->addr));
	else
		printf("[ethernet] Starting get IP via DHCP for interface %d\n", iface);

	return EXIT_SUCCESS;
}

int get_hostname(int iface, const char **hostname)
{
	*hostname = "LwIP";

	return ERR_OK;
}

int get_iface_name(int iface, char name[ETH_IFACE_NAME_LENGTH])
{
	name[0] = 'e';
	name[1] = (iface + 0x30);

	return ERR_OK;
}

int is_interface_active(int iface)
{
	return 1;
}
