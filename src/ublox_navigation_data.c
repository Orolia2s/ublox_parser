#include "ublox.h"

#include <ft_string.h>

#define append(S, ...) string_append_format(&S, __VA_ARGS__)

t_string ublox_navigation_data_tostring(struct ublox_navigation_data* message)
{
	t_string result = ublox_header_tostring(&message->header);

	if (is_valid_ublox_constellation(message->constellation))
		append(result, ", constellation: %s",
		       ublox_constellation_strings[message->constellation]);
	else
		append(result, ", constellation: %hhu", message->constellation);
	append(result, ", satellite: %hhu", message->satellite);
	append(result, ", signal: %hhu", message->signal);
	if (message->constellation == GLONASS)
		append(result, ", frequency: %hhu", message->glonass_frequency);
	append(result, ", word_count: %hhu", message->word_count);
	append(result, ", channel: %hhu", message->channel);
	append(result, ", version: %hhu", message->version);
	return result;
}
