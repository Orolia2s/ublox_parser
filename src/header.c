#include "private.h" // append
#include "ublox.h"
#include "ublox_enums.h"

#include <o2s/string.h> // string_t

#include <inttypes.h> // PRI*

string_t ublox_header_tostring(const struct ublox_header* message)
{
	string_t result = string_new();

	append(result, "group: %s, ", ublox_class_to_cstring(message->ublox_class));
	switch (message->ublox_class)
	{
	case MON:
		if (!ublox_monitoring_message_is_valid(message->type))
			goto unknown;
		append(result, "type: %s, ", ublox_monitoring_message_to_cstring(message->type));
		break;
	case RXM:
		if (!ublox_receiver_message_is_valid(message->type))
			goto unknown;
		append(result, "type: %s, ", ublox_receiver_message_to_cstring(message->type));
		break;
	case NAV:
		if (!ublox_navigation_message_is_valid(message->type))
			goto unknown;
		append(result, "type: %s, ", ublox_navigation_message_to_cstring(message->type));
		break;
	unknown:
	default: append(result, "type: %#.2" PRIx8 ", ", message->type);
	}
	append(result, "length: %hu", message->length);
	return result;
}
