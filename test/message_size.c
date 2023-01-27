#include <libunit.h>
#include "ublox.h"

#define MSGSZ_condition(TYPE, EXPECTED_SIZE) sizeof(TYPE) == EXPECTED_SIZE

TEST_SECTION(messages_size, extract_name, MSGSZ_condition,
             (ublox_message_t, 4),
             (ublox_checksum_t, 2),
             (ubx_rxm_sfrbx, sizeof(ublox_message_t) + 8)
)
