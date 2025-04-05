#include "private.h" // append
#include "ublox.h"
#include "ublox_enums.h"
#include "ublox_messages.h"

#include <o2s/string.h> // string_t

#include <inttypes.h> // PRI*

string_t ublox_navigation_data_tostring(const struct ublox_navigation_data* message)
{
	string_t result = ublox_header_tostring(&message->header);

	if (ublox_constellation_is_valid(message->constellation))
		append(result, ", constellation: %s",
		       ublox_constellation_to_cstring(message->constellation));
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
