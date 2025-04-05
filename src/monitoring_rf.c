#include "private.h" // append*
#include "ublox.h"
#include "ublox_enums.h"
#include "ublox_messages.h"

#include <o2s/string.h> // string_*

#include <inttypes.h> // PRI*

string_t ublox_monitoring_rf_block_tostring(const struct ublox_monitoring_rf_block* message)
{
	string_t result = string_new();

	append(result, "ID: %" PRIu8 , message->id);
	append(result, ", jamming_state: %s", cstring_from_ublox_jamming_state(message->jamming_state));
	append(result, ", antenna_status: %s", cstring_from_ublox_antenna_status(message->antenna_status));
	append(result, ", antenna_power: %s", cstring_from_ublox_antenna_power(message->antenna_power));
	FOR(EACH((PRIu32, post_status),
	         (PRIu16, noise_per_ms),
	         (PRIu16, agc_count),
	         (PRIu8,  jam_level),
	         (PRIi8,  offset_i),
	         (PRIu8,  magnitude_i),
	         (PRIi8,  offset_q),
	         (PRIu8,  magnitude_q)),
	    append_field, result, message);
	return result;
}

string_t ublox_monitoring_rf_tostring(const struct ublox_monitoring_rf* message)
{
	string_t                                      result = ublox_header_tostring(&message->header);
	const struct ublox_monitoring_rf_block*       block = (const struct ublox_monitoring_rf_block*)(message + 1);
	const struct ublox_monitoring_rf_block* const blocks_end = block + message->block_count;

	append_field(result, message, (PRIu8, version));
	append_field(result, message, (PRIu8, block_count));
	string_append_literal(&result, ", blocks: [");
	while (block < blocks_end)
	{
		RAII(t_string) str = ublox_monitoring_rf_block_tostring(block);
		append(result, "{%s},", cstring(&str));
		block++;
	}
	string_append_char(&result, ']');
	return result;
}
