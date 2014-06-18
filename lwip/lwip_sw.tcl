#
# lwip_sw.tcl
#
# A description of the LwIP TCP/IP stack,
# version 1.4.1, for use with the Nios II BSP tools
#

# Create a new software package named "LwIP"
create_sw_package lwip

# The version of this software
# Note: Version assignment is based on version of Nios II EDS that
#       this was distributed with. The LwIP Stack version is v1.4.1
set_sw_property version 12.0

# (Don't) initialize the driver in alt_sys_init()
set_sw_property auto_initialize false

# Location in generated BSP that above sources will be copied into
set_sw_property bsp_subdirectory LwIP

#
# Source file listings...
#

# C/C++ source files
add_sw_property c_source FreeRTOS/src/ping.c
add_sw_property c_source FreeRTOS/src/api/api_lib.c
add_sw_property c_source FreeRTOS/src/api/api_msg.c
add_sw_property c_source FreeRTOS/src/api/err.c
add_sw_property c_source FreeRTOS/src/api/netbuf.c
add_sw_property c_source FreeRTOS/src/api/netdb.c
add_sw_property c_source FreeRTOS/src/api/netifapi.c
add_sw_property c_source FreeRTOS/src/api/sockets.c
add_sw_property c_source FreeRTOS/src/api/tcpip.c
add_sw_property c_source FreeRTOS/src/arch/altera_tse_ethernetif.c
add_sw_property c_source FreeRTOS/src/arch/alt_lwip_close.c
add_sw_property c_source FreeRTOS/src/arch/alt_lwip_fcntl.c
add_sw_property c_source FreeRTOS/src/arch/alt_lwip_read.c
add_sw_property c_source FreeRTOS/src/arch/alt_lwip_write.c
add_sw_property c_source FreeRTOS/src/arch/lwip_main.c
add_sw_property c_source FreeRTOS/src/arch/lwip_tse_mac.c
add_sw_property c_source FreeRTOS/src/arch/sys_arch.c
add_sw_property c_source FreeRTOS/src/core/def.c
add_sw_property c_source FreeRTOS/src/core/dhcp.c
add_sw_property c_source FreeRTOS/src/core/dns.c
add_sw_property c_source FreeRTOS/src/core/init.c
add_sw_property c_source FreeRTOS/src/core/ipv4/autoip.c
add_sw_property c_source FreeRTOS/src/core/ipv4/icmp.c
add_sw_property c_source FreeRTOS/src/core/ipv4/igmp.c
add_sw_property c_source FreeRTOS/src/core/ipv4/inet.c
add_sw_property c_source FreeRTOS/src/core/ipv4/inet_chksum.c
add_sw_property c_source FreeRTOS/src/core/ipv4/ip.c
add_sw_property c_source FreeRTOS/src/core/ipv4/ip_addr.c
add_sw_property c_source FreeRTOS/src/core/ipv4/ip_frag.c
add_sw_property c_source FreeRTOS/src/core/mem.c
add_sw_property c_source FreeRTOS/src/core/memp.c
add_sw_property c_source FreeRTOS/src/core/netif.c
add_sw_property c_source FreeRTOS/src/core/pbuf.c
add_sw_property c_source FreeRTOS/src/core/raw.c
add_sw_property c_source FreeRTOS/src/core/snmp/asn1_dec.c
add_sw_property c_source FreeRTOS/src/core/snmp/asn1_enc.c
add_sw_property c_source FreeRTOS/src/core/snmp/mib2.c
add_sw_property c_source FreeRTOS/src/core/snmp/mib_structs.c
add_sw_property c_source FreeRTOS/src/core/snmp/msg_in.c
add_sw_property c_source FreeRTOS/src/core/snmp/msg_out.c
add_sw_property c_source FreeRTOS/src/core/stats.c
add_sw_property c_source FreeRTOS/src/core/sys.c
add_sw_property c_source FreeRTOS/src/core/tcp.c
add_sw_property c_source FreeRTOS/src/core/tcp_in.c
add_sw_property c_source FreeRTOS/src/core/tcp_out.c
add_sw_property c_source FreeRTOS/src/core/timers.c
add_sw_property c_source FreeRTOS/src/core/udp.c
add_sw_property c_source FreeRTOS/src/netif/etharp.c
add_sw_property c_source FreeRTOS/src/netif/ethernetif.c
add_sw_property c_source FreeRTOS/src/netif/slipif.c
add_sw_property c_source FreeRTOS/src/netif/ppp/auth.c
add_sw_property c_source FreeRTOS/src/netif/ppp/chap.c
add_sw_property c_source FreeRTOS/src/netif/ppp/chpms.c
add_sw_property c_source FreeRTOS/src/netif/ppp/fsm.c
add_sw_property c_source FreeRTOS/src/netif/ppp/ipcp.c
add_sw_property c_source FreeRTOS/src/netif/ppp/lcp.c
add_sw_property c_source FreeRTOS/src/netif/ppp/magic.c
add_sw_property c_source FreeRTOS/src/netif/ppp/md5.c
add_sw_property c_source FreeRTOS/src/netif/ppp/pap.c
add_sw_property c_source FreeRTOS/src/netif/ppp/ppp.c
add_sw_property c_source FreeRTOS/src/netif/ppp/ppp_oe.c
add_sw_property c_source FreeRTOS/src/netif/ppp/randm.c
add_sw_property c_source FreeRTOS/src/netif/ppp/vj.c

# Include files
add_sw_property include_source FreeRTOS/inc/altera_avalon_tse.h
add_sw_property include_source FreeRTOS/inc/ping.h
add_sw_property include_source FreeRTOS/inc/altera_tse_ethernetif.h
add_sw_property include_source FreeRTOS/inc/lwipopts.h
add_sw_property include_source FreeRTOS/inc/lwippools.h
add_sw_property include_source FreeRTOS/inc/lwip_main.h
add_sw_property include_source FreeRTOS/inc/triple_speed_ethernet_regs.h
add_sw_property include_source FreeRTOS/inc/arch/cc.h
add_sw_property include_source FreeRTOS/inc/arch/perf.h
add_sw_property include_source FreeRTOS/inc/arch/sys_arch.h
add_sw_property include_source FreeRTOS/inc/ipv4/lwip/autoip.h
add_sw_property include_source FreeRTOS/inc/ipv4/lwip/icmp.h
add_sw_property include_source FreeRTOS/inc/ipv4/lwip/igmp.h
add_sw_property include_source FreeRTOS/inc/ipv4/lwip/inet.h
add_sw_property include_source FreeRTOS/inc/ipv4/lwip/inet_chksum.h
add_sw_property include_source FreeRTOS/inc/ipv4/lwip/ip.h
add_sw_property include_source FreeRTOS/inc/ipv4/lwip/ip_addr.h
add_sw_property include_source FreeRTOS/inc/ipv4/lwip/ip_frag.h
add_sw_property include_source FreeRTOS/inc/lwip/api.h
add_sw_property include_source FreeRTOS/inc/lwip/api_msg.h
add_sw_property include_source FreeRTOS/inc/lwip/arch.h
add_sw_property include_source FreeRTOS/inc/lwip/debug.h
add_sw_property include_source FreeRTOS/inc/lwip/def.h
add_sw_property include_source FreeRTOS/inc/lwip/dhcp.h
add_sw_property include_source FreeRTOS/inc/lwip/dns.h
add_sw_property include_source FreeRTOS/inc/lwip/err.h
add_sw_property include_source FreeRTOS/inc/lwip/init.h
add_sw_property include_source FreeRTOS/inc/lwip/mem.h
add_sw_property include_source FreeRTOS/inc/lwip/memp.h
add_sw_property include_source FreeRTOS/inc/lwip/memp_std.h
add_sw_property include_source FreeRTOS/inc/lwip/netbuf.h
add_sw_property include_source FreeRTOS/inc/lwip/netdb.h
add_sw_property include_source FreeRTOS/inc/lwip/netif.h
add_sw_property include_source FreeRTOS/inc/lwip/netifapi.h
add_sw_property include_source FreeRTOS/inc/lwip/opt.h
add_sw_property include_source FreeRTOS/inc/lwip/pbuf.h
add_sw_property include_source FreeRTOS/inc/lwip/raw.h
add_sw_property include_source FreeRTOS/inc/lwip/sio.h
add_sw_property include_source FreeRTOS/inc/lwip/snmp.h
add_sw_property include_source FreeRTOS/inc/lwip/snmp_asn1.h
add_sw_property include_source FreeRTOS/inc/lwip/snmp_msg.h
add_sw_property include_source FreeRTOS/inc/lwip/snmp_structs.h
add_sw_property include_source FreeRTOS/inc/lwip/sockets.h
add_sw_property include_source FreeRTOS/inc/lwip/stats.h
add_sw_property include_source FreeRTOS/inc/lwip/sys.h
add_sw_property include_source FreeRTOS/inc/lwip/tcp.h
add_sw_property include_source FreeRTOS/inc/lwip/tcpip.h
add_sw_property include_source FreeRTOS/inc/lwip/tcp_impl.h
add_sw_property include_source FreeRTOS/inc/lwip/timers.h
add_sw_property include_source FreeRTOS/inc/lwip/udp.h
add_sw_property include_source FreeRTOS/inc/netif/etharp.h
add_sw_property include_source FreeRTOS/inc/netif/ppp_oe.h
add_sw_property include_source FreeRTOS/inc/netif/slipif.h
add_sw_property include_source FreeRTOS/inc/os/alt_syscall.h
add_sw_property include_source FreeRTOS/inc/netif/ppp/auth.h
add_sw_property include_source FreeRTOS/inc/netif/ppp/chap.h
add_sw_property include_source FreeRTOS/inc/netif/ppp/chpms.h
add_sw_property include_source FreeRTOS/inc/netif/ppp/fsm.h
add_sw_property include_source FreeRTOS/inc/netif/ppp/ipcp.h
add_sw_property include_source FreeRTOS/inc/netif/ppp/lcp.h
add_sw_property include_source FreeRTOS/inc/netif/ppp/magic.h
add_sw_property include_source FreeRTOS/inc/netif/ppp/md5.h
add_sw_property include_source FreeRTOS/inc/netif/ppp/pap.h
add_sw_property include_source FreeRTOS/inc/netif/ppp/ppp.h
add_sw_property include_source FreeRTOS/inc/netif/ppp/pppdebug.h
add_sw_property include_source FreeRTOS/inc/netif/ppp/randm.h
add_sw_property include_source FreeRTOS/inc/netif/ppp/vj.h


# Include paths
add_sw_property include_directory inc
add_sw_property include_directory inc/ipv4

# Overriden HAL files
add_sw_property excluded_hal_source HAL/inc/os/alt_syscall.h
add_sw_property excluded_hal_source drivers/inc/altera_avalon_tse.h

# This driver supports only FreeRTOS BSP (OS) type
add_sw_property supported_bsp_type FreeRTOS

# Add preprocessor definitions to public makefile: ALT_LWIP
add_sw_property alt_cppflags_addition "-DALT_LWIP"


# LwIP configuration options
add_sw_setting boolean system_h_define checksum.enable_generate_ip CONF_LWIP_CHECKSUM_GEN_IP true "Enable generation of checksums for IP frames"
add_sw_setting boolean system_h_define checksum.enable_generate_udp CONF_LWIP_CHECKSUM_GEN_UDP true "Enable generation of checksums for UDP frames"
add_sw_setting boolean system_h_define checksum.enable_generate_tcp CONF_LWIP_CHECKSUM_GEN_TCP true "Enable generation of checksums for TCP frames"

add_sw_setting boolean system_h_define checksum.enable_check_ip CONF_LWIP_CHECKSUM_CHECK_IP true "Enable checking of checksums from IP frames"
add_sw_setting boolean system_h_define checksum.enable_check_udp CONF_LWIP_CHECKSUM_CHECK_UDP true "Enable checking of checksums from UDP frames"
add_sw_setting boolean system_h_define checksum.enable_check_tcp CONF_LWIP_CHECKSUM_CHECK_TCP true "Enable checking of checksums from TCP frames"

add_sw_setting boolean system_h_define proto.enable_vlan CONF_LWIP_PROTO_VLAN false "Enable VLAN support"
add_sw_setting boolean system_h_define proto.enable_icmp CONF_LWIP_PROTO_ICMP true "Enable ICMP support"
add_sw_setting boolean system_h_define proto.enable_autoip CONF_LWIP_PROTO_AUTOIP false "Enable AutoIP support"
add_sw_setting boolean system_h_define proto.enable_snmp CONF_LWIP_PROTO_SNMP false "Enable SNMP support"
add_sw_setting boolean system_h_define proto.enable_igmp CONF_LWIP_PROTO_IGMP false "Enable IGMP support"
add_sw_setting boolean system_h_define proto.enable_dns CONF_LWIP_PROTO_DNS false "Enable DNS support"
add_sw_setting boolean system_h_define proto.enable_udp CONF_LWIP_PROTO_UDP true "Enable UDP support"
add_sw_setting boolean system_h_define proto.enable_udp_lite CONF_LWIP_PROTO_UDP_LITE false "Enable UDP Lite support"
add_sw_setting boolean system_h_define proto.enable_tcp CONF_LWIP_PROTO_TCP true "Enable TCP support"
add_sw_setting boolean system_h_define proto.enable_dhcp CONF_LWIP_PROTO_DHCP true "Enable DHCP support"

add_sw_setting decimal_number system_h_define connections.raw_pcb_count CONF_LWIP_RAW_PCB 2 "Number of raw sockets supported (Used by ICMP for example)"
add_sw_setting decimal_number system_h_define connections.udp_pcb_count CONF_LWIP_UDP_PCB 15 "Number of UDP sockets supported"
add_sw_setting decimal_number system_h_define connections.tcp_pcb_count CONF_LWIP_TCP_PCB 15 "Number of TCP sockets supported"
add_sw_setting decimal_number system_h_define connections.tcp_listen_pcb_count CONF_LWIP_TCP_PCB_LISTEN 10 "Number of TCP Listening sockets supported"
add_sw_setting decimal_number system_h_define connections.advanced.tcp_segments CONF_LWIP_MEMP_TCP_SEG 16 "The number of simultaneously queued TCP segments"
add_sw_setting decimal_number system_h_define connections.advanced.reass_data CONF_LWIP_MEMP_REASSDATA 5 "The number of simultaneously IP packets queued for reassembly (whole packets not fragments!)"
add_sw_setting decimal_number system_h_define connections.advanced.arp_queue CONF_LWIP_MEMP_ARP_QUEUE 5 "the number of simulateously queued outgoing packets defined for ARP resolve"
add_sw_setting decimal_number system_h_define connections.advanced.netbufs CONF_LWIP_MEMP_NETBUFS 10 "the number of struct netbufs. (only needed if you use the sequential API, like api_lib.c)"
add_sw_setting decimal_number system_h_define connections.advanced.pbuf_pool CONF_LWIP_MEMP_PBUF_POOL_SIZE 64 "the number of packet buffers available in the memory pool"
add_sw_setting decimal_number system_h_define connections.advanced.netconns CONF_LWIP_MEMP_NETCONNS 30 "the number of struct netconns. (only needed if you use the sequential API, like api_lib.c)"
add_sw_setting decimal_number system_h_define connections.advanced.sys_timeout CONF_LWIP_MEMP_SYS_TIMEOUT 10 "the number of simulateously active timeouts."
add_sw_setting decimal_number system_h_define connections.advanced.tcpip_msg_api CONF_LWIP_MEMP_TCPIP_MSG_API 10 "The number of tcpip_msg structs which are used for callback/timeout API communication"
add_sw_setting decimal_number system_h_define connections.advanced.tcpip_msg_inpkt CONF_LWIP_MEMP_TCPIP_MSG_INPKT 50 "The number of tcpip_msg structs which are used for incomming packets"


add_sw_setting boolean system_h_define memory.use_memory_pools CONF_LWIP_USE_MEMPOOLS false "When memory pools are not used a memory block with memory size of mem_size is used."
add_sw_setting decimal_number system_h_define memory.mem_size CONF_LWIP_MEM_SIZE 32768 "Size of the memory pool, only used when not using the memory pool"
add_sw_setting boolean system_h_define memory.overflow_check CONF_LWIP_OVERFLOW_CHECK false "Do memory overflow checking"
add_sw_setting decimal_number system_h_define memory.pbuf_count CONF_LWIP_PBUF_COUNT 32 "Number of packet buffers supported"
add_sw_setting boolean system_h_define memory.malloc_mem_pool.try_bigger_pools CONF_LWIP_MALLOC_MEMPOOL_TRY_BIGGER_POOLS false "If there is no free buffer with the given pool, can we use a bigger pool? Warning: This could waste memory, but also make the system more reliable."
add_sw_setting decimal_number system_h_define memory.malloc_mem_pool.buffers_64 CONF_LWIP_MALLOC_MEMPOOL_64 8 "The number of 64 byte buffers to place in the memory pool"
add_sw_setting decimal_number system_h_define memory.malloc_mem_pool.buffers_128 CONF_LWIP_MALLOC_MEMPOOL_128 4 "The number of 128 byte buffers to place in the memory pool"
add_sw_setting decimal_number system_h_define memory.malloc_mem_pool.buffers_256 CONF_LWIP_MALLOC_MEMPOOL_256 4 "The number of 256 byte buffers to place in the memory pool"
add_sw_setting decimal_number system_h_define memory.malloc_mem_pool.buffers_512 CONF_LWIP_MALLOC_MEMPOOL_512 4 "The number of 512 byte buffers to place in the memory pool"
add_sw_setting decimal_number system_h_define memory.malloc_mem_pool.buffers_512 CONF_LWIP_MALLOC_MEMPOOL_768 4 "The number of 768 byte buffers to place in the memory pool"
add_sw_setting decimal_number system_h_define memory.malloc_mem_pool.buffers_1024 CONF_LWIP_MALLOC_MEMPOOL_1024 4 "The number of 1024 byte buffers to place in the memory pool"
add_sw_setting decimal_number system_h_define memory.malloc_mem_pool.buffers_1536 CONF_LWIP_MALLOC_MEMPOOL_1536 20 "The number of 1536 byte buffers to place in the memory pool"

add_sw_setting boolean system_h_define loopback CONF_LWIP_LOOPBACK false "Loopback outgoing traffic to our selfs"
add_sw_setting boolean system_h_define loopback_if CONF_LWIP_LOOPBACKIF false "Create a loopback interface 'lo'"
add_sw_setting boolean system_h_define stats CONF_LWIP_STATS false "Keep track of the LwIP traffic statistics"
add_sw_setting boolean system_h_define ip_forward CONF_LWIP_IP_FORWARD false "Keep track of the LwIP traffic statistics"
add_sw_setting decimal_number system_h_define ttl CONF_LWIP_DEFAULT_TTL 64 "The default TTL (Time To Life) set to a packet"

# End of file
