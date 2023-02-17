#include "ublox.h"
#include "ublox_enums.h"
#include "ublox_messages.h"

#include <ft_string.h>

#include <inttypes.h> // PRI*

#define append(S, ...) string_append_format(&S, __VA_ARGS__)

t_string ublox_navigation_data_tostring(const struct ublox_navigation_data* message)
{
	t_string result = ublox_header_tostring(&message->header);

	if (is_valid_ublox_constellation(message->constellation))
		append(result, ", constellation: %s",
		       ublox_constellation_strings[message->constellation]);
	else
		append(result, ", constellation: %" PRIu8, message->constellation);
	append(result, ", satellite: %" PRIu8, message->satellite);
	append(result, ", signal: %" PRIu8, message->signal);
	if (message->constellation == GLONASS)
		append(result, ", frequency: %" PRIu8, message->glonass_frequency);
	append(result, ", word_count: %" PRIu8, message->word_count);
	append(result, ", channel: %" PRIu8, message->channel);
	append(result, ", version: %" PRIu8, message->version);
	return result;
}
