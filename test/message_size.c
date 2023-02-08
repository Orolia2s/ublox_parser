#include "ublox.h"

#include <libunit.h>

// clang-format off

#define MSGSZ_condition(NAME, TYPE, EXPECTED)	  \
	sizeof(TYPE) == EXPECTED

TEST_SECTION(messages_size, extract_name, MSGSZ_condition,
	(ublox_message,  ublox_message_t,                                        4),
	(ublox_checksum, ublox_checksum_t,                                       2),
	(ubx_mon_rf,     struct ublox_monitoring_rf,   sizeof(ublox_message_t) + 4),
	(ubx_mon_hw,     struct ublox_monitoring_hardware,sizeof(ublox_message_t) + 60),
	(ubx_rxm_sfrbx,  struct ublox_navigation_data, sizeof(ublox_message_t) + 8)
)
