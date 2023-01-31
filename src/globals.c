#include "ublox.h"

/**
 * All u-blox mesages are prefixed with "µb".
 *
 * Every frame starts with a 2-byte preamble consisting of two synchronization
 * characters:
 *  - `0xb5` (181) ISO 8859-1 for µ (greek letter mu)
 *  - `0x62`  (98) ISO 8859-1 for b
 */
const uint8_t ublox_sync_chars[2] = {0xb5, 'b'};

/**
 * Size in byte of the smallest message (UBX-ACK)
 */
const size_t ublox_smallest_message_size =
	sizeof(struct ublox_header) + 2 + sizeof(struct ublox_footer);
