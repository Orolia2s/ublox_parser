#include "ublox.h"

#include <ft_string.h>

#define append(S, ...) string_append_format(&S, __VA_ARGS__)

t_string ublox_monitoring_hardware_tostring(struct ublox_monitoring_hardware* message)
{
	t_string result = ublox_header_tostring(&message->header);

	append(result, ", pin_sel: %#.8x", message->pin_sel);
	append(result, ", pin_bank: %#.8x", message->pin_bank);
	append(result, ", pin_dir: %#.8x", message->pin_dir);
	append(result, ", pin_val: %#.8x", message->pin_val);
	append(result, ", noise_per_ms: %hu", message->noise_per_ms);
	append(result, ", agc_count: %hu", message->agc_count);
	append(result, ", antenna_status: %hhu", message->antenna_status);
	append(result, ", antenna_power: %hhu", message->antenna_power);
	append(result, ", rtc_calib: %b", message->rtc_calib);
	append(result, ", safe_boot: %b", message->safe_boot);
	append(result, ", jamming_state: %hhu", message->jamming_state);
	append(result, ", xtal_absent: %b", message->xtal_absent);
	append(result, ", used_mask: %#.8x", message->used_mask);
	append(result, ", vp: [");
	for (int i = 0; i < 17; i++)
	{
		append(result, ", used_mask: %#.2hhx", message->vp[i]);
	}
	append(result, "] , jam_indicator: %hhu", message->jam_indicator);
	append(result, ", pin_irq: %#.8x", message->pin_irq);
	append(result, ", pull_high: %#.8x", message->pull_high);
	append(result, ", pull_low: %#.8x", message->pull_low);
	return result;
}
