#ifndef TCP_H
#define TCP_H
#include <system.h>

typedef struct tcp_packet {
	uint16_t sport;
	uint16_t dport;
	uint32_t sqnum;
	uint32_t acnum;
	uint8_t  doff_reserved_ns;
	uint8_t  flags;
	uint16_t winsz;
	uint16_t checksum;
	uint16_t urgent;
	uint8_t *data;
} tcp_packet_t;

void tcp_send_packet(uint8_t * dst_ip, uint16_t src_port, uint16_t dst_port, void * data, int len);
void tcp_send_packet(tcp_packet_t * packet);

void tcp_handle_packet(tcp_packet_t * packet);


#endif
