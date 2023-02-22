#include "ublox.h"
#include "ublox_enums.h"

#include <ft_string.h>

#include <inttypes.h>

#define append(S, ...) string_append_format(&S, __VA_ARGS__)

t_string ublox_header_tostring(const struct ublox_header* message)
{
	t_string result = NEW_STRING;

	append(result, "group: %s, ", ublox_class_strings[message->group]);
	switch (message->group)
	{
	case MON:
		if (!is_valid_ublox_monitoring_message(message->type))
			goto unknown;
		append(result, "type: %s, ", ublox_monitoring_message_strings[message->type]);
		break;
	case RXM:
		if (!is_valid_ublox_receiver_message(message->type))
			goto unknown;
		append(result, "type: %s, ", ublox_receiver_message_strings[message->type]);
		break;
	unknown:
	default: append(result, "type: %#.2" PRIx8 ", ", message->type);
	}
	append(result, "length: %hu", message->length);
	return result;
}
