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

	FOR(EACH(("#.8" PRIx32, pins_selected),
	         ("#.8" PRIx32, pins_bank),
	         ("#.8" PRIx32, pins_direction),
	         ("#.8" PRIx32, pins_value),
	         (PRIu16,       noise_per_ms),
	         (PRIu16,       agc_count)),
	    append_field, result, message);
	append(result, ", antenna_status: %s", cstring_from_ublox_antenna_status(message->antenna_status));
	append(result, ", antenna_power: %s",  cstring_from_ublox_antenna_power(message->antenna_power));
	FOR(EACH(("B", is_rtc_calibrated),
	         ("B", in_safe_boot_mode)),
	    append_field, result, message);
	append(result, ", %s", cstring_from_ublox_jamming_state(message->jamming_state));
	FOR(EACH(("B", is_crystal_absent),
	         ("#.8" PRIx32, used_mask)),
	    append_field, result, message);
	append(result, ", vp: [");
	for (int i = 0; i < 16; i++)
		append(result, "%#.2" PRIx8 ", ", message->virtual_pins[i]);
	append(result, "%#.2" PRIx8 "]", message->virtual_pins[16]);
	FOR(EACH((PRIu8, jam_level),
	         ("#.8" PRIx32, pins_interrupt),
	         ("#.8" PRIx32, pins_pull_high),
	         ("#.8" PRIx32, pins_pull_low)),
	    append_field, result, message);
	return result;
}
