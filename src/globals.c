#include "ublox.h"

/**
 * All u-blox mesages are prefixed with "µb".
 *
 * Every frame starts with a 2-byte preamble consisting of two synchronization
 * characters:
 *  - `0xb5` (181) ISO 8859-1 for µ (greek letter mu)
 *  - `0x62`  (98) ISO 8859-1 for b
 */
const uint8_t sync_chars[2] = {0xb5, 'b'};
