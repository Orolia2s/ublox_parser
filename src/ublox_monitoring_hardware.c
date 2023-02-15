#include "ublox.h"
#include "ublox_enums.h"
#include "ublox_messages.h"

#include <ft_prepro/tools.h>
#include <ft_string.h>

#include <inttypes.h>

#define append(S, ...) string_append_format(&S, __VA_ARGS__)
#define _append_field2(STRING, MESSAGE, TYPE, FIELD) append(STRING, ", " PP_STR(FIELD) ": %" TYPE, MESSAGE->FIELD);
#define _append_field1(STRING, MESSAGE, PAIR) _append_field2(STRING, MESSAGE, PAIR)
#define append_field(STRING, MESSAGE, PAIR) _append_field1(STRING, MESSAGE, ID PAIR)

t_string ublox_monitoring_hardware_tostring(const struct ublox_monitoring_hardware* message)
{
	t_string result = ublox_header_tostring(&message->header);

	FOR(EACH(
		    ("#.8" PRIx32, pins_selected),
		    ("#.8" PRIx32, pins_bank),
		    ("#.8" PRIx32, pins_direction),
		    ("#.8" PRIx32, pins_value),
		    (PRIu16, noise_per_ms),
		    (PRIu16, agc_count)
		    ), append_field, result, message);
	//append_field(result, "#.8" PRIx32, message, pin_sel);
	//append_field(result, "#.8" PRIx32, message, pin_bank);
	//append_field(result, "#.8" PRIx32, message, pin_dir);
	//append_field(result, "#.8" PRIx32, message, pin_val);
	//append_field(result, PRIu16, message, noise_per_ms);
	//append_field(result, PRIu16, message, agc_count);
	append(result, ", antenna_status: %s", ublox_antenna_status_strings[message->antenna_status]);
	append(result, ", antenna_power: %s", ublox_antenna_power_strings[message->antenna_power]);
//	append_field(result, PRIu16, message, is_rtc_calibrated);
	/*
	append(result, ", safe_boot: %b", message->safe_boot);
	append(result, ", jamming_state: %hhu", message->jamming_state);
	append(result, ", xtal_absent: %b", message->xtal_absent);
	append(result, ", used_mask: %#.8x", message->used_mask);
	append(result, ", vp: [");
	for (int i = 0; i < 17; i++)
	{
		append(result, "%#.2hhx, ", message->vp[i]);
	}
	append(result, "] , jam_indicator: %hhu", message->jam_indicator);
	append(result, ", pin_irq: %#.8x", message->pin_irq);
	append(result, ", pull_high: %#.8x", message->pull_high);
	append(result, ", pull_low: %#.8x", message->pull_low);
	*/
	return result;
}
