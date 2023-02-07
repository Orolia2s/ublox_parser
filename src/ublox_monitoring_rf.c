#include "ublox.h"

#include <ft_string.h>

#define append(S, ...) string_append_format(&S, __VA_ARGS__)

t_string ublox_monitoring_rf_block_tostring(const struct ublox_monitoring_rf_block* message)
{
	t_string result = NEW_STRING;

	append(result, "block_id: %hhu", message->id);
	append(result, ", jamming_state: %hhu", message->jamming_state);
	append(result, ", antenna_status: %hhu", message->antenna_status);
	append(result, ", antenna_power: %hhu", message->antenna_power);
	append(result, ", post_build: %u", message->post_status);
	append(result, ", post_build: %hu", message->noise_per_ms);
	append(result, ", agc_count: %hu", message->agc_count);
	append(result, ", jam_indicator: %hhu", message->jam_indicator);
	append(result, ", ofs_i: %hhi", message->ofs_i);
	append(result, ", mag_i: %hhi", message->mag_i);
	append(result, ", ofs_q: %hhi", message->ofs_q);
	append(result, ", mag_q: %hhi", message->mag_q);
	return result;
}

t_string ublox_monitoring_rf_tostring(const struct ublox_monitoring_rf* message)
{
	t_string                                result = ublox_header_tostring(&message->header);
	const struct ublox_monitoring_rf_block* block  = (message + 1);
	const struct ublox_monitoring_rf_block* const blocks_end = block + message->block_count;

	append(result, ", version: %hhu", message->version);
	append(result, ", n_blocks: %hhu", message->block_count);
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