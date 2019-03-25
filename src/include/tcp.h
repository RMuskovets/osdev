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

#endif
