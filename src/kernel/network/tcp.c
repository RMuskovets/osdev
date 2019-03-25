#include <tcp.h>
#include <ip.h>
#include <string.h>


void tcp_send_packet(uint8_t * dst_ip, uint16_t src_port, uint16_t dst_port, void * data, int len) {

}

void tcp_handle_packet(tcp_packet_t * packet) {

}

uint16_t checksum(void *addr, int count)
{
    /* Compute Internet Checksum for "count" bytes
     *         beginning at location "addr".
     * Taken from https://tools.ietf.org/html/rfc1071
     */

    register uint32_t sum = 0;
    uint16_t * ptr = addr;

    while( count > 1 )  {
        /*  This is the inner loop */
        sum += * ptr++;
        count -= 2;
    }

    /*  Add left-over byte, if any */
    if( count > 0 )
        sum += * (uint8_t *) ptr;

    /*  Fold 32-bit sum to 16 bits */
    while (sum>>16)
        sum = (sum & 0xffff) + (sum >> 16);

    return ~sum;
}

uint16_t calculate_checksum(tcp_packet_t * packet) {
	uint8_t bytes[20];
	tcp_header_t head = packet->header;
	uint16_t crc = head->sport;
	bytes[0] = (crc >> 0) & 0xff;
	bytes[1] = (crc >> 8) & 0xff;
	crc = head->dport;
	bytes[2] = (crc >> 0) & 0xff;
	bytes[3] = (crc >> 8) & 0xff;
	bytes[4] = (head->sqnum >> 0) & 0xff;
	bytes[5] = (head->sqnum >> 8) & 0xff;
	bytes[6] = (head->sqnum >> 16) & 0xff;
	bytes[7] = (head->sqnum >> 24) & 0xff;
	bytes[8] = (head->acnum >> 0) & 0xff;
	bytes[9] = (head->acnum >> 8) & 0xff;
	bytes[10] = (head->acnum >> 16) & 0xff;
	bytes[11] = (head->acnum >> 24) & 0xff;
	bytes[12] = head->doff_reserved_ns;
	bytes[13] = head->flags;
	crc = head->winsz;
	bytes[14] = (crc >> 0) & 0xff;
	bytes[15] = (crc >> 8) & 0xff;
	bytes[16] = (head->checksum >> 0) & 0xff;
	bytes[17] = (head->checksum >> 8) & 0xff;
	bytes[18] = (head-> >> 0) & 0xff;
	bytes[19] = (head-> >> 8) & 0xff;
	return checksum(bytes, 20);
}