#include "private.h" // append
#include "ublox.h"
#include "ublox_enums.h"

#include <o2s/string.h> // string_t

#include <inttypes.h> // PRI*

string_t ublox_header_tostring(const struct ublox_header* message)
{
	string_t result = string_new();

	append(result, "group: %s, ", cstring_from_ublox_class(message->ublox_class));
	switch (message->ublox_class)
	{
	case MON:
		if (!is_valid_ublox_monitoring_message(message->type))
			goto unknown;
		append(result, "type: %s, ", cstring_from_ublox_monitoring_message(message->type));
		break;
	case RXM:
		if (!is_valid_ublox_receiver_message(message->type))
			goto unknown;
		append(result, "type: %s, ", cstring_from_ublox_receiver_message(message->type));
		break;
	case NAV:
		if (!is_valid_ublox_navigation_message(message->type))
			goto unknown;
		append(result, "type: %s, ", cstring_from_ublox_navigation_message(message->type));
		break;
	unknown:
	default: append(result, "type: %#.2" PRIx8 ", ", message->type);
	}
	append(result, "length: %hu", message->length);
	return result;
}
