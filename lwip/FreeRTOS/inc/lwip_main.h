/**
 * \file lwip_main.h
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

#ifndef __LWIP_MAIN_H_
#define __LWIP_MAIN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <arch/cc.h>
#include <lwip/netif.h>
#include <altera_tse_ethernetif.h>

#define ETH_IFACE_NAME_LENGTH		2

#define ETH_INTERFACE_UP			1
#define ETH_INTERFACE_DOWN			0

// -----------------------------------------------------------------------------------------
// ------------------- functions which must be defined in the application ------------------
// -----------------------------------------------------------------------------------------

/**
 * \brief Callback function which checks if the current interface is active and should be enabled by the stack
 *
 * \param [in] iface the interface number
 *
 * \return wether the interface is active or not
 */
extern int is_interface_active(int iface);

/**
 * \brief Callback function which should be implemented by the application.
 *
 * \param [in] iface the interface number
 * \param [in] ethif pointer to the netif structure which is used by the driver
 * \param [out] mac must be set with the MAC address the interface should use
 *
 * \return the function must return 0 on success
 */
extern int get_mac_addr(int iface, struct netif* ethif, alt_u8 mac[NETIF_MAX_HWADDR_LEN]);

/**
 * \brief Callback function which should be implemented by the application.
 *
 * \note All address must be set in network byte order
 *
 * \param [in] iface the interface number
 * \param [out] ipaddr the static IP address wich should be set on the interface
 * \param [out] netmask the static subnetmask which should be set on the interface
 * \param [out] gw the static gateway address wich should be set on the interface
 * \param [out] use_dhcp set to 1 if LwIP should get his IP address via DHCP (the other arguments will be ignored)
 *
 * \return the function must return 0 on succes
 */
extern int get_ip_addr(int iface, ip_addr_t* ipaddr, ip_addr_t* netmask, ip_addr_t* gw, int* use_dhcp);

/**
 * \brief Callback function which should be implemented by the application.
 *
 * \param [in] iface the interface number
 * \param [out] hostname must be set with a pointer to static allocated buffer
 *
 * \return the function must return 0 on succes
 */
extern int get_hostname(int iface, const char **hostname);

/**
 * \brief Callback function which should be implemented by the application.
 *
 * \param [in] iface the interface number
 * \param [out] name must be set with 2 charachters to identify the interface by name (eg. e0)
 *
 * \return the function must return 0 on success
 */
extern int get_iface_name(int iface, char name[ETH_IFACE_NAME_LENGTH]);

// -----------------------------------------------------------------------------------------
// --------------------- functions which can be used by the application --------------------
// -----------------------------------------------------------------------------------------

/**
 * \brief Get the number of MAC's available in the NIOS build
 *
 * \returns the number of MAC's in the NIOS build
 */
extern int get_mac_count(void);

/**
 * \brief Get the base address of the MAC
 *
 * \param [in] idx the index of the MAC
 *
 * \returns the base address of the MAC
 */
extern volatile np_tse_mac* get_mac_base(int idx);

/**
 * \brief Get the netif struct used by LwIP
 *
 * \return the reference to the netif used by LwIP
 */
extern struct netif* get_netif(int idx);

/**
 * \brief set status callback functions when network stats change.
 *
 * \return a pointer to the previous callback function
 */
extern netif_status_callback_fn lwip_set_status_callback(netif_status_callback_fn callback);
extern netif_status_callback_fn lwip_set_link_callback(netif_status_callback_fn callback);

/**
 * \brief Outputs a network byte orderd address to a static charachter array.
 *
 * \return the pointer to the statically allocated buffer.
 */
extern const char* print_ipad(alt_u32 ip, char* buf);

/**
 * \brief The initializer of the LwIP TCP/IP stack
 *
 * Initialize function which should be called by the application. This function 
 * initializes the LwIP TCP/IP Stack and returns when done.
 */
extern void lwip_initialize(int waitForAll);

/**
 * \brief Initialize the PHY's to there default state
 */
extern void lwip_initialize_phys(void);

#ifdef __cplusplus
}
#endif

#endif // __LWIP_MAIN_H_
