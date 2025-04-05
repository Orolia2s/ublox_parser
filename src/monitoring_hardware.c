#include "private.h" // append* boolalpha
#include "ublox.h"
#include "ublox_enums.h"
#include "ublox_messages.h"

#include <blackmagic/for.h> // FOR EACH
#include <o2s/string.h>     // string_t

#include <inttypes.h> // PRI*

string_t ublox_monitoring_hardware_tostring(const struct ublox_monitoring_hardware* message)
{
	string_t result = ublox_header_tostring(&message->header);

	FOR(EACH(("#.8" PRIx32, pins_selected),
	         ("#.8" PRIx32, pins_bank),
	         ("#.8" PRIx32, pins_direction),
	         ("#.8" PRIx32, pins_value),
	         (PRIu16,       noise_per_ms),
	         (PRIu16,       agc_count)),
	    append_field, result, message);
	append(result, ", antenna_status: %s", ublox_antenna_status_to_cstring(message->antenna_status));
	append(result, ", antenna_power: %s",  ublox_antenna_power_to_cstring(message->antenna_power));
	FOR(EACH(("u", is_rtc_calibrated),
	         ("u", in_safe_boot_mode)),
	    append_field, result, message);
	append(result, ", %s", ublox_jamming_state_to_cstring(message->jamming_state));
	FOR(EACH(("u", is_crystal_absent),
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
