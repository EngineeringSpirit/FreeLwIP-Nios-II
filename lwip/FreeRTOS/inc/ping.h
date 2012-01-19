/**
 * \file ping.h
 *
 * This file provides easy access to a ping interface.
 *
 * Created on: 6 jan. 2012
 * 
 * Created by: Engineering Spirit (c) 2012 http://engineering-spirit.nl/
 */

#ifndef __LWIP_PING_H_
#define __LWIP_PING_H_

#ifdef __cplusplus
extern "C"
{
#endif

// include typedefs and such
#include <arch/cc.h>

/**
 * \brief This functions sends a default ICMP ping.
 *
 * This function sends the number of ICMP ping requests you specifie. It can wait for a pong if you like.
 * The return value is counter started from 0 for each successfull packed send. If you set get_response to
 * one the counter will be increased with 1 if it receives a pong replay and decresed with 1 if the pong
 * request times out.
 *
 * \param [in] addr the address in network byte order to ping
 * \param [in] packets number of packets to be send
 * \param [in] get_response set to one if you want to wait for a response
 * \param [in] size the size of the packet to be send (the data send will be byte buffer with sequential count)
 * 
 * \return the counter value
 */
int lwip_ping_target(u32_t addr, u8_t packets, int get_response, u8_t size);

/**
 * \brief This functions sends a default ICMP ping.
 *
 * This function sends the number of ICMP ping requests you specifie. It can wait for a pong if you like.
 * The return value is counter started from 0 for each successfull packed send. If you set get_response to
 * one the counter will be increased with 1 if it receives a pong replay and decresed with 1 if the pong
 * request times out.
 *
 * \param [in] addr the address in network byte order to ping
 * \param [in] packets number of packets to be send
 * \param [in] get_response set to one if you want to wait for a response
 * \param [in] data the data which would be the body of the ICMP ping request
 * \param [in] length the length of the given data
 * 
 * \return the counter value
 */
int lwip_ping_target_data(u32_t addr, u8_t packets, int get_response, u8_t *data, u16_t length);

#ifdef __cplusplus
}
#endif

#endif /* __LWIP_PING_H_ */
