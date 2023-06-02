#include "ublox.h"
#include "ublox_enums.h"

#include <ft_string.h>

#include <inttypes.h>

#define append(S, ...) string_append_format(&S, __VA_ARGS__)

t_string ublox_header_tostring(const struct ublox_header* message)
{
	t_string result = NEW_STRING;

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
