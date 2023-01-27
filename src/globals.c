#include "ublox.h"

/**
 * All u-blox mesages are prefixed with : "µb"
 */
const uint8_t sync_chars[2] = {0xb5, 'b'};
