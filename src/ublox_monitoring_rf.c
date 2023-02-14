#include "ublox.h"
#include "ublox_messages.h"

#include <ft_string.h>
#include <ft_prepro/tools.h>
#include <inttypes.h>

#define append(S, ...) string_append_format(&S, __VA_ARGS__)
#define append_field(S, TYPE, MSG, FIELD) append(S, ", " PP_STR(FIELD) ": %" TYPE, MSG->FIELD);

t_string ublox_monitoring_rf_block_tostring(const struct ublox_monitoring_rf_block* message)
{
	t_string result = NEW_STRING;

	append(result, "ID: %" PRIu8 , message->id);
	append_field(result, PRIu8, message, jamming_state);
	append_field(result, PRIu8, message, antenna_status);
	append_field(result, PRIu8, message, antenna_power);
	append_field(result, PRIu32, message, post_status);
	append_field(result, PRIu16, message, noise_per_ms);
	append_field(result, PRIu16, message, agc_count);
	append_field(result, PRIu8, message, jam_suppression);
	append_field(result, PRIi8, message, offset_i);
	append_field(result, PRIu8, message, magnitude_i);
	append_field(result, PRIi8, message, offset_q);
	append_field(result, PRIu8, message, magnitude_q);
	return result;
}

t_string ublox_monitoring_rf_tostring(const struct ublox_monitoring_rf* message)
{
	t_string                                result = ublox_header_tostring(&message->header);
	const struct ublox_monitoring_rf_block* block = (const struct ublox_monitoring_rf_block*)(message + 1);
	const struct ublox_monitoring_rf_block* const blocks_end = block + message->block_count;

	append_field(result, PRIu8, message, version);
	append_field(result, PRIu8, message, block_count);
	append(result, ", blocks: [");
	while (block < blocks_end)
	{
		RAII(t_string) str = ublox_monitoring_rf_block_tostring(block);
		append(result, "{%s},", cstring(&str));
		block++;
	}
	append(result, "]");
	return result;
}
